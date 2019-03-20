#ifndef SHOOTING
#define SHOOTING

#include <iostream>
#include <sstream>
#include <fstream>

#include <matrix/matrix.hpp>
#include <matrix/algorithms.hpp>

#include <threadpool.hpp>
#include <tuple.hpp>

class FileGen
{
  std::ofstream file;
  std::string name;
  unsigned int number;
  
  void openNext()
  {
    file.close();
    std::ostringstream oss;
    oss << name << number << ".dat";
    file.open(oss.str().c_str(), std::ios::out);
    ++number;
  }
public:
  FileGen(const std::string& name): name(name), number(0)
  {openNext();}
  
  std::ofstream& f(){return file;}
  void next(){openNext();}
};

class BoundaryConditions
{
public:
  virtual state lowerPoint(const Matrix<double,0>& x) const = 0;
  virtual Matrix<double,0> higherPoint(const state& x) const = 0;
  
  virtual double a() const = 0;
  virtual double b() const = 0;

  virtual unsigned int size() const = 0;
};

template<typename Problem>
class ShootFunction
{
  LobattoIIIa<Problem> rk4;
  BoundaryConditions& bc;
  
  unsigned int N;

  void ShootNPrint(state y, std::ostream& flux) const
  {
    double t(bc.a());
    double dt((bc.b()-bc.a())/(double)N);
    for(unsigned int k(0); k<=N; ++k,t += dt)
      {
	flux << t << " " << y << std::endl;
	y=rk4.step(t,y,dt);
      }
  }
public:
  ShootFunction(Problem& p, 
		BoundaryConditions& bc_): rk4(p),
					  bc(bc_),
					  N(1000){}
  
  Matrix<double,0> operator()(const Matrix<double,0>& x) const
  {return eval(x);}

  Matrix<double,0> eval(const Matrix<double,0>& x) const
  {
    double t(bc.a());
    double dt((bc.b()-bc.a())/(double)N);

    state y(bc.lowerPoint(x));

    for(unsigned int k(0);k<N;++k, t+=dt)
      y = rk4.step(t,y,dt);
    
    return bc.higherPoint(y);
  }

  void integrate(const Matrix<double,0>& x, std::ostream& flux) const
  {
    state y(bc.lowerPoint(x));
    ShootNPrint(y,flux);
  }
};

template<typename Problem, typename Stepper>
class MultipleShootFunction
{
  const Problem& f;
  const BoundaryConditions& bc;
  unsigned int NIntervals;
  unsigned int NSteps;
  
  state Shoot(unsigned int n, state y) const
  {
    Stepper rk4(f);
    double t(bc.a()+n*(bc.b()-bc.a())/(double)NIntervals);
    double dt((bc.b()-bc.a())/(double)(NIntervals*NSteps));
    for(unsigned int k(0); k<NSteps; ++k,t += dt)
      y=rk4.step(t,y,dt);
    return y;
  }

  void ShootNPrint(unsigned int n, state y, std::ostream& flux) const
  {
    Stepper rk4(f);
    double t(bc.a()+n*(bc.b()-bc.a())/(double)NIntervals);
    double dt((bc.b()-bc.a())/(double)(NIntervals*NSteps));
    for(unsigned int k(0); k<NSteps; ++k,t += dt)
      {
	flux << t << " " << y << std::endl;
	y=rk4.step(t,y,dt);
      }

  }
public:
  MultipleShootFunction(const Problem& p,
			const BoundaryConditions& bc_,
			unsigned int N): f(p), bc(bc_),NIntervals(N), NSteps(200)
  {}
  Matrix<double,0> operator()(const Matrix<double,0>& x) const
  {return eval(x);}
  Matrix<double,0> eval(const Matrix<double,0>& x) const
  {
    Matrix<double,0> res(x.getRows(),1);

    std::vector<state> ys(NIntervals,state(f.size()));
    ys[0] = bc.lowerPoint(x);
    ys[0] = Shoot(0,ys[0]);
    for(unsigned int n(1);n<NIntervals;++n)
      {
	for(unsigned int i(0); i<f.size(); ++i)
	  ys[n][i] = x(f.size()*(n-1)+bc.size()+i,0);
	
	ys[n] = Shoot(n, ys[n]);
      }

    Matrix<double,0> hbc(bc.higherPoint(ys[NIntervals-1]));
    for(unsigned int k(0);k<bc.size();++k)
      res(k,0) = hbc(k,0);
    
    for(unsigned int n(0);n<NIntervals-1;++n)
      for(unsigned int i(0); i<f.size(); ++i)
	res(f.size()*n+bc.size()+i,0) = ys[n][i]-x(f.size()*(n)+bc.size()+i,0);

    return res;
  }
  
