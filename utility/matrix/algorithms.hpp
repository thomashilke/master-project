#ifndef ALGORITHMS
#define ALGORITHMS

#include <string>
#include <cmath>
#include <vector>

extern "C" {
#include <atlas/clapack.h>
}

#include "matrix.hpp"

double normInf(const Matrix<double,0>& x)
{
  double s(0.);
  for(unsigned int k(0);k<x.getRows();++k)
    s += std::abs(x(k,0));
  return s;
}

template<typename Storage, int Zero>
bool checkForNaN(const Matrix<double,0>& x)
{
  for(unsigned int i(Zero);i<x.getRows()+Zero;++i)
    for(unsigned int j(Zero);j<x.getColumns()+Zero;++j)
      if(x(i,j) != x(i,j))
	return false;
  return true;
}


template<typename Storage, int Zero>
void LU(Matrix<Storage, Zero>& r, const Matrix<Storage, Zero>& A)
{
  if(A.getRows() != A.getColumns())
    throw std::string("LU() - The LU decomposition is not "
		      "defined for non-square A matrix. Aborting.");
  
  unsigned int n(A.getRows());
  
  if(r.getRows() != n || r.getColumns() != n)
    r.Resize(n,n);
  
  Matrix<Storage, Zero>& U(r);
  Matrix<Storage, Zero>& L(r);
  
  //Initialisation:
  for(unsigned int j(Zero);j<n+Zero;++j)
    {
      U(Zero,j) = A(Zero,j);
    }
  for(unsigned int j(Zero+1);j<n+Zero;++j)
    {
      if(std::abs(U(Zero,Zero)) < 1e-14)
	throw std::string("void LU(Matrix, Matrix) - Divide by Zero.");
      L(j,Zero) = A(j,Zero)/U(Zero,Zero);
    }

  //Induction step:
  for(unsigned int k(Zero+1);k<n-1+Zero; ++k)
    {
      for(unsigned int j(k);j<n+Zero;++j)
	{
	  U(k,j) = A(k,j);
	  for(unsigned int m(Zero);m<=k-1;++m)
	    U(k,j) -= L(k,m)*U(m,j);
	}
      for(unsigned int j(k+1);j<n+Zero;++j)
	{
	  if(std::abs(U(k,k)) < 1e-14)
	    throw std::string("void LU(Matrix, Matrix) - Divide by Zero.");

	  L(j,k) = A(j,k);
	  for(unsigned int m(Zero);m<=k-1;++m)
	    L(j,k) -= L(j,m)*U(m,k);
	  L(j,k) /= U(k,k);
	}
    }
  
  //End:
  U(n-1+Zero,n-1+Zero) = A(n-1+Zero,n-1+Zero);
  for(unsigned int m(Zero);m<n+Zero-1;++m)
    U(n-1+Zero,n-1+Zero) -= L(n-1+Zero,m)*U(m,n-1+Zero);
}

template<typename Storage, int Zero>
void LUp(Matrix<Storage, Zero>& lu, std::vector<unsigned int>& indx)
{
  if(lu.getRows() != lu.getColumns())
    throw std::string("LUp() - The LU decomposition is not "
		      "defined for non-square A matrix. Aborting.");
  
  double big(0.);
  double temp(0.);
  unsigned int imax(0);

  double d(1.0);
  unsigned int n(lu.getRows());
  
  if(lu.getRows() != n || lu.getColumns() != n)
    lu.Resize(n,n);
  
  indx.resize(n);
  for(unsigned int i(0);i<n;++i)
    indx[i] = i;
  
  std::vector<double> vv(n,0.);
  for (unsigned int i(0);i<n;++i)
    {
      big = 0.;
      for (unsigned int j(0);j<n;++j)
	if ((temp=std::abs(lu(i,j))) > big)
	  big=temp;
      if (big == 0.0)
	throw std::string("void LUp(Matrix, std::vector<uint>) - Singular matrix");
      vv[i]=1.0/big;
    }
  
  for (unsigned int k(0);k<n;k++)
    {
      big = 0.;
      for (unsigned int i(k);i<n;i++)
	{
	  temp=vv[i]*std::abs(lu(i,k));
	  if (temp > big)
	    {
	      big = temp;
	      imax = i;
	    }
	}
      if (k != imax)
	{
	  for (unsigned int j(0);j<n;j++)
	    std::swap(lu(imax,j), lu(k,j));
	  d = -d;
	  std::swap(vv[imax],vv[k]);
	  std::swap(indx[k],indx[imax]);
	}
      for (unsigned int i(k+1);i<n;i++)
	{
	  temp = lu(i,k) /= lu(k,k);
	  for (unsigned int j(k+1);j<n;j++)
	    lu(i,j) -= temp*lu(k,j);
	}
    }
}


