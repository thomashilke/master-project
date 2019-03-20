#include <valarray>

#include <SmartPaper/utils/stringbuilder.hpp>

#include <matrix/matrix.hpp>
#include <matrix/io.hpp>
#include <matrix/algorithms.hpp>

typedef std::valarray<double> state;

std::ostream& operator<<(std::ostream& flux, const state& y)
{
  for(unsigned int k(0);k<y.size();++k)
    flux << y[k] << " ";
  return flux;
}

template<typename Problem>
class RungeKutta4
{
  const Problem& m_p;
  state k1;
  state k2;
  state k3;
  state k4;
public:
  RungeKutta4(const Problem& p): m_p(p),
				 k1(p.size()),
				 k2(p.size()),
				 k3(p.size()),
				 k4(p.size())
  {}
  
  state step(double t, const state& y, double dt) const
  {
    k1 = (dt*m_p(t       , y));
    k2 = (dt*m_p(t+0.5*dt, y+0.5*k1));
    k3 = (dt*m_p(t+0.5*dt, y+0.5*k2));
    k4 = (dt*m_p(t+    dt, y+k3));
    
    return y+(k1+2.*k2+2.*k3+k4)/6.;
  }
};

template<typename Problem>
class Euler
{
  Problem& f;
public:
  Euler(Problem& p): f(p){}
  
  state step(double t, const state& y, double dt)
  { return y+dt*f(t,y); }
};

state max_(const state& v1, const state& v2)
{
  state r(v1.size());
  for(unsigned int k(0);k<v1.size();++k)
    r[k] = std::max(v1[k], v2[k]);
  return r;
}

template<typename Problem>
class DormandPrince
{
  const Problem& f;
  state k1;
  state k2;
  state k3;
  state k4;
  state k5;
  state k6;
  state k7;

  double atol;
  double rtol;

  double t;
  double dt;
  state y;
public:
  DormandPrince(const Problem& p, const state& y,
		double t, double dt): f(p),
				      k1(p.size()), k2(p.size()), k3(p.size()),
				      k4(p.size()), k5(p.size()), k6(p.size()),
				      k7(p.size()),
				      atol(1.e-8), rtol(1.e-8),
				      t(t), dt(dt), y(y)
  {}
  
  const state& getState(){ return y; }
  const double& getTime(){ return t; }

  void step()
  { 
    //fifth order
    static const double b1(35./384.), b2(0.), b3(500./1113.),
      b4(125./192.), b5(-2187./6784.), b6(11./84.), b7(0.);
    
    //fourth order:
    static const double bc1(5179./57600.), bc2(0.), bc3(7571./16695.),
      bc4(393./640.), bc5(-92097./339200.), bc6(187./2100.), bc7(1./40.);
    
    //error:
    static const double e1(b1-bc1), e2(b2-bc2), e3(b3-bc3), e4(b4-bc4),
      e5(b5-bc5), e6(b6-bc6), e7(b7-bc7);

    //Collocation points:
    static const double c2(1./5.), c3(3./10.), c4(4./5.),
      c5(8./9.), c6(1.), c7(1.);
    
    //Butcher coefficients:
    double a21(1./5.);
    double a31(3./40.), a32(9./40.);
    double a41(44./45.), a42(-56./15.), a43(32./9.);
    double a51(19372./6561.), a52(-25360./2187.), a53(64448./6561.), a54(-212./729.);
    double a61(9017./3168.), a62(-355./33.), a63(46732./5247.), a64(49./176.), a65(-5103./18656.);
    double a71(35./384.), a72(0.), a73(500./1113.), a74(125./192.), a75(-2187./6784.), a76(11./84.);
    
    bool stepAccepted(false);
    //std::clog << "Step at time: " << t << ". ";
    do{
      std::clog << "Trying for dt: " << dt << std::endl;
      k1=(dt*f(t       , y));
      k2=(dt*f(t+c2*dt , y + a21*k1));
      k3=(dt*f(t+c3*dt , y + a31*k1 + a32*k2));
      k4=(dt*f(t+c4*dt , y + a41*k1 + a42*k2 + a43*k3));
      k5=(dt*f(t+c5*dt , y + a51*k1 + a52*k2 + a53*k3 + a54*k4));
      k6=(dt*f(t+c6*dt , y + a61*k1 + a62*k2 + a63*k3 + a64*k4 + a65*k5));
      k7=(dt*f(t+c7*dt , y + a71*k1 + a72*k2 + a73*k3 + a74*k4 + a75*k5 + a76*k6));      

      state yn(y + b1*k1 + b2*k2 + b3*k3 + b4*k4 + b5*k5 + b6*k6 + b7*k7);
      
      state delta(std::abs(e1*k1 + e2*k2 + e3*k3 + e4*k4 + e5*k5 + e6*k6 + e7*k7));
      state scale(atol + rtol*max_(std::abs(yn), std::abs(y)));
      double err(std::sqrt(1./yn.size()*(delta*delta/(scale*scale)).sum()));
      //std::clog << "y: "  << yn << ", delta: " << delta << ", err: " << err << std::endl;

      std::clog << "Ideal new stepsize, dt: " << dt*std::pow(1./std::abs(err),1./5.) << ". " <<std::endl;
      if(err>1.)
	{
	  if(dt*std::pow(1./std::abs(err),1./5.)<dt/2.)
	    {
	      if(dt > 1.e-5)
		{
		  //std::clog << "Down stepping" << std::endl;
		  dt /= 2.;
		}
	      else 
		{
		  t+=dt;
		  y = yn;
		  stepAccepted=true;
		}
	    }
	  else
	    {
	      t+=dt;
	      y = yn;
	      stepAccepted=true;
	    }
	}
      if(err<=1.)
	{
	  if(dt*std::pow(1./std::abs(err),1./5.)>dt*2.)
	    {
	      //std::clog << "Up stepping" << std::endl;
	      dt *= 2.;
	    }
	  else
	    {
	      t += dt;
	      y = yn;
	      stepAccepted=true;
	    }
	}
    } while(!stepAccepted);
    std::cout << dt << " ";
  }
};