  void integrate(const Matrix<double,0>& x, std::ostream& flux) const
  {
    std::vector<state> ys(NIntervals,state(f.size()));
    ys[0] = bc.lowerPoint(x);
    ShootNPrint(0,ys[0],flux);
    flux << std::endl;
    for(unsigned int n(1);n<NIntervals;++n)
      {
	for(unsigned int i(0); i<f.size(); ++i)
	  ys[n][i] = x(f.size()*(n-1)+bc.size()+i,0);
	
        ShootNPrint(n, ys[n], flux);
	flux << std::endl;
      }
  }
  unsigned int size(){return f.size()*(NIntervals-1)+bc.size();}
};

class JacobianBuilder: public ResultAccountant<Tuple<unsigned int, Matrix<double,0> > >
{
  Matrix<double,0>& j;
  const Matrix<double,0>& z;
public:
  JacobianBuilder(const Matrix<double,0>& z, Matrix<double,0>& j): j(j), z(z)
  {}
  void Process(const Tuple<unsigned int, Matrix<double,0> >& ret)
  {
    double delta(1e-6);

    const unsigned int& id(val<1>(ret));
    const Matrix<double,0>& dz(val<2>(ret));
    
    for(unsigned int k(0);k<dz.getRows();++k)
      j(k,id) = (dz(k,0)-z(k,0))/delta;
  }
};

template<typename Function>
Tuple<unsigned int, Matrix<double,0> > JacobianColumnJob( Function f, Matrix<double,0> x, unsigned int i)
{
  double delta(1e-6);
  x(i,0) += delta;
  return MakeTuple(i,f(x));
}

template<typename Function>
Matrix<double,0> ParallelJacobian(const Function& f, const Matrix<double,0>& x, unsigned int NJobs)
{  
  Matrix<double,0> z(f(x));
  Matrix<double,0> jacobian(x.getRows(), x.getRows());
  
  JacobianBuilder jb(z,jacobian);
  ThreadPool<Tuple<unsigned int, Matrix<double,0> > > tp(NJobs);
  tp.setResultAccountant(&jb);

  for(unsigned int k(0);k<x.getRows();++k)
    tp.enqueue(MakeJob(JacobianColumnJob<Function>, f, x, k));
  tp.waitForJobsDone();

  return jacobian;
}


template<typename Function>
Matrix<double,0> Jacobian(const Function& f, const Matrix<double,0>& x)
{
  double delta(1e-6);
  
  Matrix<double,0> z(f(x));
  Matrix<double,0> jacobian(x.getRows(), x.getRows());
  
  for(unsigned int k(0);k<x.getRows();++k)
    {
      Matrix<double,0> dx(x);
      dx(k,0) += delta;
      Matrix<double,0> dz(f(dx));
      
      for(unsigned int j(0);j<x.getRows();++j)
	jacobian(j,k) = (dz(j,0)-z(j,0))/delta;
    }
  return jacobian;
}

template<typename Function>
void Newton(const Function& f, Matrix<double,0>& x)
{
  int MaxIter(1000);

  int k(1);
  double delta(0.);
  double tol(1e-10);

  do{
    Matrix<double,0> g(f(x));    
    Solvelapack(Jacobian(f,x),g);
    delta = normInf(g);
    if(delta>1.)
      for(unsigned int j(0);j<x.getRows();++j)
	x(j,0) -= g(j,0)*(1./(1.+std::sqrt(delta)));
    else
      for(unsigned int j(0);j<x.getRows();++j)
	  x(j,0) -= g(j,0);
    std::clog << "Newton iter. " << k << " delta: " << delta;
    std::clog << ", |g(x)| = " << normInf(f(x)) << std::endl;
  } while((k++)<MaxIter && delta>tol);
}

template<typename Function>
void ParallelMNewton(const Function& f, Matrix<double,0>& x, unsigned int NJobs = 2)
{
  unsigned int MaxIter(1000);
  
  int k(1);
  double delta(0.);
  double tol(1e-10);

  do{
    Matrix<double,0> g(f(x));
    Solvelapack(ParallelJacobian(f,x, NJobs),g);

    delta = normInf(g);
    if(delta>1.)
      {
	double f((2./(1.+std::pow(delta,0.5))));
      for(unsigned int j(0);j<x.getRows();++j)
	x(j,0) -= g(j,0)*f;
      }
    else
      for(unsigned int j(0);j<x.getRows();++j)
	  x(j,0) -= g(j,0);
    std::clog << "ParallelMNewton iter. " << k << " delta: " << delta;
    std::clog << ", |g(x)| = " << normInf(f(x)) << std::endl;
  } while((k++)<MaxIter && delta>tol);
}

template<typename Problem>
void integrate(Problem& p, BoundaryConditions& bc, const Matrix<double,0>& param)
{
  unsigned int N(1000);
  double t(bc.a());
  double dt((bc.b()-bc.a())/(double)N);
  
  state y(bc.lowerPoint(param));

  std::clog << "Initial conditions for integration: " << y << std::endl;
  
  RungeKutta4<Problem> rk4(p);
  for(unsigned int k(0);k<=N;++k, t+=dt)
    {
      std::cout << t << " " << y << std::endl;
      y = rk4.step(t,y,dt);
    }
}

#endif