template<typename Storage, int Zero>
void Mult(Matrix<Storage, Zero>& r, const Matrix<Storage, Zero>& A, const Matrix<Storage, Zero>& B)
{
  if(A.getColumns() != B.getRows())
    throw std::string("Mult() - Incompatible sizes. Aborting.");


  unsigned int m(A.getRows());
  unsigned int n(B.getColumns());
  
  if(r.getRows() != m || r.getColumns() != n)
    r.Resize(m,n);

  for(unsigned int i(Zero);i<m+Zero;++i)
    for(unsigned int j(Zero);j<n+Zero;++j)
      {
	r(i,j) = 0.;
	for(unsigned int k(Zero);k<A.getColumns()+Zero;++k)
	  r(i,j) += A(i,k)*B(k,j);
      }
}

template<typename Storage, int Zero>
void Substract(Matrix<Storage, Zero>& r, const Matrix<Storage, Zero>& A, const Matrix<Storage, Zero>& B)
{
  if(A.getColumns() != B.getColumns() || A.getRows() != B.getRows())
    throw std::string("Substact() - Sizes mismatch. Aborting.");

  unsigned int m(A.getRows());
  unsigned int n(A.getColumns());
  
  if(r.getRows() != m || r.getColumns() != n)
    r.Resize(m,n);

  for(unsigned int i(Zero);i<m+Zero;++i)
    for(unsigned int j(Zero);j<n+Zero;++j)
      r(i,j) = A(i,j) - B(i,j);
}

template<typename Storage, int Zero>
void Add(Matrix<Storage, Zero>& r, const Matrix<Storage, Zero>& A, const Matrix<Storage, Zero>& B)
{
  if(A.getColumns() != B.getColumns() || A.getRows() != B.getRows())
    throw std::string("Add() - Sizes mismatch. Aborting.");

  unsigned int m(A.getRows());
  unsigned int n(A.getColumns());
  
  if(r.getRows() != m || r.getColumns() != n)
    r.Resize(m,n);

  for(unsigned int i(Zero);i<m+Zero;++i)
    for(unsigned int j(Zero);j<n+Zero;++j)
      r(i,j) = A(i,j) + B(i,j);
}

template<typename Storage, int Zero>
void RemoveFrom(Matrix<Storage, Zero>& A, const Matrix<Storage, Zero>& B)
{
  if(A.getColumns() != B.getColumns() || A.getRows() != B.getRows())
    throw std::string("Add() - Sizes mismatch. Aborting.");

  unsigned int m(A.getRows());
  unsigned int n(A.getColumns());
  
  for(unsigned int i(Zero);i<m+Zero;++i)
    for(unsigned int j(Zero);j<n+Zero;++j)
      A(i,j) -= B(i,j);
}

template<typename Storage, int Zero>
Storage EuclidianNorm(const Matrix<Storage, Zero>& m)
{
  if(m.getColumns() != 1)
    throw std::string("EuclidianNorm()"
		      " - The Euclidian norm is only defined on "
		      "vectors, and not on matrices.. Aborting.");

  Storage s(m(Zero,Zero)*m(Zero,Zero));
  for(unsigned int i(Zero+1);i<m.getRows()+Zero;++i)
    s += m(i,Zero)*m(i,Zero);

  return std::sqrt(s);
}

template<typename Storage, int Zero>
void InPlaceSolveFromLU(const Matrix<Storage, Zero>& lu, Matrix<Storage, Zero>& v)
{
  if(v.getColumns() != 1)
    throw std::string("InPlaceSolveFromLu() - v must be a vector, i.e., a matrix with 1 row. Aborting.");

  if(lu.getColumns() != v.getRows())
    throw std::string("InPlaceSolveFromLu() - dimensions mismatch. Aborting.");

  if(lu.getRows() != lu.getColumns())
    throw std::string("InPlaceSolveFromLu() - lu matrix must be square.");
  
  unsigned int n(v.getRows());

  //Initial is already done. (Ly = v)
  for(unsigned int k(Zero+1);k<n+Zero;++k)
    for(unsigned int m(Zero);m<=k-1;++m)
      v(k, Zero) -= lu(k,m)*v(m, Zero);

  if(std::abs(lu(n-1+Zero,n-1+Zero)) < 1e-14)
    throw std::string("void InPlaceSolveFromLU(Matrix, Matrix) - Divide by Zero.");
  //Init: (Ux = y)
  v(n-1+Zero, Zero) /= lu(n-1+Zero,n-1+Zero);
  
  for(int k(n-1+Zero-1);k>=Zero;--k)
    {
      for(unsigned int m(k+1);m<=n-1+Zero;++m)
	v(k, Zero) -= lu(k,m)*v(m, Zero);
      if(std::abs(lu(k,k)) < 1e-14)
	throw std::string("void LU(Matrix, Matrix) - Divide by Zero.");
      v(k, Zero) /= lu(k,k);
    }
}

