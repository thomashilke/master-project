#ifndef ALGORITHMS
#define ALGORITHMS

#include <string>
#include <cmath>

#include "../matrix.hpp"

template<typename Storage>
void LU(Matrix<Storage>& r, const Matrix<Storage>& A)
{
  if(A.getRows() != A.getColumns())
    throw std::string("LU() - The LU decomposition is not "
		      "defined for non-square A matrix. Aborting.");
  
  unsigned int n(A.getRows());
  
  if(r.getRows() != n || r.getColumns() != n)
    r.Resize(n,n);
  
  Matrix<Storage>& U(r);
  Matrix<Storage>& L(r);
  
  //Initialisation:
  for(unsigned int j(1);j<=n;++j)
    {
      L(j,1) = A(j,1)/U(1,1);
      U(1,j) = A(1,j);
    }
  
  //Induction step:
  for(unsigned int k(2);k<=n-1; ++k)
    {
      for(unsigned int j(k);j<=n;++j)
	{
	  U(k,j) = A(k,j);
	  for(unsigned int m(1);m<=k-1;++m)
	    U(k,j) -= L(k,m)*U(m,j);
	}
      for(unsigned int j(k+1);j<=n;++j)
	{
	  L(j,k) = A(j,k);
	  for(unsigned int m(1);m<=k-1;++m)
	    L(j,k) -= L(j,m)*U(m,k);
	  L(j,k) /= U(k,k);
	}
    }
  
  //End:
  U(n,n) = A(n,n);
  for(unsigned int m(1);m<=n-1;++m)
    U(n,n) -= L(n,m)*U(m,n);
}


template<typename Storage>
void Mult(Matrix<Storage>& r, const Matrix<Storage>& A, const Matrix<Storage>& B)
{
  if(A.getColumns() != B.getRows())
    throw std::string("Mult() - Incompatible sizes. Aborting.");


  unsigned int m(A.getRows());
  unsigned int n(B.getColumns());
  
  if(r.getRows() != m || r.getColumns() != n)
    r.Resize(m,n);

  for(unsigned int i(1);i<=m;++i)
    for(unsigned int j(1);j<=n;++j)
      {
	r(i,j) = 0.;
	for(unsigned int k(1);k<=A.getColumns();++k)
	  r(i,j) += A(i,k)*B(k,j);
      }
};

template<typename Storage>
void Substract(Matrix<Storage>& r, const Matrix<Storage>& A, const Matrix<Storage>& B)
{
  if(A.getColumns() != B.getColumns() || A.getRows() != B.getRows())
    throw std::string("Substact() - Sizes mismatch. Aborting.");

  unsigned int m(A.getRows());
  unsigned int n(A.getColumns());
  
  if(r.getRows() != m || r.getColumns() != n)
    r.Resize(m,n);

  for(unsigned int i(1);i<=m;++i)
    for(unsigned int j(1);j<=n;++j)
      r(i,j) = A(i,j) - B(i,j);
}

template<typename Storage>
void Add(Matrix<Storage>& r, const Matrix<Storage>& A, const Matrix<Storage>& B)
{
  if(A.getColumns() != B.getColumns() || A.getRows() != B.getRows())
    throw std::string("Add() - Sizes mismatch. Aborting.");

  unsigned int m(A.getRows());
  unsigned int n(A.getColumns());
  
  if(r.getRows() != m || r.getColumns() != n)
    r.Resize(m,n);

  for(unsigned int i(1);i<=m;++i)
    for(unsigned int j(1);j<=n;++j)
      r(i,j) = A(i,j) + B(i,j);
}

template<typename Storage>
void RemoveFrom(Matrix<Storage>& A, const Matrix<Storage>& B)
{
  if(A.getColumns() != B.getColumns() || A.getRows() != B.getRows())
    throw std::string("Add() - Sizes mismatch. Aborting.");

  unsigned int m(A.getRows());
  unsigned int n(A.getColumns());
  
  for(unsigned int i(1);i<=m;++i)
    for(unsigned int j(1);j<=n;++j)
      A(i,j) -= B(i,j);
}

template<typename Storage>
Storage EuclidianNorm(const Matrix<Storage>& m)
{
  if(m.getColumns() != 1)
    throw std::string("EuclidianNorm()"
		      " - The Euclidian norm is only defined on "
		      "vectors, and not on matrices.. Aborting.");

  Storage s(m(1,1)*m(1,1));
  for(unsigned int i(2);i<=m.getRows();++i)
    s += m(i,1)*m(i,1);

  return std::sqrt(s);
}

template<typename Storage>
void InPlaceSolveFromLU(const Matrix<Storage>& lu, Matrix<Storage>& v)
{
  if(v.getColumns() != 1)
    throw std::string("InPlaceSolveFromLu() - v must be a vector, i.e., a matrix with 1 row. Aborting.");

  if(lu.getColumns() != v.getRows())
    throw std::string("InPlaceSolveFromLu() - dimensions mismatch. Aborting.");

  if(lu.getRows() != lu.getColumns())
    throw std::string("InPlaceSolveFromLu() - lu matrix must be square.");
  
  unsigned int n(v.getRows());

  //Initial is already done. (Ly = v)
  for(unsigned int k(2);k<=n;++k)
    for(unsigned int m(1);m<=k-1;++m)
      v(k, 1) -= lu(k,m)*v(m, 1);

  //Init: (Ux = y)
  v(n, 1) /= lu(n,n);
  
  for(unsigned int k(n-1);k!=0;--k)
    {
      for(unsigned int m(k+1);m<=n;++m)
	v(k, 1) -= lu(k,m)*v(m, 1);
      v(k, 1) /= lu(k,k);
    }
}

template<typename Storage>
void Solve(const Matrix<Storage>& A, Matrix<Storage>& v)
{
  if(A.getRows() != A.getColumns())
    throw std::string("Solve() - A must be square. Aborting.");

  if(A.getColumns() != v.getRows())
    throw std::string("Solve() - Dimensions mismatch. Aborting.");

  Mat lu(A.getRows(), A.getRows());
  LU(lu, A);
  InPlaceSolveFromLU(lu,v);
}

#endif
