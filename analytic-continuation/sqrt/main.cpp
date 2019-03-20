#include <iostream>
#include <fstream>
#include <vector>

#include "../rungekutta4.hpp"
#include "../outputimpl.hpp"

#include "sqrt.hpp"

class IntegrateOverPath: public Output
{
  std::vector<std::complex<double> > m_coords;
  std::vector<std::complex<double> > m_func;
public:
  virtual void write(const Coord& c, const function& i, unsigned int Step)
  {
    m_coords.push_back(std::complex<double>(c.x(),c.y()));
    m_func.push_back(std::complex<double>(i.real()[0],i.imag()[0]));
  }
  static std::complex<double> csqrt(std::complex<double> z)
  {
    return std::complex<double>(std::sqrt((abs(z)+z.real())/2.), std::sqrt((abs(z)-z.real())/2.));
  }
  std::complex<double> value()
  {
    std::complex<double> sum(0.,0.);
    std::complex<double> analyticalSum(0.,0.);

    for(unsigned int k(0);k<m_func.size()-1;++k)
      {
	std::clog << m_func[k] << " " << csqrt(m_coords[k]) << " "  << analyticalSum << std::endl;
	sum += m_func[k]*(m_coords[k+1]-m_coords[k]);
	analyticalSum += csqrt(m_coords[k])*(m_coords[k+1]-m_coords[k]);
      }
    std::clog << "analytical: " << analyticalSum << std::endl;
    return sum;
  }
};

class ShiftedPolarCoord: public Coord
{
  double m_r, m_t;
  static const double m_dx, m_dy;

public:
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

const double ShiftedPolarCoord::m_dx(1.);
const double ShiftedPolarCoord::m_dy(5.);

int main()
{
  std::cout.precision(15);
  n_range r(0.5, 1, 20);
  //n_range y(0.,10., 200);
  n_range theta(0.,4.*M_PI,400); 

  MyProblem p;
  RungeKutta4<MyProblem> rk4(p);
  RangeConsoleOutput out(theta.size(),1);

  for(unsigned int k(0); k<=r.size();++k)
    {

      const function ic(std::valarray<double>(sqrt(r[k]), 1),
			std::valarray<double>(0.,      1));
      
      //rk4.PolarIntegration(r[k], theta, ic, out);
      rk4.GenericCoordinatesIntegration<RangeConsoleOutput, PolarCoord>(r[k], theta, ic, out);
      //rk4.GenericCoordinatesIntegration<RangeConsoleOutput, CartCoord>(r[k], y, ic, out);
    }
  //std::clog << "Done, let's take a merry-go-round."<< std::endl;

  /*double rad(1.5);
  n_range straight_path(0,5, 200);
  n_range round_path(0,2*M_PI,20);
  function ic(std::valarray<double>(sqrt(rad), 1),
		    std::valarray<double>(0.,      1));
  
  std::ofstream file("path.dat", std::ios::out);
  CartesianFluxOutput fileOut(file);

  IntegrateOverPath intOut;

  //ic = rk4.GenericCoordinatesIntegration<CartesianFluxOutput, CartCoord>(rad, straight_path, ic, fileOut);
  //rk4.GenericCoordinatesIntegration<IntegrateOverPath, ShiftedPolarCoord>(0.5, round_path, ic, intOut);
  //rk4.GenericCoordinatesIntegration<CartesianFluxOutput, ShiftedPolarCoord>(0.5, round_path, ic, fileOut);
  
  std::clog << "Value of the integral: " << intOut.value() << std::endl;*/
  
  return 0;
}
