#ifndef RUNGEKUTTA4
#define RUNGEKUTTA4

#include <iostream>
#include <valarray>
#include <complex>

#include "coord.hpp"

typedef std::complex<std::valarray<double> > function;

#include "output.hpp"

struct CoordinateAdaptorBase
{
  virtual function operator()(const function& f,
			      const Coord& c) const = 0;
};

/*struct CartesianCoordinateAdaptor: public CoordinateAdaptorBase
{
  virtual function operator()(const function& f,
			      const Coord&) const
  { return f; }
};

struct PolarCoordinateAdaptor: public CoordinateAdaptorBase
{
  function operator()(const function& f,
		      const Coord& c) const
  { return function(c.r()*(std::cos(c.t())*f.real()
			   -std::sin(c.t())*f.imag()),
		    c.r()*(std::sin(c.t())*f.real()
			   +std::cos(c.t())*f.imag())); }
			   };*/

struct GenericCoordinatesAdaptor: public CoordinateAdaptorBase
{
  function operator()(const function& f,
		      const Coord& c) const
  { return function(c.dxdnu()*f.imag()-c.dydnu()*f.real(),
		    c.dxdnu()*f.real()+c.dydnu()*f.imag()); }
};

template<typename Problem>
class RungeKutta4
{
  Problem& m_p;
public:
  RungeKutta4(Problem& p): m_p(p){}
  
  template<typename CoordSyst, typename CoordinateAdaptor>
  function delta(const function& y, double alpha,
		 double beta, double dbeta) const
  {
    std::valarray<double> half(0.5, y.real().size());
    std::valarray<double> two(2., y.real().size());
    std::valarray<double> onesixth(1./6., y.real().size());
    std::valarray<double> h(dbeta, y.real().size());
    
    function k1(h*m_p.template f<CoordinateAdaptor>(y,CoordSyst(alpha,beta)));
    function k2(h*m_p.template f<CoordinateAdaptor>(y+half*k1, CoordSyst(alpha,beta + 0.5*dbeta)));
    function k3(h*m_p.template f<CoordinateAdaptor>(y+half*k2, CoordSyst(alpha,beta + 0.5*dbeta)));
    function k4(h*m_p.template f<CoordinateAdaptor>(y+k3,     CoordSyst(alpha,beta+dbeta)));

    return onesixth*(k1+k2*two+k3*two+k4);
  }

  /*template<typename Output>
  void PolarIntegration(double radius,
			const n_range& theta,
			function ic,
			Output& out) const
  {
    for(unsigned int k(0); k <= theta.size(); ++k)
      {
	out.write(PolarCoord(radius,theta[k]), ic, k);
	ic += delta<PolarCoord, PolarCoordinateAdaptor>(ic,radius,theta[k], theta.h());
      }
      }*/

  template<typename Output, typename CoordSyst>
  function GenericCoordinatesIntegration(double mu,
				    const n_range& nu,
				    function ic,
				    Output& out) const
  {
    out.write(CoordSyst(mu,nu[0]), ic, 0);
    for(unsigned int k(0); k < nu.size(); ++k)
      {
	ic += delta<CoordSyst, GenericCoordinatesAdaptor>(ic, mu, nu[k], nu.h());
	out.write(CoordSyst(mu,nu[k+1]), ic, k+1);
      }
    return ic;
  }

  /*void CartesianIntegration(double x,
			    const n_range& y,
			    function ic,
			    Output& out) const
  {
    for(unsigned int k(0); k <= y.size(); ++k)
      {
	out.write(CartCoord(x,y[k]), ic, k);
	ic += delta<CartCoord, CartesianCoordinateAdaptor>(ic,x,y[k],y.h());
      }
      }*/
};




#endif
