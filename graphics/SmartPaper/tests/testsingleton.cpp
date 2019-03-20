#include <iostream>

#include <utils/singleton.hpp>


class A: public Singleton<A>
{
  friend class Singleton<A>;
  A(){std::cout << "Constructeur de A" << std::endl;}
  ~A(){std::cout << "Destructeur de A" << std::endl;}
public:

};

int main()
{
  A::Instance();

  A::Destroy();
  return 0;
}
