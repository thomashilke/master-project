#include <sys/time.h>
#include <vector>

class Chrono
{
public:
  virtual ~Chrono(){}
  virtual double getTimeFromInit() = 0;
  virtual double getTime() = 0;
  virtual double getDelta() = 0;
};

class FakeChrono: public Chrono
{
  double time;
  double h;
public:
  FakeChrono(): time(0.), h(0.01) {}
  virtual double getTimeFromInit(){ return time+=h; }
  virtual double getTime(){ return time; }
  virtual double getDelta(){ return h; }  
};

  class UnixChrono: public Chrono
{
  timeval m_init;
  timeval m_lasttick;
public:
  UnixChrono()
  {
    gettimeofday(&m_init, NULL);
    m_lasttick = m_init;
  }

  double getTimeFromInit()
  {
    timeval t;
    gettimeofday(&t, NULL);
    return (t.tv_sec-m_init.tv_sec)+(double)(t.tv_usec-m_init.tv_usec)/1000000.;
  }
  double getTime()
  {
    timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec+(double)t.tv_usec/1000000.;
  }
  double getDelta()
  {
    timeval t;
    gettimeofday(&t, NULL);
    double d((t.tv_sec-m_lasttick.tv_sec)+(double)(t.tv_usec-m_lasttick.tv_usec)/1000000.);
    m_lasttick = t;
    return d;
  }
};

class SlopeScrollFunction
{
  double m_starttime;
  double m_slopeduration;
  double m_stepsize;
public:
  SlopeScrollFunction(double StartTime, double StepSize): m_starttime(StartTime),
					 m_slopeduration(.1),
					 m_stepsize(StepSize)
  {}
  void setStartTime(double StartTime){m_starttime = StartTime;}
  double operator()(double t)
  {
    if(t > m_starttime+m_slopeduration)
      return m_stepsize;
    if(t<m_starttime)
      return 0.;
    return (t-m_starttime)/m_slopeduration*m_stepsize;
  }
};

class DampedDynamic
{
  Chrono* m_time;
  std::vector<SlopeScrollFunction> m_slopes;
  double m_alpha;
  double m_lambda;

public:
  double f(double t)
  {
    double r(0.);
    for(unsigned int i(0);i<m_slopes.size();++i)
      r += m_slopes[i](t);
    return r;
  }

  DampedDynamic(Chrono* c): m_time(c),
					   m_alpha(0.),
					   m_lambda(8.)
  {}
  void Update()
  {
    double t(m_time->getTimeFromInit());
    double dt(m_time->getDelta());
    double lambda(m_lambda<1./dt?m_lambda:0.9/dt);
    m_alpha = m_alpha-lambda*dt*(m_alpha-f(t));
    //std::cout << t << " " << m_alpha << " " << f(t) << std::endl;
  }
  double getValue()
  { return m_alpha; }
  
  void addSlope(const SlopeScrollFunction& s)
  {
    m_slopes.push_back(s);
    //m_slopes.back().setStartTime(m_time.getTimeFromInit());
  }
};
