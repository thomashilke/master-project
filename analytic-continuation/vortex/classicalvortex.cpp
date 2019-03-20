#include <iostream>
#include <fstream>
#include "../../utility/matrix/matrix.hpp"
#include "../../utility/matrix/io.hpp"
#include "../../utility/matrix/algorithms.hpp"
#include "../../utility/cmdline.hpp"
#include "../coord.hpp"

#include "interpolatingfunction.hpp"

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

  const double& A(int i) const
  {return m_array[i+1];}
  const double& F(int i) const
  {return m_array[i+m_steps+3];}

  double& A(int i)
  {return m_array[i+1];}
  double& F(int i)
  {return m_array[i+m_steps+3];}
 
  void Amin(double x){m_array[0] = x;}
  void Amax(double x){m_array[m_steps+1] = x;}

  void Fmin(double x){m_array[m_steps+2] = x;}
  void Fmax(double x){m_array[2*m_steps+3] = x;}

  double h() const { return m_r.h(); }

  int size() const { return m_steps; }
};

class Vortex
{
  double m_lambda;
  double m_e;
  double m_v;

public:
  Vortex(double e, double v, double Lambda): m_lambda(Lambda),
					     m_e(e), m_v(v)
  {}
  
  Function eval(Function g)
  {
    const double hsinv(1./(g.h()*g.h()));
    Function ret(g);
	
    for(int k(0);k<(int)g.size();++k)
      {
	ret.A(k) = -((g.A(k-1)-2.*g.A(k)+g.A(k+1))*hsinv/g.r(k)
		     -(g.A(k+1)-g.A(k-1))/(2.*g.h()*g.r(k)*g.r(k)))
	  -2.*m_e*m_e*m_v*m_v*g.F(k)*g.F(k)*(1.-g.A(k))/g.r(k);

	ret.F(k) = -((g.F(k+1)-g.F(k-1))/(2.*g.h())
		     +g.r(k)*(g.F(k-1)-2.*g.F(k)+g.F(k+1))*hsinv)
	  +m_lambda*m_v*m_v*g.r(k)*g.F(k)*(g.F(k)*g.F(k)-1.)
	  +g.F(k)*(1.-g.A(k))*(1.-g.A(k))/g.r(k);
      }
	
    return ret;
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
	dg.A(i) += dx;
	Function dF(eval(dg));
	
	for(unsigned int k(0);k<m_steps;++k)
	  {
	    Jacobian(k,i) = (dF.A(k)-F.A(k))/dx;
	    Jacobian(m_steps+k,i) = (dF.F(k)-F.F(k))/dx;
	  }
      }

    for(unsigned int i(0);i<m_steps;++i)
      {
	Function dg(g);
	dg.F(i) += dx;
	Function dF(eval(dg));
	
	for(unsigned int k(0);k<m_steps;++k)
	  {
	    Jacobian(k,m_steps+i) = (dF.A(k)-F.A(k))/dx;
	    Jacobian(m_steps+k,m_steps+i) = (dF.F(k)-F.F(k))/dx;
	  }
      }
    return Jacobian;
  }
};

void print(std::ostream& flux, const Function& f)
{
  for(int i(-1);i<=(int)f.size();++i)
    flux << f.r(i) << " " << f.A(i) << " " << f.F(i) << std::endl;
}

double Max(const Function& f)
{
  double max(f.A(0));
  for(unsigned int i(1);i<f.size();++i)
    {if(f.A(i)>max)max = f.A(i);}

  for(unsigned int i(0);i<f.size();++i)
    {if(f.F(i)>max)max = f.F(i);}
  return max;
}

void Newton(Vortex& m, Function& y)
{
  unsigned int Steps(15);
  Function tmp(y);
  for(unsigned int i(0);i<Steps && Max(m.eval(y))>1e-10;++i)
    {
      std::clog << "Step: " << i << " (max: " << Max(m.eval(y))  << ")" << std::endl;
      tmp = m.eval(y);
      Matrix<double,0> g(2*y.size(),1);
      for(unsigned int i(0);i<y.size();++i)
	{
	  g(i,0)= tmp.A(i);
	  g(i+y.size(),0)= tmp.F(i);
	}
      Solvelapack(m.evalNJacobian(y), g);
      for(unsigned int i(0);i<y.size();++i)
	{
	  y.A(i) -= g(i, 0);
	  y.F(i) -= g(y.size()+i,0);
	}
    }
}

void loadFile(const std::string& filename,
	      InterpolatingFunction& a, 
	      InterpolatingFunction& f)
{
  std::ifstream file(filename.c_str(), std::ios::in);
  double r(0.), x(0.), y(0.);
  
  do{
    file >> r >> x >> y;
    if(file)
      a.addPoint(r,x);
      f.addPoint(r,y);
  }while(file);
  return;
}

int main(int argc, char** argv)
{
  CmdLine cmd(argc,argv);

  try{
    Vortex m(1.,1.,cmd.Get<double>("lambda")[0]);
    
    unsigned int Steps(cmd.Get<unsigned int>("steps")[0]);
    double R(cmd.Get<double>("r")[0]);
    
    Function y(R, Steps);
    y.Amin(0.);
    y.Amax(1.);
    y.Fmin(0.);
    y.Fmax(1.);
    
    std::string source(cmd.Get<std::string>("source")[0]);
    if(source == "file")
      {
	std::string filename(cmd.Get<std::string>("sourcefile")[0]);
	InterpolatingFunction a;
	InterpolatingFunction f;
	loadFile(filename, a, f);
	
	for(unsigned int k(0);k<Steps;++k)
	  {
	    double r(k*R/(Steps-1));
	    y.A(k) = a(r);
	    y.F(k) = f(r);
	  }	
      }
    else if(source == "gen")
      {
	for(unsigned int k(0);k<Steps;++k)
	  {
	    y.A(k) = 1.-y.r(k);
	    y.F(k) = y.r(k);
	  }
      }
    else
      throw std::string("Source parameter should be either file or gen.");

    Newton(m, y);
    print(std::cout, y);
  }
  catch(std::string& e)
    {std::cout << e << std::endl;}
  return 0;
}
