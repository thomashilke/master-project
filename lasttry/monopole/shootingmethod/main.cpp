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

class Vortex
{
  double e,v,lambda;
public:
  Vortex(double e_, double v_, double lambda_):e(e_),v(v_),lambda(lambda_) {}

  state operator()(double t, const state& y)
  {
    const double &y1(y[0]), &y2(y[1]), &y3(y[2]), &y4(y[3]);
    
    state r(4);
    r[0] = y2;
    r[1] = y2/t-2.*e*e*v*v*y3*y3*(1.-y1);
    r[2] = y4;
    r[3] = -y4/t+lambda*v*v*y3*(y3*y3-1.)+y3/(t*t)*(1.-y1)*(1.-y1);
    return r;
  }
};

class DoubleHO
{
public:
  state operator()(double t, const state& y)
  {
    double omega(2.*M_PI);
    state r(4);
    r[0] = y[1];
    r[1] = -omega*omega*y[0];
    r[2] = y[3];
    r[3] = -omega*omega*y[2]/(t*t+1.);
    return r;
  }
};

int main()
{
  state start(4);
  double t(0.);

  t = 0.012430041152263;
  start[0] = 0.000076751938222;
  start[1] = 0.012429199600993;
  start[2] = 0.010535874586259;
  start[3] = 0.858619159268147;

  double dt(0.0002);
  Vortex vortex(1.,1.,1.);
  //RungeKutta4<Vortex> rk4(vortex);
  //LobattoIIIa<Vortex> l3a(vortex);
  DormandPrince<Vortex> dp(vortex);
  state y1(start),y2(start),y3(start);
  state e(0.,4);

  std::cout.precision(15);  
  for(unsigned int k(0);k<=10000;++k)
    {
      std::cout << t 
	//<< " " << y1 
		<< " " << y2  << " " << e
	//<< " " << y3 
		<< std::endl;
      //y1 = rk4.step(t,y1,dt);
      y2 = dp.step(t,y2,dt,&e);
      //y3 = l3a.step(t,y3,dt);
      t += dt;
    }
  return 0;
}
