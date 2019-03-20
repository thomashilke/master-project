#include <iostream>
#include <vector>
#include <valarray>

#include "rungekutta4.hpp"
#include "shooting.hpp"

#include "heatequation.hpp"
#include "vortexequation.hpp"
#include "monopoleequation.hpp"
#include "verbinequation.hpp"

template<typename Problem, typename Stepper>
state IntegrateProblem(std::ostream& flux, const Problem& p,
		       double t, const state& y, double dt, unsigned int NSteps)
{
  Stepper s(p);
  state yn(y);
  for(unsigned int k(0);k<NSteps;++k, t += dt)
    {
      std::cout << t << " " << yn << std::endl;
      yn = s.step(t,yn,dt);
    }
  return yn;
}

template<typename Problem>
void IntegrateAdaptive(std::ostream& flux, const Problem& p,
		       double t, const state& y, double dt, double tmax)
{
  DormandPrince<Problem> dp(p,y,t,dt);
  do{
    dp.step();
    std::cout << dp.getTime() << " " << dp.getState() << std::endl;
  } while(dp.getTime() < tmax);
}

void MultipleShootingMethod()
{
  unsigned int NIntervals(5);

  VerbinEquation equ(1.,1.);
  VerbinBC bc(5.);
  MultipleShootFunction<VerbinEquation, LobattoIIIa<VerbinEquation> > sf(equ,bc,NIntervals);

  /*MonopoleEquation equ(1.,1.,1.);
  MonopoleBC bc(10.);
  MultipleShootFunction<MonopoleEquation> sf(equ,bc,NIntervals);*/

  /*VortexEquation equ(1.,1.,1.);
  VortexBC bc(10.);
  MultipleShootFunction<VortexEquation> sf(equ,bc,NIntervals);*/
  
  /*HeatEquation equ(1.);
  HeatBC bc(0.,0.,4.,1.);
  MultipleShootFunction<HeatEquation> sf(equ,bc,NIntervals);*/
  
  Matrix<double,0> param(equ.size()*(NIntervals-1)+bc.size(),1);
  for(unsigned int i(0);i<equ.size()*(NIntervals-1)+bc.size();++i)
    param(i,0) = 1. *(i%2?1:0);
  
  param(0,0) = 0.391247;
  param(1,0) = -0.00079873;
  
  sf.integrate(param, std::cout);

  Newton(sf,param);  
  sf.integrate(param, std::cout);
}


Matrix<double,0> ModifiedShootingMethod()
{
  VortexEquation ve(1.,1.,1.);
  VortexBC vbc(4.);
  ShootFunction<VortexEquation> sf(ve,vbc);
  
  Matrix<double,0> param(2,1);
  param(0,0) = 0.;
  param(1,0) = 1.;
  
  unsigned int N(50);
  for(unsigned int k(1);k<=N;++k)
    {
      vbc.setT(k*1./(double)N);
      Newton(sf,param);
    }
  return param;
}

Matrix<double,0> ShootingMethod()
{
  VerbinEquation equ(1.,1./6.);
  VerbinBC bc(2.);
  ShootFunction<VerbinEquation> sf(equ,bc);
  
  Matrix<double,0> param(bc.size(),1);
  for(unsigned int i(0);i<bc.size();++i)
    param(i,0) = -0.0001;

  param(0,0) = 1.0562;
  param(1,0) = -0.00245;
  
  integrate(equ,bc,param);
  Newton(sf,param);
  //integrate(equ,bc,param);
  
  return param;
}

int main()
{
  try
    {
      double init[] = {
	0.11194 ,
	-0.87133 ,
	0.27995 ,
	-0.83277 ,
	-0.08107 ,
	-0.37318 ,
	1.0646 ,
	-1.0354
      };
      state y(init,8);
      
      double t(0.001+14.*(5.-0.001)/15.);

      /*IntegrateProblem<VerbinEquation,
		       RungeKutta4<VerbinEquation> >(std::cout, VerbinEquation(1.,1.),
						     0.001+14.*(5.-0.001)/15.,
						     y,(5.-0.001)/(15.*400.*4),4*401);*/
      IntegrateAdaptive<VerbinEquation>(std::cout, VerbinEquation(1.,1.),
					t,
					y,0.01, t+0.1);
    }
  //{std::clog << ShootingMethod() << std::endl;}
  catch(std::string& e)
    {std::clog << "An exception occured: " << e << std::endl;}
  return 0;
}
