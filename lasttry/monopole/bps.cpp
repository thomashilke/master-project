#include <complex>
#include <cmath>
#include <iostream>

#include "../coord.hpp"

std::complex<double> K(const Coord& c)
{
  std::complex<double> z(c.x(), c.y());
  return z/std::sinh(z);
}

std::complex<double> H(const Coord& c)
{
  std::complex<double> z(c.x(), c.y());
  return z/std::tanh(z)-1.;
}


int main()
{
  n_range r(0.1,10,300);
  n_range t(0,M_PI/2., 500);
  
  for(unsigned int i(0);i<r.size();++i)
    {
      for(unsigned int j(0);j<t.size();++j)
	{
	  PolarCoord z(r[i], t[j]);
	  std::cout << z.x() << " "
		    << z.y() << " "
		    << K(z).real() << " "
		    << K(z).imag() << " "
		    << H(z).real() << " "
		    << H(z).imag() << std::endl;
	}
      std::cout << std::endl;
    }
  
  return 0;
}
