#include "numerics/matrix.hpp"
#include "numerics/matrix/algorithms.hpp"
#include "utility/cmdline.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

class MyFunction
{
  Matrix<double> m_r;
  double m_R;
  unsigned int m_N;

  double m_alpha;
  double m_beta;

  double L(double r, double x, double y)
  { return -x*(x-1.)*(x-2.) + m_alpha*r*r*y*y*(1.-x); }

  double L_x(double r, double x, double y)
  { return -2.+6.*x-3.*x*x-m_alpha*r*r*y*y; }

  double L_y(double r, double x, double y)
  { return 2.*m_alpha*r*r*y*(1.-x); }

  double M(double r, double x, double y)
  { return 2.*y*(x-1.)+2.*x*y*(1.-x) + m_beta*r*r*y*(1.-y*y); }

  double M_x(double r, double x, double y)
  { return 4.*(1.-x)*y; }

  double M_y(double r, double x, double y)
  { return 2.*(x-1.)*(1.-x)+m_beta*r*r*(1.-y*y)-2.*m_beta*r*r*y*y; }
public:
  MyFunction(unsigned int N, double R,
	     double alpha,
	     double beta):m_N(N/2),
			  m_r(N/2, 1), 
			  m_alpha(alpha),
			  m_beta(beta),
			  m_R(R)
  {
    if(N%2 != 0)
      throw std::string("MyFunction::MyFunction()"
			" - The number of point must be even. Aborting.");

    for(unsigned int i(1);i<=m_N;++i)
      m_r(i) = i*R/(m_N+1.);
  }
  void Evaluate(Matrix<double>& b, const Matrix<double>& x)
  {
    if(x.getRows() != 2*m_N || b.getRows() != 2*m_N
       || x.getColumns() != 1 || b.getRows() != 2*m_N)
      throw std::string("MyFunction::Evaluate() - Dimensions doesn't match.");

    double h(m_R/(m_N+1.));

    b(1) = (m_r(1)*m_r(1)/(h*h)*(x(2)-2.*x(1))
	    + L(m_r(1),x(1),x(m_N+1)));
   
    b(m_N) = (m_r(m_N)*m_r(m_N)/(h*h)*(1.-2.*x(m_N)+x(m_N-1))
	    + L(m_r(m_N),x(m_N),x(2*m_N)));

    b(m_N+1) = (m_r(1)*m_r(1)/(h*h)*(x(m_N+2)-2.*x(m_N+1))
	      + m_r(1)/h*x(m_N+2)
	      + M(m_r(1),x(1),x(m_N+1)));

    b(2*m_N) = (m_r(m_N)*m_r(m_N)/(h*h)*(1.-2.*x(2*m_N) + x(2*m_N-1))
	      + m_r(m_N)/h*(1.-x(2*m_N-1))
	      + M(m_r(m_N), x(m_N), x(2*m_N)));

    for(unsigned int i(2);i<=m_N-1;++i)
      {
	b(i) = (m_r(i)*m_r(i)/(h*h)*(x(i+1)-2.*x(i)+x(i-1))
		+ L(m_r(i),x(i),x(m_N+i))); 
      }
    
    for(unsigned int i(m_N+2);i<=2*m_N-1;++i)
      {
	b(i) = (m_r(i-m_N)*m_r(i-m_N)/(h*h)*(x(i+1)-2.*x(i) + x(i-1))
		+ m_r(i-m_N)/h*(x(i+1)-x(i-1))
		+ M(m_r(i-m_N), x(i-m_N), x(i)));
      }
  }
  void EvaluateJacobian(Matrix<double>& j, const Matrix<double>& x)
  {
    if(j.getRows() != 2*m_N || j.getColumns() != 2*m_N)
      throw std::string("NewtonMethod::EvaluateJacobian()"
			" - Dimensions of j mismatch.");
    if(x.getRows() != 2*m_N || x.getColumns() != 1)
      throw std::string("NewtonMethod::EvaluateJacobian()"
			" - Dimensions of x mismatch.");

    double h(m_R/(m_N+1.));

    for(unsigned int i(1);i<=2*m_N;++i)
      for(unsigned int k(1);k<=2*m_N;++k)
	j(i,k) = 0.;
    
    // i == 0
    j(1,1) = -2.*m_r(1)*m_r(1)/(h*h)+ L_x(m_r(1), x(1), x(m_N+1));
    j(1,2) = m_r(1)*m_r(1)/(h*h);
    j(1,m_N+1) = L_y(m_r(1), x(1), x(m_N+1));

    // i == N-1
    j(m_N,m_N-1) = m_r(m_N)*m_r(m_N)/(h*h);
    j(m_N,m_N) = -2.*m_r(m_N)*m_r(m_N)/(h*h) + L_x(m_r(m_N), x(m_N), x(2*m_N));
    j(m_N-1,2*m_N-1) = L_y(m_r(1), x(m_N-1), x(2*m_N-1));

    // i == N
    j(m_N+1,1) = M_x(m_r(1), x(1), x(m_N+1));
    j(m_N+1,m_N+1) = -2.*m_r(1)*m_r(1)/(h*h) + M_y(m_r(1), x(1), x(m_N+1));
    j(m_N+1,m_N+2) = m_r(1)*m_r(1)/(h*h) + m_r(1)/h;

    // i == 2N-1
    j(2*m_N,m_N) = M_x(m_r(m_N), x(m_N), x(2*m_N));
    j(2*m_N,2*m_N-1) = m_r(m_N)*m_r(m_N)/(h*h)-m_r(m_N)/h;
    j(2*m_N,2*m_N) = -2.*m_r(m_N)*m_r(m_N)/(h*h) + M_y(m_r(m_N), x(m_N), x(2*m_N));

    // i from 1 to N-2
    for(unsigned int i(2);i<=m_N-1;++i)
      {
	j(i,i-1) = m_r(i)*m_r(i)/(h*h);
	j(i,i) = -2.*m_r(i)*m_r(i)/(h*h) + L_x(m_r(i),x(i),x(i+m_N));
	j(i,i+1) = m_r(i)*m_r(i)/(h*h);
	j(i,i+m_N) = L_y(m_r(i), x(i), x(i+m_N));
      }

    // i from N+1 to 2N-2
    for(unsigned int i(m_N+2);i<=2*m_N-1;++i)
      {
	j(i,i-1) = m_r(i-m_N)*m_r(i-m_N)/(h*h)-m_r(i-m_N)/h;
	j(i,i) = -2.*m_r(i-m_N)*m_r(i-m_N)/(h*h )+ M_y(m_r(i-m_N), x(i-m_N), x(i));
	j(i,i+1) = m_r(i-m_N)*m_r(i-m_N)/(h*h)+m_r(i-m_N)/h;
	j(i,i-m_N) = M_x(m_r(i-m_N), x(i-m_N), x(i));
      }
  }
};

