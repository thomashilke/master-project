#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>

template<typename T, unsigned int n>
class Vector
{
  T m_array[n];
public:
  Vector()
  {
    for(unsigned int i(0);i<n;i++)
      m_array[i] = 0.;
  }
  Vector(T x)
  {
    m_array[0] = x;
  }
  Vector(T x, T y)
  {
    m_array[0] = x;
    m_array[1] = y;
  }
  Vector(T x, T y, T z)
  {
    m_array[0] = x;
    m_array[1] = y;
    m_array[2] = z;
  }
  Vector(T x, T y, T z, T w)
  {
    m_array[0] = x;
    m_array[1] = y;
    m_array[2] = z;
    m_array[3] = w;
  }
  
  Vector(const Vector<T,n>& op)
  {
    for(unsigned int i(0);i<n;i++)
      m_array[i] = op.m_array[i];
  }
  
  Vector<T,n>& operator=(const Vector<T,n>& op);

  T& at(unsigned int i){return m_array[i];}
  const T& at(unsigned int i) const {return m_array[i];}
};

template<typename T, unsigned int n>
Vector<T,n>& operator+=(const Vector<T,n>& op1, const Vector<T,n>& op2)
{
  for(unsigned int i(0);i<n;++i)
    op1.at(i) += op2.at(i);
  return op1;
}

template<typename T, unsigned int n>
Vector<T,n>& operator-=(const Vector<T,n>& op1, const Vector<T,n>& op2)
{
  for(unsigned int i(0);i<n;++i)
    op1.at(i) += op2.at(i);
  return op1;
}

template<typename T, unsigned int n>
Vector<T,n>& operator*=(const Vector<T,n>& op1, const T& op2)
{
  for(unsigned int i(0);i<n;++i)
    op1.at(i) *= op2;
  return op1;
}

template<typename T, unsigned int n>
Vector<T,n>& operator/=(const Vector<T,n>& op1, const T& op2)
{
  for(unsigned int i(0);i<n;++i)
    op1.at(i) /= op2;
  return op1;
}

template<typename T, unsigned int n>
Vector<T,n> operator+(const Vector<T,n>& op1, const Vector<T,n>& op2)
{
  Vector<T,n> r(op1);
  return r += op2;
}

template<typename T, unsigned int n>
Vector<T,n> operator-(const Vector<T,n>& op1, const Vector<T,n>& op2)
{
  Vector<T,n> r(op1);
  return r -= op2;
}

template<typename T, unsigned int n>
Vector<T,n> operator*(const Vector<T,n>& op1, const T& op2)
{
  Vector<T,n> r(op1);
  return r *= op2;
}

template<typename T, unsigned int n>
Vector<T,n> operator*(const T& op1, const Vector<T,n>& op2)
{
  Vector<T,n> r(op2);
  return r *= op2;
}

template<typename T, unsigned int n>
Vector<T,n> operator/(const Vector<T,n>& op1, const T& op2)
{
  Vector<T,n> r(op1);
  return r /= op2;
}

template<typename T, unsigned int n>
T DotProduct(const Vector<T,n>& op1, const Vector<T,n>& op2)
{
  T r(0);
  for(unsigned int i(0);i<n;++i)
    r += op1.at(i)*op2.at(i);
  return r;
}

template<typename T>
Vector<T,2> Perp(const Vector<T,2>& op)
{
  return Vector<T,2>(- op.at(1), op.at(0));
}

template<typename T>
Vector<T,3> CrossProduct(const Vector<T,3>& op1, const Vector<T,3>& op2)
{
  return Vector<T,3>(op1.at(1)*op2.at(2)-op1.at(2)*op2.at(1),
		     op1.at(2)*op2.at(0)-op1.at(0)*op2.at(2),
		     op1.at(0)*op2.at(1)-op1.at(1)*op2.at(0));
}

template<typename T, unsigned int n>
std::ostream& operator<<(std::ostream& Flux, const Vector<T,n>& v)
{
  Flux << "Vector<T," << n << ">(";
  for(unsigned int i(0);i<n-1;++i)
    Flux << v.at(i) << ", ";
  Flux << v.at(n-1) << ")";
  return Flux;
}

typedef Vector<double,2> vec2;
typedef Vector<double,3> vec3;

#endif

