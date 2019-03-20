#include <iostream>
#include <cstdlib>
#include "utility/matrix/matrix.hpp"
#include "utility/matrix/algorithms.hpp"
#include "utility/matrix/io.hpp"

extern "C" { 
#include <atlas/clapack.h>
}

double randN()
{
  return (double)rand()/(double)RAND_MAX;
}

int main()
{
  srand(time(NULL));

  std::cout.precision(5);
  try{
    unsigned int N(10);
    
    
    Matrix<double,0> b(N,1);
    Matrix<double,0> A(N,N);

    for(unsigned int i(0);i<N;++i)
      {
	b(i,0) = randN();
	for(unsigned int j(0);j<N;++j)
	  A(i,j) = randN();
      }

    std::cout << A << std::endl;
    std::cout << b << std::endl;

    Matrix<double,0> x(b);
    Matrix<double,0> LU(A);
    //Solvep(LU,x);
    std::vector<int > p(N);
    int info(0);
    clapack_dgesv(CblasColMajor,
		  N,
		  1,
		  &LU(0,0),
		  N,
		  &p[0],
		  &x(0,0),
		  N);

    Matrix<double,0> bp(N,1);
    Mult(bp, A, x);

    RemoveFrom(b,bp);
    std::cout << b << std::endl;
  }
  catch(std::string& e)
    { std::cout << e << std::endl; }
  return 0;
}
