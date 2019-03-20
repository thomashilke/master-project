#ifndef OUTPUT
#define OUTPUT

class Output
{
public:
  //virtual void writeCartesian(const Coord&, const function& f) = 0;
  //virtual void writePolar(const Coord&, const function& i) = 0;
  virtual void write(const Coord& c, const function& i, unsigned int Step) = 0;
};

std::ostream& operator<<(std::ostream& flux, const std::valarray<double>& v)
{
  for(unsigned int i(0);i<v.size();++i)
    if(std::isnan(v[i])) flux << 0.0 << "\t";
    else flux << v[i] << "\t";
  return flux;
}

class NullOutput: public Output
{
public:
  virtual void write(const Coord& c, const function& i, unsigned int Steps){}
};

class FluxOutput: public Output
{
protected:
  std::ostream& m_flux;

  virtual void writeCartesian(const Coord& c,
			      const function& f)
  { m_flux << c.x() << " " << c.y() << " "
	   << f.real() << " " << f.imag() << " "; }
	   
  virtual void writePolar(const Coord& c,
			  const function& f)
  { m_flux << c.r() << " " << c.t() << " "
	   << f.real() << " " << f.imag() << " "; }
public:
  FluxOutput(std::ostream& flux): m_flux(flux){}
};

#endif