template<typename Functor>
class NewtonMethod
{

  Functor m_f;
  Matrix<double> m_x0;
  Matrix<double> m_Solution; 
public:
  NewtonMethod(Functor f, const Matrix<double>& x0): m_f(f),
						     m_x0(x0),
						     m_Solution(x0)
  {}
  
  ~NewtonMethod()
  {}

  bool Iterate()
  {
    unsigned int n_max(10000);
    double tolerence(1.e-6);
    double delta(1000.);

    Matrix<double> z(m_x0);
    Matrix<double> g(m_x0);

    Matrix<double> j(m_x0.getRows(),m_x0.getRows());
    Matrix<double> lu(m_x0.getRows(),m_x0.getRows());

    for(unsigned int n(0);n<n_max && delta>=tolerence;++n)
      {
	m_f.EvaluateJacobian(j, z);
	m_f.Evaluate(g, z);

	LU(lu,j);
	InPlaceSolveFromLU(lu, g);
	RemoveFrom(z, g);
	delta = EuclidianNorm(g);
	std::cout << "delta = " << delta << std::endl;
      }

    if(delta>tolerence)
      return false;
    else
      m_Solution = z;
    return true;
    
  }
  const Matrix<double>& getSolution(){return m_Solution;}
};

inline double TheEnergyFunctional(Matrix<double>& e,  const Matrix<double>& fh, double R,
				  double g,
				  double v,
				  double lambda)
{
  double r(0);
  double N(fh.getRows()/2);
  double h(R/(fh.getRows()/2+1.));

  if(e.getRows() != fh.getRows()/2-3)
    e.Resize(fh.getRows()/2-3, 1);

  for(unsigned int i(2);i<fh.getRows()/2-1;++i)
    {
      r = i*h;
      e(i-1) = 4.*M_PI/(g*g)*(
			      (fh(i+1)-fh(i-1))/(2.*h)*(fh(i+1)-fh(i-1))/(2.*h)
			      +fh(i)*fh(i)*(fh(i)-2.)*(fh(i)-2.)/(2*r*r)
			      +v*v*g*g*r*r*((fh(i+N+1)-fh(i+N-1))/(2.*h))*((fh(i+N+1)-fh(i+N-1))/(2.*h))/2.
			      +g*g*v*v*fh(i+N)*fh(i+N)*(1.-fh(i))*(1.-fh(i))
			      +lambda/4.*g*g*v*v*v*v*r*r*(fh(i+N)*fh(i+N)-1.)*(fh(i+N)*fh(i+N)-1.)
			      );
    }  
}

double getTheTotalEnergy(const Matrix<double>& e, double h)
{
  double E(0.);
  
  for(unsigned int i(1);i<=e.getRows();++i)
    E += h*e(i);

  return E;
}

class StringBuilder
{
  std::ostringstream oss;
public:
  template<typename T>
  StringBuilder(const T& op)
  {
    oss << op;
  }
  template<typename T>
  StringBuilder& operator()(const T& op)
  {
    oss << op;
    return *this;
  }
  std::string Str(){return oss.str();}
};

