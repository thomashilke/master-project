#include <iostream>
#include <vector>
#include <string>

#include "rungekutta4.hpp"

std::ostream& operator<<(std::ostream& flux, const state& y)
{
  for(unsigned int k(0);k<y.size();++k)
    flux << y[k] << " ";
  return flux;
}

class HarmonicOscillator
{
public:
  state operator()(double /*t*/, const state& y)
  {
    double omega(2.*M_PI);
    state r(2);
    r[0] = y[1];
    r[1] = -omega*omega*y[0];
    return r;
  }
};

int main()
{
  state start(4);
  double t(0.);

  t = 0.;
  start[0] = 1.;
  start[1] = 0.;

  double dt(.5);
  HarmonicOscillator ho;

  Euler<HarmonicOscillator> eul(ho);
  RungeKutta4<HarmonicOscillator> rk4(ho);
  LobattoIIIa<HarmonicOscillator> l3a(ho);
  DormandPrince<HarmonicOscillator> dp(ho);


  std::cout.precision(15);  
  for(unsigned int k(0);k<9;++k)
    {
      state y1(start), y2(start), y3(start), y4(start);

      y1 = eul.step(t,y1,dt);
      y2 = rk4.step(t,y2,dt);
      y3 = l3a.step(t,y3,dt);
      y4 = dp.step(t,y4,dt);
      
      double f(std::cos(2.*M_PI*(t+dt)));
      std::cout << dt 
		<< " " << f
		<< " " << std::abs(y1[0]-f) 
		<< " " << std::abs(y2[0]-f)
		<< " " << std::abs(y3[0]-f)
		<< " " << std::abs(y4[0]-f)
		<< std::endl;
      dt /= 2.;
    }
  return 0;
}