template<typename Problem>
class LobattoIIIa
{
  const Problem& f;
  
  Matrix<double,0> eval(double t, const state& y, double dt, const Matrix<double,0>& x) const
  {
    unsigned int size(x.getRows()/3);
    state k1(y.size()), k2(y.size()), k3(y.size());
    state d1(y.size()), d2(y.size()), d3(y.size());
    
    for(unsigned int j(0);j<size;++j)
      {
	k1[j] = x(j,0);
	k2[j] = x(j+size,0);
	k3[j] = x(j+2*size,0);
      }

    d1=f(t,y)-k1;
    d2=f(t+0.5*dt, y+dt*(5./24.*k1+1./3.*k2-1./24.*k3))-k2;
    d3=f(t+dt, y+dt*(1./6.*k1+2./3.*k2+1./6.*k3))-k3;
    
    Matrix<double,0> r(3*size,1);
    
    for(unsigned int j(0);j<size;++j)
      {
	r(j,0) = d1[j];
	r(j+size,0) = d2[j];
	r(j+2*size,0) = d3[j];
      }
    return r;
  }

  Matrix<double,0> evalJacobian(double t,
				const state& y,
				double dt,
				const Matrix<double,0>& x) const
  {
    double delta(1e-6);
    Matrix<double,0> Jacobian(x.getRows(), x.getRows());

    Matrix<double,0> z(eval(t,y,dt,x));
    for(unsigned int i(0);i<x.getRows();++i)
      {
        Matrix<double,0> dx(x);
	dx(i,0) += delta;
	Matrix<double,0> dz(eval(t,y,dt,dx));
	
	for(unsigned int k(0);k<x.getRows();++k)
	  {
	    Jacobian(k,i) = (dz(k,0)-z(k,0))/delta;
	  }
      }
    return Jacobian;
  }
  
  static double ninf(const Matrix<double,0>& x)
  {
    double s(0.);
    for(unsigned int k(0);k<x.getRows();++k)
      s += std::abs(x(k,0));
    return s;
  }

  void SolveForks(double t, const state& y, double dt, state& k1, state& k2, state& k3) const
  {
    Matrix<double,0> x(3*k1.size(),1);
    for(unsigned int j(0);j<k1.size();++j)
      {
	x(j,0) = k1[j];
	x(j+k1.size(),0) = k2[j];
	x(j+2*k1.size(),0) = k3[j];
      }
    unsigned int k(10);
    double tol(1e-6);
    double delta(0.);
    do{
      //for(unsigned int s(0);s<6;++s)
      //{
	Matrix<double,0> g(eval(t,y,dt,x));
	delta = ninf(g);
	Solvelapack(evalJacobian(t,y,dt,x),g);
	for(unsigned int i(0);i<x.getRows();++i)
	  x(i,0) -= g(i, 0);
	//}
    }while((k--)> 0 && delta>tol);

    for(unsigned int j(0);j<k1.size();++j)
      {
	k1[j] = x(j,0);
	k2[j] = x(j+k1.size(),0);
	k3[j] = x(j+2*k1.size(),0);
      }
  }
public:
  LobattoIIIa(const Problem& p): f(p) {}
  
  state step(double t, const state& y, double dt) const
  {
    state k1(f(t,y)), k2(f(t,y)), k3(f(t,y));
    SolveForks(t,y,dt,k1,k2,k3);
    return y+dt*(k1+4.*k2+k3)/6.;
  }
};
