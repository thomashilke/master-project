#ifndef COORD
#define COORD

#include <cmath>

class n_range
{
  double m_min;
  double m_max;
  unsigned int m_N;
  
  int m_start;
public:
  n_range(double min, double max, unsigned int N): m_min(min),
						   m_max(max),
						   m_N(N),
						   m_start(0)
  {}

  n_range(double min, int Start,
	  double max, unsigned int End): m_min(min),
					 m_max(max),
					 m_N(End-Start),
					 m_start(Start)
  {}
  
  inline double operator[](int i) const
  { return m_min+(i-m_start)*(m_max-m_min)/size(); }
  inline double h() const { return (m_max-m_min)/(m_N+1); }
  inline unsigned int size() const { return m_N; }
  inline int start() const {return m_start; }
};

std::ostream& operator<<(std::ostream& flux, const n_range& r)
{
  for(int i(r.start()); i<=(int)r.size()+r.start();++i)
    flux << i << ": " << r[i] << std::endl;
  return flux;
}

class Coord
{
public:
  virtual ~Coord(){}
  //virtual Coord& operator=(const Coord& c) = 0;
  virtual double r() const = 0;
  virtual double t() const = 0;
  virtual double x() const = 0;
  virtual double y() const = 0;
  
  virtual double dxdnu() const = 0;
  virtual double dydnu() const = 0;
};

class CartCoord: public Coord
{
  double m_x, m_y;
public:
  CartCoord(double _x, double _y): m_x(_x), m_y(_y) {}
  CartCoord(const CartCoord& c): m_x(c.x()), m_y(c.y()) {}
  //virtual CartCoord& operator=(const Coord& c){m_x = c.x(); m_y = c.y(); return *this;}
  virtual double r() const { return std::sqrt(m_x*m_x+m_y*m_y); }
  virtual double t() const { return std::atan2(m_y,m_x); }
  virtual double x() const { return m_x; }
  virtual double y() const { return m_y; }

  //nu is y in this case.
  virtual double dxdnu() const
  { return 0.; }
  virtual double dydnu() const
  { return 1.; }
};

class PolarCoord: public Coord
{
  double m_r, m_t;
public:
  PolarCoord(double _r, double _t): m_r(std::abs(_r)), m_t(_t) {}
  PolarCoord(const Coord& c): m_r(c.r()), m_t(c.t()) {}
  //virtual PolarCoord& operator=(const Coord& c){m_r = c.r(); m_t = c.t(); return *this;}
  virtual double r() const { return m_r; }
  virtual double t() const { return m_t; }
  virtual double x() const { return m_r*std::cos(m_t); };
  virtual double y() const { return m_r*std::sin(m_t); };

  virtual double dxdnu() const
  { return -m_r*std::sin(m_t); }
  virtual double dydnu() const
  { return m_r*std::cos(m_t); }
};


#endif
