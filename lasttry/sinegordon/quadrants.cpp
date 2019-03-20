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

  unsigned int Steps(4000);

  n_range rI_IV(0.1,9, 70);
  n_range rII_III(-0.1,-9, 70);

  n_range thetaI(0.,M_PI/2.,Steps);
  n_range thetaII(M_PI, M_PI/2.,Steps);
  n_range thetaIII(-M_PI,-M_PI/2.,Steps);
  n_range thetaIV(0,-M_PI/2.,Steps);

  MyProblem p;
  RungeKutta4<MyProblem> rk4(p);
  
  std::ofstream fI("i.dat"), fII("ii.dat"), fIII("iii.dat"), fIV("iv.dat");

  RangeConsoleOutput outI(Steps,10,fI);
  RangeConsoleOutput outII(Steps,10,fII);
  RangeConsoleOutput outIII(Steps,10,fIII);
  RangeConsoleOutput outIV(Steps,10,fIV);

  for(unsigned int k(0); k<=rI_IV.size();++k)
    {
      std::cout << "r = " << rI_IV[k] << std::endl;
      std::valarray<double> initialrealpart(2);
      initialrealpart[0] = 4.*std::atan(std::exp(rI_IV[k]));
      initialrealpart[1] = 4.*std::exp(rI_IV[k])/(std::exp(2.*rI_IV[k])+1.);
      
      const function ic(initialrealpart,
			std::valarray<double>(0., 2));

      rk4.GenericCoordinatesIntegration<RangeConsoleOutput, PolarCoord>(rI_IV[k], thetaI, ic, outI);
      rk4.GenericCoordinatesIntegration<RangeConsoleOutput, PolarCoord>(rI_IV[k], thetaIV, ic, outIV);
    }

  for(unsigned int k(0); k<=rII_III.size();++k)
    {
      std::cout << "r = " << rII_III[k] << std::endl;

      std::valarray<double> initialrealpart(2);
      initialrealpart[0] = 4.*std::atan(std::exp(rII_III[k]));
      initialrealpart[1] = 4.*std::exp(rII_III[k])/(std::exp(2.*rII_III[k])+1.);
      
      const function ic(initialrealpart,
			std::valarray<double>(0., 2));

      rk4.GenericCoordinatesIntegration<RangeConsoleOutput, PolarCoord>(rII_III[k], thetaII, ic, outII);
      rk4.GenericCoordinatesIntegration<RangeConsoleOutput, PolarCoord>(rII_III[k], thetaIII, ic, outIII);
    }
  

  return 0;
}
