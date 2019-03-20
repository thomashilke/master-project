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

class Gravity
{
  double m_lambda;
  double m_v;
  double m_e;
  double m_c;

public:
  Gravity(double Lambda, double v, double e, double c): m_lambda(Lambda), m_v(v),
							m_e(e), m_c(c)
  {}
  
  Function eval(const Function& g)
  {
    const double hsinv(1./(g.h()*g.h()));
    Function ret(g);
    
    for(int k(0);k<(int)g.size();++k)
      {
	const double &h(g.h());
	
	const double &r(g.r(k)),
	  &alpha(g.func(0,k)), &beta(g.func(1,k)),
	  &F(g.func(3,k)), &A(g.func(2,k));

	const double dalpha( (g.func(0,k+1)-g.func(0,k-1))/(2.*h) );
	const double ddalpha((g.func(0,k+1)-2.*g.func(0,k)+g.func(0,k-1))/(h*h));

	const double dbeta( (g.func(1,k+1)-g.func(1,k-1))/(2.*h) );
	const double ddbeta((g.func(1,k+1)-2.*g.func(1,k)+g.func(1,k-1))/(h*h));

	const double dA( (g.func(2,k+1)-g.func(2,k-1))/(2.*h) );
	const double ddA((g.func(2,k+1)-2.*g.func(2,k)+g.func(2,k-1))/(h*h));

	const double dF( (g.func(3,k+1)-g.func(3,k-1))/(2.*h) );
	const double ddF((g.func(3,k+1)-2.*g.func(3,k)+g.func(3,k-1))/(h*h));

	/*
	//Check:
	ret.func(0,k) = g.func(0,k)+g.func(1,k)+g.func(2,k)+g.func(3,k);
	ret.func(1,k) = g.func(0,k)+g.func(1,k)+g.func(2,k)+g.func(3,k);
	ret.func(2,k) = g.func(0,k)+g.func(1,k)+g.func(2,k)+g.func(3,k);
	ret.func(3,k) = g.func(0,k)+g.func(1,k)+g.func(2,k)+g.func(3,k);*/

	//First try:
	/*ret.func(0,k) = (
			 alpha*beta*(2*F*F*(A-1)*(A-1) -m_lambda*m_v*m_v*r*r*(F*F-1)*(F*F-1))
			 +beta*dA*dA/(m_v*m_v*m_e*m_e)
			 -2*r*beta*dF*(4*F+r*dF)
			 -2*r*dbeta*(m_c/(m_v*m_v)+F*F+2*r*F*dF)
			 );
	ret.func(1,k) = (
			 8*r*alpha*F*(dF+r*ddF)
			 -2*r*dalpha*(m_c/(m_v*m_v)+F*F+2*r*dF*F)
			 +m_lambda*m_v*m_v*r*r*alpha*alpha*(1-F*F)*(1-F*F)
			 -2*alpha*alpha*F*F*(1-A)*(1-A)
			 +6*r*r*alpha*dF*dF
			 +alpha*dA*dA/(m_e*m_e*m_v*m_v)
			 );
	ret.func(2,k) = (
			 4*m_v*m_v*m_e*m_e*r*alpha*alpha*beta*F*F*(1-A)
			 +dA*beta*(2*alpha+r*dalpha)
			 -r*alpha*(dA*dbeta+2*beta*ddA)
			 );
	ret.func(3,k) = (
			 -2*alpha*alpha*beta*beta*F*(1-A)*(1-A)
			 +2*m_lambda*m_v*m_v*r*r*alpha*alpha*beta*beta*F*(F*F-1)
			 -r*r*F*alpha*dbeta*dbeta
			 +r*beta*(2*F+r*dF)*(alpha*dbeta-dalpha*beta)
			 +r*beta*(2*alpha*beta*(dF+r*ddF)+r*F*(2*alpha*ddbeta-dalpha*dbeta))
			 );*/
	
	ret.func(0,k) = (
			 beta*(beta*(dalpha*dalpha-m_e*m_e*m_v*m_v*((r*r*m_v*m_v*m_lambda-2.*(1.+r*r*m_v*m_v*m_lambda+(-2.+A)*A)*F*F+r*r*m_v*m_v*m_lambda*F*F*F*F)*alpha+2.*r*dF*(4.*F+r*dF))) - 2.*m_e*m_e*r*(m_c+m_v*m_v*F*(F+2.*r*dF))*dbeta)
			 );
	ret.func(1,k) = (
			 -m_e*m_e*m_v*m_v*(r*r*m_v*m_v*m_lambda-2.*(1.+r*r*m_v*m_v*m_lambda+(-2+A)*A)*F*F+r*r*m_v*m_v*m_lambda*F*F*F*F)*alpha*alpha+2*m_e*m_e*r*(m_c+m_v*m_v*F*(F+2*r*dF))*dalpha-alpha*(dalpha*dalpha+2*m_e*m_e*r*m_v*m_v*(3*r*dF*dF+4*F*(dF*r*ddF)))
			 );
	ret.func(2,k) = (
			 4*m_e*m_e*r*m_v*m_v*(-1+A)*F*F*alpha*alpha*beta+dA*(-beta*(2*alpha+r*dalpha)+r*alpha*dbeta)+2*r*alpha*beta*ddA
			 );
	ret.func(3,k) = (
			 -2*r*r*m_v*m_v*m_lambda*F*F*F*alpha*alpha*beta*beta+r*beta*(dF*(r*beta*dalpha-alpha*(2*beta+r*dbeta))-2*r*alpha*beta*ddF)+F*(2*(1+r*r*m_v*m_v*m_lambda+(-2+A)*A)*alpha*alpha*beta*beta+r*beta*dalpha*(2*beta+r*dbeta)+r*alpha*(r*dbeta*dbeta-2*beta*(dbeta+r*ddbeta)))
			 );
      }
	
    return ret;
  }
  
