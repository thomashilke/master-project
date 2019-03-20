#include <iostream>

#include <utils/transform.hpp>

int main()
{
  mat4 m1(mat4::identity());
  
  std::cout << Translate(vec3(1,1,1)) << std::endl;
  std::cout << LookAt(vec3(0,0,-1), vec3(0,0,0), vec3(0,1,0)) << std::endl;


  return 0;
}
