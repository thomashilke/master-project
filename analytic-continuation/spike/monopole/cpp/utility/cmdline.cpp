#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>

#include "cmdline.hpp"


std::ostream& operator<<(std::ostream& Flux, const std::vector<double>& v)
{
  for(unsigned int i(0);i<v.size();++i)
    Flux << v[i] << " ";
  return Flux;
}

void Print(const Map& m)
{
  Map::const_iterator It(m.begin());
  while(It != m.end())
    {
      std::cout << "\""<<It->first<<"\""<< ": ";
      const Vec& v(It->second);
      for(unsigned int i(0);i<v.size();++i)
	std::cout << v[i] << " ";
      std::cout << std::endl;
      ++It;
    }
}



int main(int argc, char** argv)
{
  CmdLine C(argc, argv);

  std::vector<double> dt(C.Get<double>("dt"));

  std::cout << dt << std::endl;

  return 0;
}
