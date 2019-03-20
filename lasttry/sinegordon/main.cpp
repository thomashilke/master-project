#include <iostream>

#include "../rungekutta4.hpp"
#include "../outputimpl.hpp"

#include "sinegordon.hpp"

std::complex<double> atan(const std::complex<double>& c)
{
  return 0.5*std::complex<double>(0.,1.)*(log(1.-std::complex<double>(0.,1.)*c)
					  -log(1.+std::complex<double>(0.,1.)*c));
}

int main()
{
  std::cout.precision(15);

  unsigned int Steps(20000);

  n_range r(0.,-1,3.*M_PI, 202);
  n_range theta(0,M_PI,Steps);
  n_range y(0.1,6.,Steps);

  MyProblem p;
  RungeKutta4<MyProblem> rk4(p);
  RangeConsoleOutput out(Steps,50);

  for(unsigned int k(0); k<r.size();++k)
    {
      std::valarray<double> initialrealpart(2);
      initialrealpart[0] = 4.*std::atan(std::exp(r[k]));
      initialrealpart[1] = 4.*std::exp(r[k])/(std::exp(2.*r[k])+1.);
      
      const function ic(initialrealpart,
			std::valarray<double>(0., 2));

      rk4.GenericCoordinatesIntegration<RangeConsoleOutput, PolarCoord>(r[k], theta, ic, out);
    }

  return 0;
}
