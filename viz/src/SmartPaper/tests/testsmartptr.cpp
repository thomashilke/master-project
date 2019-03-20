#include <iostream>

#include <utils/smartptr.hpp>

template<typename T>
class A
{
public:
  typedef SmartPtr<A<T> > Ptr;
  A(){ std::cout << "Construction de A" << std::endl; }
  ~A(){ std::cout << "Destruction de A" << std::endl; }
  
  void Waouf(){ std::cout << "Waouf" << std::endl; }
};

int main()
{
  A<int>::Ptr p;
  p = new A<int>();
  {
    A<int>::Ptr p1(p);
    A<int>::Ptr p2;
    p2 = p1;
    
    p2->Waouf();
    (*p2).Waouf();

    A<int>* const a(p2.RawPtr());
    
    a->Waouf();
  }
  return 0;
}