double Simulation(double g, double v, double lambda, double R, unsigned int N)
{
  double TotalEnergy(0.);
  
  Matrix<double> init(N,1);
  for(unsigned int i(1);i<=N/2;++i)
    {
      init(i) =  1.;
      init(i+N/4) = 1.;
    }
  
  for(unsigned int gridlevel(0);gridlevel<7;++gridlevel)
    {
      std::cout << "Level " << gridlevel << std::endl;
      unsigned int nextN(2*N-2);
      
      
      Matrix<double> x0(nextN,1);
      x0(1) = init(1);
      x0(nextN/2) = init(N/2+1);
      
      for(unsigned int i(1);i<=N/2-1;++i)
	{
	  x0(2*i) = 0.5*(init(i)+init(i+1));
	  x0(2*i+1) = init(i+1);
	  
	  x0(2*i+N-1) = 0.5*(init(i+N/2)+init(i+1+N/2));
	  x0(2*i+1+N-1) = init(i+1+N/2);
	}
      
      N = nextN;
      
      MyFunction G(N,R, g*g*v*v, lambda*v*v);
      NewtonMethod<MyFunction> Solver(G, x0);
      if(!Solver.Iterate())
	throw std::string("Newton method didn't converge.");
      
      Matrix<double> Energy(1,1);
      TheEnergyFunctional(Energy, Solver.getSolution(), R,g,v,lambda);
      TotalEnergy = getTheTotalEnergy(Energy, R/(N/2+1));
      
      std::cout << "The energy of the solution at this level is: E = "
		<< TotalEnergy
		<< std::endl;
      init = Solver.getSolution();
      
      std::ofstream file1(StringBuilder("tmp_f_g")(g)("_v")(v)("_lambda")(lambda)(".dat").Str().c_str());
      for(unsigned int i(1);i<=init.getRows()/2;++i)
	{
	  file1 << i*R/(N/2+1) << " " << init(i) << std::endl;
	}
      std::ofstream file2(StringBuilder("tmp_h_g")(g)("_v")(v)("_lambda")(lambda)(".dat").Str().c_str());
      for(unsigned int i(init.getRows()/2+1);i<=init.getRows();++i)
	{
	  file2 << (i-init.getRows()/2)*R/(N/2+1) << " " << init(i) << std::endl;
	}
      std::ofstream file3(StringBuilder("tmp_energy_g")(g)("_v")(v)("_lambda")(lambda)(".dat").Str().c_str());
      for(unsigned int i(1);i<=Energy.getRows();++i)
	{
	  file3 << Energy(i) << std::endl;
	}
    }
  return TotalEnergy; 
}

std::ostream& operator<<(std::ostream& flux, const std::vector<double>& v)
{
  for(unsigned int i(0);i<v.size();++i)
    flux << v[i] << " ";
  return flux;
}

int main(int argc, char** argv)
{ 
  CmdLine args(argc, argv);

  std::vector<double> g(args.Get<double>("g")),
    v(args.Get<double>("v")),
    lambda(args.Get<double>("lambda"));

  unsigned int N(args.Get<unsigned int>("N")[0]);
  double R(args.Get<double>("R")[0]);

  double Energy(0.);

  std::ofstream EnergyFile("energies.dat", std::ios::out);
  EnergyFile << "#g" << '\t' << "v" << '\t' << "lambda" << '\t' << "Energy" << std::endl;

  std::ofstream logfile("log.dat", std::ios::out);
  logfile << "Integration of the spherical field equations for the monopole." << std::endl
	  << "The parameters are the following: " << std::endl
	  << "The radius is: " << R << std::endl
	  << "The initial number of points of the grid is: " << N << std::endl
	  << "g is in {" << g << "}" << std::endl
	  << "v is in {" << v << "}" << std::endl
	  << "lambda is in {" << lambda << "}" << std::endl << std::endl;
  
  logfile << "The lines belows are notifications of errors occuring "
    "during the execution of the integration:" << std::endl;

  for(unsigned int i(0);i<g.size();++i)
    for(unsigned int j(0);j<v.size();++j)
      for(unsigned int k(0);k<lambda.size();++k)
	{
	  std::cout << "Set of parameters: " << std::endl
		    << "g = " << g[i] << std::endl
		    << "v = " << v[j] << std::endl
		    << "lambda = " << lambda[k] << std::endl;
	  try{
	    Energy = Simulation(g[i],v[j],lambda[k],R,N);
	    EnergyFile << g[i] << '\t' << v[j] << '\t' << lambda[k] << '\t' << Energy << std::endl;
	    std::cout << "The Energy is " << Energy << std::endl;
	  }
	  catch(std::string& e)
	    {
	      std::cout << "Exception:" << std::endl;
	      std::cout << e << std::endl;
	      logfile << " - Exception: " << e << std::endl;
	      logfile << "    (g =  " << g[i] << ")" << std::endl
		      << "    (v =  " << v[j] << ")" << std::endl
		      << "    (lambda =  " << lambda[k] << ")" << std::endl;
	    }
	}
  
  return 0;
}
