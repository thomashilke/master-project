#include "dampeddynamic.hpp"
#include <iostream>

int main()
{
  FakeChrono fc;
  DampedDynamic dd(0., &fc);
  dd.addSlope(SlopeScrollFunction(2.,2.));

  for(unsigned int i(0);i<1000;++i)
    {
      dd.Update();
      std::cout << fc.getTime() << " " << dd.getValue() << " " << dd.f(fc.getTime()) << std::endl;
    }
  
  return 0;
}
