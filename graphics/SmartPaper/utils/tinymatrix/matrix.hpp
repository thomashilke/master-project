#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>

#include "vector.hpp"

template<typename T, unsigned int n>
class Matrix
{
  T m_array[n*n];
public:
  Matrix()
  {
    /*for(unsigned int i(0);i<n*n;++i)
      m_array[i] = 0.;*/
  }

  Matrix(const Matrix<T,n>& op)
  {
    for(unsigned int i(0);i<n*n;++i)
      m_array[i] = op.m_array[i];
  }
  //virtual ~Matrix();

  static Matrix<T,n> identity()
  {
    Matrix<T,n> r(zero());
    for(unsigned int i(0);i<n;++i)
      r.at(i,i) = 1.;
    return r;
  }

  static Matrix<T,n> zero()
  {
    Matrix<T,n> r;
    for(unsigned int i(0);i<n*n;++i)
      r.m_array[i] = 0.;
    return r;
  }


  Matrix<T,n>& operator=(const Matrix<T,n>& op)
  {
    for(unsigned int i(0);i<n*n;++i)
      m_array[i] = op.m_array[i];
    return *this;
  }

  T& at(unsigned int i, unsigned int j)
  { return m_array[i+n*j]; }
  const T& at(unsigned int i, unsigned int j) const
  { return m_array[i+n*j]; }
};


template<typename T, unsigned int n>
Matrix<T,n>& operator+=(const Matrix<T,n>& op1, const Matrix<T,n>& op2)
{
  for(unsigned int i(0);i<n;++i)
    for(unsigned int j(0);j<n;++j)
      op1.at(i,j) += op2.at(i,j);
  return op1;
}

template<typename T, unsigned int n>
Matrix<T,n>& operator-=(const Matrix<T,n>& op1, const Matrix<T,n>& op2)
{
  for(unsigned int i(0);i<n;++i)
    for(unsigned int j(0);j<n;++j)
      op1.at(i,j) -= op2.at(i,j);
  return op1;
}

template<typename T, unsigned int n>
Matrix<T,n> operator*(const T& op1, const Matrix<T,n>& op2)
{
  Matrix<T,n> r(op2);
  return r *= op1;
}

template<typename T, unsigned int n>
Matrix<T,n> operator*(const Matrix<T,n>& op1, const T& op2)
{
  Matrix<T,n> r(op1);
  return r *= op2;
}

template<typename T, unsigned int n>
Matrix<T,n>& operator*=(Matrix<T,n>& op1, const Matrix<T,n>& op2)
{
  op1 = op1*op2;

  return op1;
}

template<typename T, unsigned int n>
Matrix<T,n>& operator*=(Matrix<T,n>& op1, const T& op2)
{
  for(unsigned int i(0);i<n;++i)
    for(unsigned int j(0);j<n;++j)
      op1.at(i,j) *= op2;
  return op1;
}

template<typename T, unsigned int n>
Matrix<T,n> operator+(const Matrix<T,n>& op1, const Matrix<T,n>& op2)
{
  Matrix<T,n> r(op1);
  return r += op2;
}

template<typename T, unsigned int n>
Matrix<T,n> operator-(const Matrix<T,n>& op1, const Matrix<T,n>& op2)
{
  Matrix<T,n> r(op1);
  return r -= op2;
}

template<typename T, unsigned int n>
Matrix<T,n> operator*(const Matrix<T,n>& op1, const Matrix<T,n>& op2)
{
  Matrix<T,n> t(Matrix<T,n>::zero());

  for(unsigned int i(0);i<n;++i)
    for(unsigned int j(0);j<n;++j)
      for(unsigned int k(0);k<n;++k)
	t.at(i,j) += op1.at(i,k)*op2.at(k,j);

  return t;
}

template<typename T, unsigned int n>
Matrix<T,n>& operator/=(const Matrix<T,n>& op1, const T& op2)
{
  Matrix<T,n> r(op1);
  return r /= op2;
}

template<typename T, unsigned int n>
Matrix<T,n> Transpose(const Matrix<T,n>& op)
{
  Matrix<T,n> r(op);
  for(unsigned int i(0);i<n-1;++i)
    for(unsigned int j(i+1);j<n;++j)
      std::swap(op.at(i,j),op.at(j,i));
  return r;
}

template<typename T>
Matrix<T,2> Inverse(const Matrix<T,2>& op)
{
  T det(op.at(0,0)*op.at(1,1)-op.at(1,0)*op.at(0,1));
  Matrix<T,2> r;
  r.at(0,0) = op.at(1,1)/det;
  r.at(0,1) = - op.at(0,1)/det;
  r.at(1,0) = - op.at(1,0)/det;
  r.at(1,1) = op.at(0,0)/det;
  return r;
}

template<typename T>
Matrix<T,3> Inverse(const Matrix<T,3>& op)
{
  T& a(op.at(0,0));
  T& b(op.at(0,1));
  T& c(op.at(0,2));

  T& d(op.at(1,0));
  T& e(op.at(1,1));
  T& f(op.at(1,2));

  T& g(op.at(2,0));
  T& h(op.at(2,1));
  T& k(op.at(2,2));


  T det(a*(e*k-f*h)
	+b*(f*g-k*d)
	+c*(d*h-e*g));

  Matrix<T,3> r;
  r.at(0,0) = (e*k-f*g)/det;
  r.at(0,1) = (c*h-b*k)/det;
  r.at(0,2) = (b*f-c*e)/det;
  
  r.at(1,0) = (f*g-d*k)/det;
  r.at(1,1) = (a*k-c*g)/det;
  r.at(1,2) = (c*d-a*f)/det;
  
  r.at(2,0) = (d*h-e*g)/det;
  r.at(2,1) = (g*b-a*h)/det;
  r.at(2,2) = (a*e-b*d)/det;

  return r;
}

template<typename T, unsigned int n>
Vector<T,n> operator*(const Matrix<T,n>& m, const Vector<T,n>& v)
{
  Vector<T,n> r;
  
  for(unsigned int i(0);i<n;++i)
    for(unsigned int j(0);j<n;++j)
      r.at(i) += m.at(i,j)*v.at(j);

  return r;
}

template<typename T, unsigned int n>
std::ostream& operator<<(std::ostream& Flux, const Matrix<T,n>& m)
{
  Flux << "Matrix<T," << n << ">[";
  for(unsigned int i(0);i<n;++i)
    {
      if(i != n-1)
	{
	  for(unsigned int j(0);j<n-1;++j)
	    Flux << m.at(i,j) << ", ";
	  Flux << m.at(i,n-1) << "; ";
	}
      else
	{
	  for(unsigned int j(0);j<n-1;++j)
	    Flux << m.at(i,j) << ", ";
	}
    }
  Flux << m.at(n-1,n-1) << "]";

  return Flux;
}

typedef Matrix<double,2> mat2;
typedef Matrix<double,3> mat3;
typedef Matrix<double,4> mat4;

#endif
