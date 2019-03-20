#ifndef SINGLETON
#define SINGLETON

#include <string>

#include "smartptr.hpp"

template<typename Base>
class Singleton
{
  static Base* m_instance;

  Singleton(const Singleton<Base>&);
  Singleton<Base>& operator=(const Singleton<Base>&);

protected:
  Singleton(){}
  virtual ~Singleton(){}
public:
  static Base* Instance()
  {
    if(!m_instance)
      m_instance = new Base();
    return m_instance;
  }
  
  static void Destroy()
  { delete m_instance; m_instance = 0;}
};

template<typename Base>
Base* Singleton<Base>::m_instance(0);

#endif
