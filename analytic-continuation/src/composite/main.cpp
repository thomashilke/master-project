#include <iostream>

#include "../rungekutta4.hpp"
#include "../outputimpl.hpp"

#include "composite.hpp"

int main()
{
  std::cout.precision(15);
  double r(5);
  n_range y(0.,10., 500);
  n_range theta(0.,4*M_PI,400);
  
  std::valarray<double> initialrealpart(2);
  initialrealpart[0] = sqrt(r);
  initialrealpart[1] = log(r);
  const function ic(initialrealpart,
		    std::valarray<double>(0.,2));
  
  MyProblem p;
  RungeKutta4<MyProblem> rk4(p);
  ConsoleOutput out;
  
  rk4.GenericCoordinatesIntegration<ConsoleOutput, PolarCoord>(r, theta, ic, out);
  return 0;
}
