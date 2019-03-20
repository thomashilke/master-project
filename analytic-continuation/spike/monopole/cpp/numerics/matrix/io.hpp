#ifndef IO
#define IO

#include <iostream>
#include <iomanip>

#include "../matrix.hpp"

std::ostream& operator<<(std::ostream& flux, const Mat& m)
{
  unsigned int p(flux.precision(3));
  for(unsigned int i(1); i<=m.getRows(); ++i)
    {
      for(unsigned int j(1); j<=m.getColumns(); ++j)
	flux << std::setw(10) << m(i,j);
      flux << std::endl;
    }
  flux.precision(p);
  
  return flux;
}

#endif
