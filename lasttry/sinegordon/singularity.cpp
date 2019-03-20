#include <iostream>
#include <vector>

#include "../rungekutta4.hpp"
#include "../outputimpl.hpp"

#include "sinegordon.hpp"

std::complex<double> atan(const std::complex<double>& c)
{
  return 0.5*std::complex<double>(0.,1.)*(log(1.-std::complex<double>(0.,1.)*c)
					  -log(1.+std::complex<double>(0.,1.)*c));
}

class IntegrateOverPath: public Output
{
  std::vector<std::complex<double> > m_coords;
  std::vector<std::complex<double> > m_func;
public:
  virtual void write(const Coord& c, const function& i, unsigned int Step)
  {
    m_coords.push_back(std::complex<double>(c.x(),c.y()));
    m_func.push_back(std::complex<double>(i.real()[0],i.imag()[0]));
    
    std::cout << c.x() << " " << c.y() << " "
	      << i.real() << " " << i.imag() << std::endl;
  }

  std::complex<double> value()
  {
    std::complex<double> sum(0.,0.);
    std::complex<double> asum(0.,0.);
    for(unsigned int k(0);k<m_func.size()-1;++k)
      {
	sum += m_func[k]*(m_coords[k+1]-m_coords[k]);
	asum += atan(std::exp(m_coords[k+1]))*(m_coords[k+1]-m_coords[k]);
      } 
    std::clog << "Analytical Sum: " << asum << std::endl;
    std::clog << "Numerical Sum: " << sum << std::endl;
    return sum;
  }
};

class ShiftedPolarCoord: public Coord
{
  double m_r, m_t;

public:
  static const double m_dx, m_dy;

  ShiftedPolarCoord(double _r, double _t): m_r(std::abs(_r)), m_t(_t){}
  ShiftedPolarCoord(const Coord& c): m_r(c.r()), m_t(c.t()) {}
  virtual double r() const { throw 0; }
  virtual double t() const { throw 0; }
  virtual double x() const { return m_dx + m_r*std::cos(m_t); };
  virtual double y() const { return m_dy + m_r*std::sin(m_t); };

  virtual double dxdnu() const
  { return -m_r*std::sin(m_t); }
  virtual double dydnu() const
  { return m_r*std::cos(m_t); }
};

const double ShiftedPolarCoord::m_dx(0.);
const double ShiftedPolarCoord::m_dy(M_PI/2.-0.1);

int main()
{
  std::cout.precision(15);

  n_range r(0.1,15, 100);

  MyProblem p;
  RungeKutta4<MyProblem> rk4(p);

  double h(ShiftedPolarCoord::m_dy);
  unsigned int K(2);
  double rad(r[K]);

  n_range straight_path(0,h, 2000);
  n_range round_path(0,2*M_PI,2000);

  std::valarray<double> initialrealpart(2);
  initialrealpart[0] = 4.*std::atan(std::exp(r[K]));
  initialrealpart[1] = 4.*std::exp(r[K])/(std::exp(2.*r[K])+1.);
  
  function ic(initialrealpart,
	      std::valarray<double>(0.,      2));
  
  NullOutput nullOut;
  IntegrateOverPath intOut;

  ic = rk4.GenericCoordinatesIntegration<NullOutput, CartCoord>(rad, straight_path, ic, nullOut);
  rk4.GenericCoordinatesIntegration<IntegrateOverPath, ShiftedPolarCoord>(rad, round_path, ic, intOut);

  std::clog << "Value of the integral: " << intOut.value() << std::endl;  

  return 0;
}
