#include <vector>
#include <string>
#include <cmath>

class InterpolatingFunction
{
  std::vector<double> r;
  std::vector<double> f;
public:
  InterpolatingFunction(){}

  void addPoint(double x, double y)
  {
    r.push_back(x);
    f.push_back(y);
  }

  unsigned int findIndex(double x, unsigned int lower, unsigned int higher)
  {
    if((higher-lower)==1)
      return lower;

    unsigned mid((higher-lower)/2+lower);
    if(r[mid]<x)
      return findIndex(x,mid,higher);
    else
      return findIndex(x,lower,mid);
  }
  
  double operator()(double x)
  {
    if(r.size()<2)
      throw std::string("InterpolatingFunction::operator() - Undefined.");

    if(r.front()>x)
      return f.front();
    if(r.back()<x)
      return f.back();
    
    unsigned int i(findIndex(x, 0, r.size()-1));
    double frac((x-r[i])/(r[i+1]-r[i]));
    return f[i]+frac*(f[i+1]-f[i]);
  }
  
};
