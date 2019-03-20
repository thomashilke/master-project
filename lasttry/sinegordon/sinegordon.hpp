class MyProblem
{
  std::valarray<double> F(const function& f,
			  const Coord&) const
  {
    std::complex<double> alpha(f.real()[0], f.imag()[0]);
    std::complex<double> beta(f.real()[1], f.imag()[1]);

    std::valarray<double> r(2);
    r[0] = beta.imag();
    r[1] = std::cos(alpha.real())*std::sinh(alpha.imag());

    return r;
  }
  
  std::valarray<double> G(const function& f,
			  const Coord&) const
  {
    std::complex<double> alpha(f.real()[0], f.imag()[0]);
    std::complex<double> beta(f.real()[1], f.imag()[1]);

    std::valarray<double> i(2);
    i[0] = beta.real();
    i[1] = std::sin(alpha.real())*cosh(alpha.imag());

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
