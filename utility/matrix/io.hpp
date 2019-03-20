#ifndef IO
#define IO

#include <iostream>
#include <iomanip>

#include "matrix.hpp"

template<typename T, int Zero>
std::ostream& operator<<(std::ostream& flux, const Matrix<T,Zero>& m)
{
  unsigned int p(flux.precision(10));
  for(int i(Zero); i<(int)m.getRows()+Zero; ++i)
    {
      for(int j(Zero); j<(int)m.getColumns()+Zero; ++j)
	flux << std::setw(5) << m(i,j) << " ";
      flux << std::endl;
    }
  flux.precision(p);
  
  return flux;
}

#endif