template<typename Storage, int Zero>
void InPlaceSolveFromLUp(const Matrix<Storage, Zero>& lu,
			 const std::vector<unsigned int>& indx,
			 Matrix<Storage, Zero>& v,
			 Matrix<Storage, Zero>& bp)
{
    if(v.getColumns() != 1)
    throw std::string("InPlaceSolveFromLUp() - v must be a vector, i.e., a matrix with 1 row. Aborting.");

  if(lu.getColumns() != v.getRows())
    throw std::string("InPlaceSolveFromLUp() - dimensions mismatch. Aborting.");

  if(lu.getRows() != lu.getColumns())
    throw std::string("InPlaceSolveFromLUp() - lu matrix must be square.");
  
  unsigned int n(v.getRows());


  //Matrix<Storage, Zero> b(bp);
  for(unsigned int k(Zero);k<n+Zero;++k)
    v(k,Zero) = bp(indx[k-Zero]+Zero,Zero);

  //Initial is already done. (Ly = v)
  for(unsigned int k(Zero+1);k<n+Zero;++k)
    for(unsigned int m(Zero);m<=k-1;++m)
      v(k, Zero) -= lu(k,m)*v(m, Zero);

  if(std::abs(lu(n-1+Zero,n-1+Zero)) < 1e-14)
    throw std::string("void InPlaceSolveFromLUp(Matrix, Matrix) - Divide by Zero.");
  //Init: (Ux = y)
  v(n-1+Zero, Zero) /= lu(n-1+Zero,n-1+Zero);
  
  for(int k(n-1+Zero-1);k>=Zero;--k)
    {
      for(unsigned int m(k+1);m<=n-1+Zero;++m)
	v(k, Zero) -= lu(k,m)*v(m, Zero);
      if(std::abs(lu(k,k)) < 1e-14)
	throw std::string("void InPlaceSolveFromLUp(Matrix, Matrix) - Divide by Zero.");
      v(k, Zero) /= lu(k,k);
    }
}

template<typename Storage, int Zero>
void Solve(const Matrix<Storage, Zero>& A, Matrix<Storage, Zero>& v)
{
  if(A.getRows() != A.getColumns())
    throw std::string("Solve() - A must be square. Aborting.");

  if(A.getColumns() != v.getRows())
    throw std::string("Solve() - Dimensions mismatch. Aborting.");

  Matrix<Storage,Zero> lu(A.getRows(), A.getRows());
  LU(lu, A);
  InPlaceSolveFromLU(lu,v);
}

template<typename Storage, int Zero>
void Solvep(const Matrix<Storage, Zero>& A, Matrix<Storage, Zero>& v)
{
  if(A.getRows() != A.getColumns())
    throw std::string("Solvep() - A must be square. Aborting.");

  if(A.getColumns() != v.getRows())
    throw std::string("Solvep() - Dimensions mismatch. Aborting.");

  Matrix<Storage,Zero> lu(A);
  std::vector<unsigned int> indx;
  LUp(lu, indx);

  Matrix<Storage,Zero> y(v);
  InPlaceSolveFromLUp(lu,indx,v,y);
}

template<typename Storage, int Zero>
void Solvelapack(const Matrix<Storage, Zero>& A, Matrix<Storage, Zero>& v)
{
  if(A.getRows() != A.getColumns())
    throw std::string("Solvep() - A must be square. Aborting.");

  if(A.getColumns() != v.getRows())
    throw std::string("Solvep() - Dimensions mismatch. Aborting.");


  unsigned int N(A.getRows());

  Matrix<double,0> lu(A);  
  std::vector<int > p(N);

  int info(0);
  clapack_dgesv(CblasColMajor,
		N,
		1,
		&lu(0,0),
		N,
		&p[0],
		&v(0,0),
		N);
}

#endif
