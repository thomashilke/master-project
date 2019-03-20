#include <iostream>
#include <cmath>

class range
{
  double m_min, m_max;
  unsigned int m_N;
public:
  range(double min, double max, unsigned int N): m_min(min),
						 m_max(max),
						 m_N(N)
  {}
  
  double operator[](unsigned int i)
  {
    if(i > m_N)
      throw std::string("range::operator[] - Out of range");
    return m_min+ i*(m_max-m_min)/m_N;
  }
};

int main()
{
  unsigned int N(200);
  double R(20.);
  range x(-R,R,N);
  range y(-R,R,N);
  
  for(unsigned int i(0); i<N;++i)
    {
      for(unsigned int j(0); j<N;++j)
	std::cout << x[i] << " " 
		  << y[j] << " " 
		  << cos(sqrt(x[i]*x[i]+y[j]*y[j])) << std::endl;
      std::cout << std::endl;
    }    
  return 0;
}
