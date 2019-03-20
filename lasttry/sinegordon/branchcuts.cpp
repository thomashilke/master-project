#include <iostream>
#include <fstream>

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

  unsigned int Steps(40000);

  double r1(M_PI/2.-0.01);
  double r2(M_PI/2.+0.01);

  n_range theta(0.,M_PI,Steps);

  MyProblem p;
  RungeKutta4<MyProblem> rk4(p);

  std::ofstream fl("./lower.dat");
  std::ofstream fu("./higher.dat");
  
  PolarFluxOutput outl(fu);
  PolarFluxOutput outu(fl);

  /*CartesianFluxOutput out1(f1);
    CartesianFluxOutput out2(f2);*/

  std::valarray<double> initialrealpart(2);

  initialrealpart[0] = 4.*std::atan(std::exp(r1));
  initialrealpart[1] = 4.*std::exp(r1)/(std::exp(2.*r1)+1.);
  {
    const function ic(initialrealpart,
		      std::valarray<double>(0., 2));
    
    rk4.GenericCoordinatesIntegration<PolarFluxOutput, PolarCoord>(r1, theta, ic, outl);      
  }

  initialrealpart[0] = 4.*std::atan(std::exp(r2));
  initialrealpart[1] = 4.*std::exp(r2)/(std::exp(2.*r2)+1.);
  {
    const function ic(initialrealpart,
		      std::valarray<double>(0., 2));
    
    rk4.GenericCoordinatesIntegration<PolarFluxOutput, PolarCoord>(r2, theta, ic, outu);
  }

  return 0;
}
