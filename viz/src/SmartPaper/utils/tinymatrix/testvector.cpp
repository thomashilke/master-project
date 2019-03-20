#include "vector.hpp"
#include "matrix.hpp"

int main()
{
  vec2 v1;
  vec3 v2;

  mat2 m1;
  mat3 m2;

  v1.at(0) = 1;
  v1.at(1) = 2;

  m1.at(0,0) = 1;
  m1.at(0,1) = 2;
  m1.at(1,0) = 3;
  m1.at(1,1) = 4;
  
  std::cout << m1 << std::endl;
  std::cout << v1 << std::endl;
  
  std::cout << m1*v1 << std::endl;
  return 0;
}
