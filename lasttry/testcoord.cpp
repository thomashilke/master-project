#include <iostream>
#include "../coord.hpp"


int main()
{
  CartCoord c1(-1,1);
  PolarCoord c2(1,);

  c1.print(std::cout);
  c2.print(std::cout);
}
