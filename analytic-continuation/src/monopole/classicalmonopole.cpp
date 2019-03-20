#include <iostream>
#include <matrix/matrix.hpp>
#include <matrix/io.hpp>
#include <matrix/algorithms.hpp>
#include <cmdline.hpp>
#include "../coord.hpp"
#include <fstream>
#include "../vortex/interpolatingfunction.hpp"

class Function
{
  n_range m_r;
  int m_steps;
  double* m_array;

  double m_Rmax;
public:
  Function(double R, int Steps): m_r(0,-1,R,Steps),
				 m_steps(Steps),
				 m_array(NULL),
				 m_Rmax(R)
  {m_array = new double[2*m_steps+4];}
  
  Function(const Function& f): m_r(f.m_r),
			       m_steps(f.m_steps),
			       m_array(NULL),
			       m_Rmax(f.m_Rmax)
  {
    m_array = new double[2*m_steps+4];
    for(unsigned int i(0);i<2*m_steps+4;++i)
      m_array[i] = f.m_array[i];
  }
  ~Function()
  { delete [] m_array; }
  
  Function& operator=(const Function& f)
  {
    if(m_steps != f.m_steps)
      {
	m_steps = f.m_steps;
	delete [] m_array;
	m_array = new double[2*m_steps+4];
      }
    m_r = f.m_r;
    m_Rmax = f.m_Rmax;
    for(unsigned int i(0);i<2*m_steps+4;++i)
      m_array[i] = f.m_array[i];

    return *this;
  }

  double r(int i) const
  { return m_r[i]; }

  const double& F(int i) const
  {return m_array[i+1];}
  const double& H(int i) const
  {return m_array[i+m_steps+3];}

  double& F(int i)
  {return m_array[i+1];}
  double& H(int i)
  {return m_array[i+m_steps+3];}
 
  void Fmin(double x){m_array[0] = x;}
  void Fmax(double x){m_array[m_steps+1] = x;}

  void Hmin(double x){m_array[m_steps+2] = x;}
  void Hmax(double x){m_array[2*m_steps+3] = x;}

  double h() const { return m_r.h(); }

  int size() const { return m_steps; }
};

class Monopole
{
  double m_lambda;
  double m_g;
  double m_v;

public:
  Monopole(double Lambda, double g, double v): m_lambda(Lambda),
					       m_g(g), m_v(v)
  {}
  
  Function eval(Function g)
  {
    const double hsinv(1./(g.h()*g.h()));
    Function ret(g);
	
    for(int k(0);k<(int)g.size();++k)
      {
	ret.F(k) = (g.F(k-1)-2.*g.F(k)+g.F(k+1))*hsinv*(g.r(k)*g.r(k))
	  -g.F(k)*(g.F(k)*g.F(k)-1.)
	  -m_g*m_v*m_v*(g.H(k)*g.H(k)*g.F(k));

	ret.H(k) = (g.H(k-1)-2.*g.H(k)+g.H(k+1))*hsinv*(g.r(k)*g.r(k))
	  -2.*g.H(k)*g.F(k)*g.F(k)
	  -m_lambda*m_v*m_v*g.H(k)*(g.H(k)*g.H(k)-g.r(k)*g.r(k));
      }
	
    return ret;
  }
  
  Matrix<double, 0> evalJacobian(Function g)
  {
    const double hsinv(1./(g.h()*g.h()));
    unsigned int m_steps(g.size());
    unsigned int N(2*m_steps);

    Matrix<double, 0> Jacobian(N,N);

    for(unsigned int i(0);i<N;++i)
      for(unsigned int j(0);j<N;++j)
	Jacobian(i,j) = 0.;
    
    for(unsigned int i(0);i<m_steps;++i)
      {
	Jacobian(i,i)   = (-m_g*m_v*m_v*g.H(i)*g.H(i)/(g.r(i)*g.r(i))
			   -(3.*g.F(i)*g.F(i)-1.)/(g.r(i)*g.r(i))
			   -2.*hsinv);	
	Jacobian(i,m_steps+i) = -2.*m_g*m_v*m_v*g.H(i)*g.F(i)/(g.r(i)*g.r(i));
      }
    for(unsigned int i(1);i<m_steps;++i)
      {
	Jacobian(i-1,i)   = hsinv;
	Jacobian(i,  i-1) = hsinv;
      }

    for(unsigned int i(0);i<m_steps;++i)
      {
	Jacobian(m_steps+i,m_steps+i) = -(m_lambda*m_v*m_v *(3.*g.H(i)*g.H(i)-g.r(i)*g.r(i))/(g.r(i)*g.r(i))
					  +g.F(i)*g.F(i)/(g.r(i)*g.r(i))
					  +2.*hsinv);
	Jacobian(m_steps+i,i) = -(2.*g.H(i)*g.F(i)/(g.r(i)*g.r(i)));
      }
    for(unsigned int i(1);i<m_steps;++i)
      {
	Jacobian(m_steps+i-1,m_steps+i) = hsinv;
	Jacobian(m_steps+i,  m_steps+i-1) = hsinv;
      }

    return Jacobian;
  }
  Matrix<double, 0> evalNJacobian(Function g)
  {
    double dx(1e-6);
    unsigned int m_steps(g.size());
    unsigned int N(2*m_steps);
    Matrix<double, 0> Jacobian(N,N);


    Function F(eval(g));    
    for(unsigned int i(0);i<m_steps;++i)
      {
	Function dg(g);
	dg.F(i) += dx;
	Function dF(eval(dg));
	
	for(unsigned int k(0);k<m_steps;++k)
	  {
	    Jacobian(k,i) = (dF.F(k)-F.F(k))/dx;
	    Jacobian(m_steps+k,i) = (dF.H(k)-F.H(k))/dx;
	  }
      }

    for(unsigned int i(0);i<m_steps;++i)
      {
	Function dg(g);
	dg.H(i) += dx;
	Function dF(eval(dg));
	
	for(unsigned int k(0);k<m_steps;++k)
	  {
	    Jacobian(k,m_steps+i) = (dF.F(k)-F.F(k))/dx;
	    Jacobian(m_steps+k,m_steps+i) = (dF.H(k)-F.H(k))/dx;
	  }
      }
    return Jacobian;
  }
};

