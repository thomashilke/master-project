#include <iostream>

#include "../rungekutta4.hpp"
#include "../outputimpl.hpp"

#include "log.hpp"

int main()
{
  std::cout.precision(15);
  n_range r(0.1,1,20);
  n_range y(0.,10., 1000);
  n_range theta(0.,4*M_PI,400); 

  MyProblem p;
  RungeKutta4<MyProblem> rk4(p);
  RangeConsoleOutput out(theta.size(),1);

  for(unsigned int k(0); k<=r.size();++k)
    {
      const function ic(std::valarray<double>(std::log(r[k]), 1),
			std::valarray<double>(0.,          1));
      
      rk4.GenericCoordinatesIntegration<RangeConsoleOutput, PolarCoord>(r[k], theta, ic, out);
    }
  return 0;
}
