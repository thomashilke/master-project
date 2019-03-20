#include <cstdlib>

#include "matrix.hpp"
#include "matrix/io.hpp"
#include "matrix/algorithms.hpp"


int main()
{
  unsigned int n(10);

  Mat A(n,n);
  Mat b(n,1);
  
  for(unsigned int i(1);i<=n;++i)
    for(unsigned int j(1);j<=n;++j)
      A(i,j) = 2.*(Mat::storage)rand()/(Mat::storage)RAND_MAX - 1.;
  
  for(unsigned int i(1);i<=n;++i)
    b(i, 1) = 2.*(Mat::storage)rand()/(Mat::storage)RAND_MAX - 1.;

  Mat bsaved(b);

  std::cout << "A = " << std::endl
	    << A << std::endl;
  std::cout << "b = " << std::endl 
	    << b << std::endl;
  
  Solve(A,b);
  
  std::cout << "=> x = " << std::endl 
	    << b << std::endl;
  
  
  Mat t(n,1);
  Mult(t, A, b);
  std::cout << "because A*x = " << std::endl
	    << t << std::endl;
  
  Mat delta(n,1);  
  Substract(delta, t,bsaved);
  
  std::cout << "The difference is:" << std::endl
	    << delta << std::endl;
  
  return 0;
}
