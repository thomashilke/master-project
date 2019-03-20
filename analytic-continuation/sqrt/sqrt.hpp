class MyProblem
{
  std::valarray<double> F(const function& f,
			  const Coord&) const
  {
    std::complex<double> g(f.real()[0], f.imag()[0]);
    return std::valarray<double>((0.5/g).imag(), 1);
  }
  
  std::valarray<double> G(const function& f,
			  const Coord&) const
  {
    std::complex<double> g(f.real()[0], f.imag()[0]);
    return std::valarray<double>((0.5/g).real(), 1);
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
