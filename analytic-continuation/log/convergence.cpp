#include <iostream>
#include <valarray>
#include <complex>

#include "../rungekutta4.hpp"

#include "sqrt.hpp"

class KeepOnlyLastOutput: public FluxOutput
{
  unsigned int m_lastStep;
public:
  KeepOnlyLastOutput(unsigned int LastStep): FluxOutput(std::cout),
					     m_lastStep(LastStep)
  {}
  virtual void write(const Coord& c,
		     const function& f,
		     unsigned int Step)
  {
    if(Step == m_lastStep)
      {
	m_flux << m_lastStep << " ";
	FluxOutput::writeCartesian(c,f);
	m_flux << std::sqrt(std::complex<double>(c.x(),c.y())).real() << " "
	       << std::sqrt(std::complex<double>(c.x(),c.y())).imag() << " "
	       << std::endl;
      }
  }
};

int main()
{
  std::cout.precision(15);
  double r(5);
  unsigned int N(500);
  for(unsigned int k(0);k<8;++k,N*=2)
    {
      n_range y(0.,10.,N); 
      const function ic(std::valarray<double>(sqrt(r), 1),
			std::valarray<double>(0.,      1));
      
      MyProblem p;
      RungeKutta4<MyProblem> rk4(p);
      KeepOnlyLastOutput out(N);
      
      rk4.CartesianIntegration(r, y, ic, out);
    }
  return 0;
}