void print(std::ostream& flux, const Function& f)
{
  for(int i(-1);i<=(int)f.size();++i)
    flux << f.r(i) << " " << f.F(i) << " " << f.H(i) << std::endl;
}

double Max(const Function& f)
{
  double max(std::abs(f.F(0)));
  for(unsigned int i(1);i<f.size();++i)
    {if(f.F(i)>max)max = std::abs(f.F(i));}

  for(unsigned int i(0);i<f.size();++i)
    {if(f.H(i)>max)max = std::abs(f.H(i));}
  return max;
}

void Newton(Monopole& m, Function& y)
{
  unsigned int Steps(15);
  Function tmp(y);

  unsigned int k(0);
  unsigned int maxIter(15);
  double delta(0.);
  do{
    tmp = m.eval(y);

    Matrix<double,0> g(2*y.size(),1);
    for(unsigned int i(0);i<y.size();++i)
      {
	g(i,0)= tmp.F(i);
	g(i+y.size(),0)= tmp.H(i);
      }
    Solvelapack(m.evalNJacobian(y), g);
    delta = normInf(g);
    for(unsigned int i(0);i<y.size();++i)
      {
	y.F(i) -= g(i, 0);
	y.H(i) -= g(y.size()+i,0);
      }
    std::clog << "Step: " << k << " (delta: " << delta <<  ", max: " << Max(m.eval(y))  << ")" << std::endl;

  }while((k++)<maxIter && delta>1e-10);
}

void buildBSPLimit(Function& f)
{
  f.F(-1) = 1.;
  f.H(-1) = 0.;
  for(int i(0);i<=(int)f.size();++i)
    {
      f.F(i) = f.r(i)/std::sinh(f.r(i));
      f.H(i) = f.r(i)/std::tanh(f.r(i))-1.;
    }
}

void loadFile(const std::string& filename,
	      InterpolatingFunction& f, 
	      InterpolatingFunction& h)
{
  std::ifstream file(filename.c_str(), std::ios::in);
  double r(0.), x(0.), y(0.);
  
  do{
    file >> r >> x >> y;
    if(file)
      f.addPoint(r,x);
      h.addPoint(r,y);
  }while(file);
  return;
}
  
int main(int argc, char** argv)
{
  CmdLine cmd(argc,argv);

  try{
    Monopole m(cmd.Get<double>("lambda")[0],1.,1.);
    
    unsigned int Steps(cmd.Get<unsigned int>("steps")[0]);
    double R(cmd.Get<double>("r")[0]);
    
    Function y(R, Steps);
    y.Fmin(1.);
    y.Fmax(0.);
    y.Hmin(0.);
    y.Hmax(R);

    std::string source(cmd.Get<std::string>("source")[0]);
    if(source == "file")
      {
	std::string filename(cmd.Get<std::string>("sourcefile")[0]);
	InterpolatingFunction f;
	InterpolatingFunction h;
	loadFile(filename, f, h);

	for(unsigned int k(0);k<Steps;++k)
	  {
	    double r(k*R/(Steps-1));
	    y.F(k) = f(r);
	    y.H(k) = h(r);
	  }
      }
    else if(source == "gen")
      {
	for(unsigned int k(0);k<Steps;++k)
	  {
	    y.F(k) = 1.-y.r(k);
	    y.H(k) = y.r(k);
	  }
      }
    else
      throw std::string("Source parameter should be either file or gen.");

    Newton(m, y);
    print(std::cout, y);
  }
  catch(std::string& e)
    {std::clog << e << std::endl;}
  return 0;
}