  Matrix<double, 0> evalJacobian(const Function& g)
  {
    throw 0;
  }
  Matrix<double, 0> evalNJacobian(const Function& g)
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

void Newton(Gravity& m, Function& y)
{
  unsigned int Steps(40);
  Function tmp(y);
  for(unsigned int i(0);i<Steps;++i)
    {
      std::clog << "Step: " << i << " (max: " << Max(m.eval(y))  << ")" << std::endl;
      tmp = m.eval(y);
      Matrix<double,0> g(y.funcs()*y.size(),1);
      for(int k(0);k<y.funcs();++k)
	for(unsigned int i(0);i<y.size();++i)
	  g(k*y.size()+i,0)= tmp.func(k,i);
      
      Matrix<double,0> sol(g);
      Matrix<double,0> sol2(g);
      //std::cout << m.evalNJacobian(y) << std::endl;
      Solve(m.evalNJacobian(y), g);
      //std::cout<<sol<<std::endl;
      //Mult(sol2,m.evalNJacobian(y), g);
      //std::cout<< sol2<<std::endl;

      for(unsigned int i(0);i<y.size();++i)
	for(int k(0);k<y.funcs();++k)
	  y.func(k,i) -= g(k*y.size()+i, 0);
      
    }
  print(std::cout,y);
}

/*void buildBSPLimit(Function& f)
{
  f.F(-1) = 1.;
  f.H(-1) = 0.;
  for(int i(0);i<=(int)f.size();++i)
    {
      f.F(i) = f.r(i)/std::sinh(f.r(i));
      f.H(i) = f.r(i)/std::tanh(f.r(i))-1.;
    }
    }*/


  
int main()
{
  try{
    Gravity m(0.,1.,1.,1.);
    
    unsigned int Steps(20);
    double R(10.);
    
    Function y(R, Steps,4);

    y.setMin(0, 1.);
    y.setMin(1, 1.);
    y.setMin(2, 0.);
    y.setMin(3, 0.);

    y.setMin(0, 1.);
    y.setMin(1, 1.);
    y.setMin(2, 1.);
    y.setMin(3, 1.);

    for(unsigned int k(0);k<4;++k)
      for(unsigned int i(0);i<Steps;++i)
	y.func(k,i) = .5;
    //print(std::cout,y);
    //print(std::cout, m.eval(y));
    Newton(m, y);
    
    //print(std::cout, m.eval(y));
    print(std::cout, y);
  }
  catch(std::string& e)
    {std::cout << e << std::endl;}
  return 0;
}
