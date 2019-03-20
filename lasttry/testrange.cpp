#include <iostream>
#include "../coord.hpp"

std::ostream& operator<<(std::ostream& flux, const n_range& r)
{
  std::cout << "start: " << r.start() << std::endl;
  std::cout << "size: " << r.size() << std::endl;
  std::cout << "i_max: " << r.size()+r.start() << std::endl;

  for(int i(r.start());i <= static_cast<int>(r.size())+r.start();i += 1)
  
    std::cout << i << ": " << r[i] << std::endl;
  return flux;
}
int main()
{
  n_range r(0., -2, 10., 4);
  std::cout << r << std::endl;
  return 0;
}
