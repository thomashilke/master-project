#ifndef STRINGBUILDER
#define STRINGBUILDER

#include <sstream>

class StringBuilder
{
  std::ostringstream iss;
public:
  StringBuilder(){}
  template<typename T>
  StringBuilder(const T& m)
  {iss << m;}
  
  template<typename T>
  StringBuilder& operator()(const T& m)
  {iss << m; return *this;}
  
  std::string Str(){return iss.str();}
};


#endif
