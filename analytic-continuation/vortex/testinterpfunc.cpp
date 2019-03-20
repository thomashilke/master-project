#include "interpolatingfunction.hpp"
#include <iostream>


int main()
{
  InterpolatingFunction f;

  double rmin(3.), rmax(8.);  

  for(unsigned int i(0);i<40;++i)
    {
      double r(rmin+i*(rmax-rmin)/39.);
      double y(std::sin(2*M_PI*r));
      
      f.addPoint(r, y);
      std::cout << r << " " << y << std::endl;
    }
  
  std::cout << std::endl;
  
  for(unsigned int k(0); k<100;++k)
    {
      double r(rmin+k*(rmax-rmin)/99.);
      double y(f(r));
      std::cout << r << " " << y << std::endl;
    }
  return 0;
}
