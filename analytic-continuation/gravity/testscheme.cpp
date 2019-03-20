#include <iostream>
#include "../../utility/matrix/matrix.hpp"
#include "../../utility/matrix/io.hpp"
#include "../../utility/matrix/algorithms.hpp"
#include "../coord.hpp"

class Function
{
  n_range m_r;
  int m_steps;
  int m_funcn;
  double* m_array;

  double m_Rmax;
public:
  Function(double R, int Steps, int FuncNumber): m_r(0,-1,R,Steps),
							  m_steps(Steps),
							  m_funcn(FuncNumber),
							  m_array(NULL),
							  m_Rmax(R)
  {m_array = new double[m_funcn*(m_steps+2)];}
  
  Function(const Function& f): m_r(f.m_r),
			       m_steps(f.m_steps),
			       m_funcn(f.m_funcn),
			       m_array(NULL),
			       m_Rmax(f.m_Rmax)
  {
    m_array = new double[m_funcn*(m_steps+2)];
    for(int i(0);i<m_funcn*(m_steps+2);++i)
      m_array[i] = f.m_array[i];
  }
  ~Function()
  { delete [] m_array; }
  
  Function& operator=(const Function& f)
  {
    if(m_steps != f.m_steps || m_funcn != f.m_funcn)
      {
	m_steps = f.m_steps;
	delete [] m_array;
	m_array = new double[m_funcn*(m_steps+2)];
      }
    m_r = f.m_r;
    m_Rmax = f.m_Rmax;
    for(int i(0);i<m_funcn*(m_steps+2);++i)
      m_array[i] = f.m_array[i];

    return *this;
  }

  double r(int i) const
  { return m_r[i]; }

  const double& func(int FuncIndex, int PointIndex) const
  {return m_array[FuncIndex*(m_steps+2)+PointIndex+1];}
  double& func(int FuncIndex, int PointIndex)
  {return m_array[FuncIndex*(m_steps+2)+PointIndex+1];}
 
  void setMin(int FuncIndex, double x){m_array[FuncIndex*(m_steps+2)] = x;}
  void setMax(int FuncIndex, double x){m_array[FuncIndex*(m_steps+2)+m_steps+1] = x;}

  double h() const { return m_r.h(); }

  int size() const { return m_steps; }
  int funcs() const { return m_funcn; }
};

class HeatEquations
{
  static double N(double r0, double sigma, double r){return std::exp(-(r-r0)*(r-r0)/(2.*sigma*sigma));}
public:
  HeatEquations(){}

  Function eval(const Function& g) const
  {
    const double hsinv(1./(g.h()*g.h()));
    Function ret(g);
    
    for(int k(0);k<(int)g.size();++k)
      {
	const double &h(g.h());
	
	const double &r(g.r(k)),
	  &A(g.func(0,k)), &B(g.func(1,k)),
	  &C(g.func(3,k)), &D(g.func(2,k));

	const double dA( (g.func(0,k+1)-g.func(0,k-1))/(2.*h) );
	const double ddA((g.func(0,k+1)-2.*g.func(0,k)+g.func(0,k-1))*hsinv);

	const double dB( (g.func(1,k+1)-g.func(1,k-1))/(2.*h) );
	const double ddB((g.func(1,k+1)-2.*g.func(1,k)+g.func(1,k-1))*hsinv);

	const double dC( (g.func(2,k+1)-g.func(2,k-1))/(2.*h) );
	const double ddC((g.func(2,k+1)-2.*g.func(2,k)+g.func(2,k-1))*hsinv);

	const double dD( (g.func(3,k+1)-g.func(3,k-1))/(2.*h) );
	const double ddD((g.func(3,k+1)-2.*g.func(3,k)+g.func(3,k-1))*hsinv);

	ret.func(0,k) = (ddA+0.*(N(0.2,0.05,r)+N(0.8,0.05,r)));
	ret.func(1,k) = (ddB+0.*(N(0.2,0.05,r)+N(0.8,0.05,r)));
	ret.func(2,k) = (ddC+N(0.2,0.05,r)+N(0.8,0.05,r));
	ret.func(3,k) = (ddD+N(0.2,0.05,r)+N(0.8,0.05,r));
      }
	
    return ret;
  }
  
  Matrix<double, 0> evalNJacobian(const Function& g) const
  {
    double dx(1e-6);
    unsigned int m_steps(g.size());
    unsigned int N(g.funcs()*m_steps);
    Matrix<double, 0> Jacobian(N,N);

    Function F(eval(g));    

    for(unsigned int k(0);k<g.funcs();++k)
      {
	for(unsigned int i(0);i<m_steps;++i)
	  {
	    Function dg(g);
	    dg.func(k,i) += dx;
	    Function dF(eval(dg));

	    for(unsigned int l(0);l<g.funcs();++l)
	      for(unsigned int j(0);j<m_steps;++j)
		Jacobian(l*m_steps+j ,k*m_steps+i) = (dF.func(l,j)-F.func(l,j))/dx;
	  }
      }
    return Jacobian;
  }
};

void print(std::ostream& flux, const Function& f)
{
  for(int i(-1);i<=(int)f.size();++i)
    {
      flux << f.r(i) << " ";
      for(int k(0);k<f.funcs();++k)
	flux << f.func(k,i) << " ";
      flux << std::endl;
    }
}

double Max(const Function& f)
{
  double max(f.func(0, 0));
  for(unsigned int k(1);k<f.funcs();++k)
    for(unsigned int i(1);i<f.size();++i)
      {if(f.func(k,i)>max)max = f.func(k,i);}

  return max;
  }

void Newton(const HeatEquations& m, Function& y)
{
  unsigned int Steps(20);
  Function tmp(y);
  for(unsigned int i(0);i<Steps;++i)
    {
      std::clog << "Step: " << i << " (max: " << Max(m.eval(y))  << ")" << std::endl;
      tmp = m.eval(y);
      Matrix<double,0> g(y.funcs()*y.size(),1);
      for(int k(0);k<y.funcs();++k)
	for(unsigned int i(0);i<y.size();++i)
	  g(k*y.size()+i,0)= tmp.func(k,i);
      
      Solve(m.evalNJacobian(y), g);

      for(unsigned int i(0);i<y.size();++i)
	for(int k(0);k<y.funcs();++k)
	  y.func(k,i) -= g(k*y.size()+i, 0);
      
      print(std::cout,y);
      std::cout << std::endl;
    }
}

int main()
{
  try{
    HeatEquations m;
    
    unsigned int Steps(100);
    double R(1.);
    
    Function y(R, Steps,4);

    y.setMin(0, 0.);
    y.setMin(1, 0.);
    y.setMin(2, 0.);
    y.setMin(3, 0.);

    y.setMin(0, 2.);
    y.setMin(1, 2.);
    y.setMin(2, -0.);
    y.setMin(3, -0.);

    for(unsigned int k(0);k<4;++k)
      for(unsigned int i(0);i<Steps;++i)
	y.func(k,i) = .5;

    Newton(m, y);    
    //print(std::cout, y);
  }
  catch(std::string& e)
    {std::cout << e << std::endl;}
  return 0;
}
