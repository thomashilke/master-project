class MyProblem
{
  std::valarray<double> F(const function& f,
			  const Coord&) const
  {
    std::complex<double> g(f.real()[0], f.imag()[0]);
    std::complex<double> h(f.real()[1], f.imag()[1]);
    std::valarray<double> r(2);
    r[0] = 0.5*g.imag()/norm(g);
    r[1] = std::exp(-h.real())*std::sin(h.imag());
    
    return r;
  }
  
  std::valarray<double> G(const function& f,
			  const Coord&) const
  {
    std::complex<double> g(f.real()[0], f.imag()[0]);
    std::complex<double> h(f.real()[1], f.imag()[1]);
    std::valarray<double> i(2);
    i[0] = 0.5*g.real()/norm(g);
    i[1] = std::exp(-h.real())*std::cos(h.imag());
    return i;
  }
  
public:

  template<typename CoordinateAdaptor>
  function f(const function& f,
	     const Coord& c) const
  {CoordinateAdaptor ca;
    return ca(function(F(f, c),
		       G(f, c)),
	      c);}
};
