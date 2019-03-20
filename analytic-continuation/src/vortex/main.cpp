#include <iostream>
#include <vector>
#include <fstream>
#include <cmdline.hpp>

#include "../rungekutta4.hpp"

#include "vortex.hpp"

class RangeConsoleOutput: public FluxOutput
{
  unsigned int m_lastStep;
  unsigned int m_stride;
public:
  RangeConsoleOutput(unsigned int lastStep,
		     unsigned int stride): FluxOutput(std::cout),
					   m_lastStep(lastStep),
					   m_stride(stride)
  {}
  virtual void write(const Coord& c, const function&f, unsigned int Step)
  {
    if(Step%m_stride == 0)
      {
	FluxOutput::writeCartesian(c,f);
	m_flux << std::endl;
      }
    
    if(Step == m_lastStep) m_flux << std::endl;
  }
};


double LoadDatafile(const std::string& filename, std::vector<double>& A,
			  std::vector<double>& F)
{
  F.clear();
  A.clear();
  std::ifstream ifs(filename.c_str(), std::ios::in);
  double r(0.), a(0.), f(0.);
  while(ifs)
    {
      ifs >> r >> a >> f;
      A.push_back(a);
      F.push_back(f);
    }
  std::clog << "Loaded: " << filename << " (" << A.size() << " entries, Rmax: " << r << ")" << std::endl;
  return r;
}

void ComputeDerivative(const std::vector<double>& fnc, std::vector<double>& deriv, n_range radius)
{
  double h(radius.h());

  deriv.clear();
  deriv.resize(fnc.size());
  deriv[0] = (fnc[1]-fnc[0])/h;
  for(unsigned int i(1);i<fnc.size()-1;++i)
    deriv[i] = (fnc[i+1]-fnc[i-1])/(2.*h);
  deriv.back() = (fnc.back()-fnc[fnc.size()-2])/h;
}

/*class IntegrateOverPath: public Output
{
  std::vector<std::complex<double> > m_coords;
  std::vector<std::complex<double> > m_func;
public:
  virtual void write(const Coord& c, const function& i, unsigned int Step)
  {
    m_coords.push_back(std::complex<double>(c.x(),c.y()));
    m_func.push_back(std::complex<double>(i.real()[0],i.imag()[0]));
    std::cout << c.x() << " "
	      << c.y() << " "
	      << i.real() << " "
	      << i.imag() << std::endl;
  }
  std::complex<double> value()
  {
    std::complex<double> sum(0.,0.);
    for(unsigned int k(0);k<m_func.size()-1;++k)
      sum += m_func[k]*(m_coords[k+1]-m_coords[k]);
    return sum;
  }
};

class ShiftedPolarCoord: public Coord
{
  double m_r, m_t;
  static const double m_dx, m_dy;

public:
  ShiftedPolarCoord(double _r, double _t): m_r(std::abs(_r)), m_t(_t){}
  ShiftedPolarCoord(const Coord& c): m_r(c.r()), m_t(c.t()) {}
  virtual double r() const { throw 0; }
  virtual double t() const { throw 0; }
  virtual double x() const { return m_dx + m_r*std::cos(m_t); };
  virtual double y() const { return m_dy + m_r*std::sin(m_t); };

  virtual double dxdnu() const
  { return -m_r*std::sin(m_t); }
  virtual double dydnu() const
  { return m_r*std::cos(m_t); }
};

const double ShiftedPolarCoord::m_dx(0.);
const double ShiftedPolarCoord::m_dy(1.7);*/

int main(int argc, char** argv)
{
  CmdLine cmd(argc, argv);

  std::string filename(cmd.Get<std::string>("sol")[0]);
  
  unsigned int Steps(cmd.Get<unsigned int>("steps")[0]);

  std::cout.precision(15);
  n_range theta(0.,cmd.Get<double>("theta")[0],Steps);
  //n_range theta(0.,M_PI/2.+0.1,Steps);
  //n_range y(0.,8.,Steps);

  std::vector<double> SolutionOnTheRealLineA;
  std::vector<double> DerivativeOnTheRealLineA;

  std::vector<double> SolutionOnTheRealLineF;
  std::vector<double> DerivativeOnTheRealLineF;

  double R(LoadDatafile(filename,
			SolutionOnTheRealLineA,
			SolutionOnTheRealLineF));

  unsigned int rSteps(SolutionOnTheRealLineA.size()-1);
  n_range radius(0,R,rSteps);
  
  ComputeDerivative(SolutionOnTheRealLineA, DerivativeOnTheRealLineA, radius);
  ComputeDerivative(SolutionOnTheRealLineF, DerivativeOnTheRealLineF, radius);

  std::valarray<double> initialrealpart(4);


  MyProblem p(1.,1.,cmd.Get<double>("lambda")[0]);
  RungeKutta4<MyProblem> rk4(p);
  RangeConsoleOutput out(Steps, cmd.Get<unsigned int>("stride")[0]);

  unsigned int increment(cmd.Get<unsigned int>("incr")[0]);
  double rmax(cmd.Get<double>("rmax")[0]);
  for(unsigned int k(0);k<=rSteps && radius[k]<rmax;k+=increment)
    {
      initialrealpart[0] = SolutionOnTheRealLineA[k]; //Ar
      initialrealpart[1] = DerivativeOnTheRealLineA[k];//Br
      initialrealpart[2] = SolutionOnTheRealLineF[k];  //Fr
      initialrealpart[3] = DerivativeOnTheRealLineF[k];//Gr
      
      const function ic(initialrealpart,
			std::valarray<double>(0., 4));
      
      rk4.GenericCoordinatesIntegration<RangeConsoleOutput, PolarCoord>(radius[k], theta, ic, out);
    }


  /*
    Let's integrate around a singularity:
  */

  /*
    double h(1.7);
  unsigned int K(10);
  double rad(radius[K]);
  std::clog << "Circle radius: " << rad << std::endl;

  n_range straight_path(0,h, 2000);
  n_range round_path(0,2*M_PI,2000);

  initialrealpart[0] = SolutionOnTheRealLineA[K]; //Ar
  initialrealpart[1] = DerivativeOnTheRealLineA[K];//Br
  initialrealpart[2] = SolutionOnTheRealLineF[K];  //Fr
  initialrealpart[3] = DerivativeOnTheRealLineF[K];//Gr

  function ic(initialrealpart,
	      std::valarray<double>(0.,      4));
  
  NullOutput nullOut;
  IntegrateOverPath intOut;

  ic = rk4.GenericCoordinatesIntegration<NullOutput, CartCoord>(rad, straight_path, ic, nullOut);
  rk4.GenericCoordinatesIntegration<IntegrateOverPath, ShiftedPolarCoord>(rad, round_path, ic, intOut);

  std::clog << "Value of the integral: " << intOut.value() << std::endl;
  */

  return 0;
}
