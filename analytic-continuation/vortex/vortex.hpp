class MyProblem
{
  double m_e;
  double m_v;
  double m_lambda;

  std::valarray<double> F(const function& f,
			  const Coord& c) const
  {
    std::complex<double> alpha(f.real()[0], f.imag()[0]);
    std::complex<double> beta(f.real()[1], f.imag()[1]);
    
    std::complex<double> phi(f.real()[2], f.imag()[2]);
    std::complex<double> gamma(f.real()[3], f.imag()[3]);

    std::complex<double> z(c.x(), c.y());

    std::valarray<double> r(4);
    r[0] = beta.imag();
    r[1] = (beta/z-2.*m_e*m_e*m_v*m_v*phi*phi*(1.-alpha)).imag();
    r[2] = gamma.imag();
    r[3] = (-gamma/z+m_lambda*m_v*m_v*(phi*phi-1.)*phi+phi/(z*z)*(alpha-1.)*(alpha-1.)).imag();

    return r;
  }
  
  std::valarray<double> G(const function& f,
			  const Coord& c) const
  {
    std::complex<double> alpha(f.real()[0], f.imag()[0]);
    std::complex<double> beta(f.real()[1], f.imag()[1]);
    
    std::complex<double> phi(f.real()[2], f.imag()[2]);
    std::complex<double> gamma(f.real()[3], f.imag()[3]);

    std::complex<double> z(c.x(), c.y());

    std::valarray<double> i(4);
    i[0] = beta.real();;
    i[1] = (beta/z-2.*m_e*m_e*m_v*m_v*phi*phi*(1.-alpha)).real();
    i[2] = gamma.real();
    i[3] = (-gamma/z+m_lambda*m_v*m_v*(phi*phi-1.)*phi+phi/(z*z)*(alpha-1.)*(alpha-1.)).real();

    return i;
  }
  
public:
  MyProblem(double e, double v, double lambda): m_e(e), m_v(v), m_lambda(lambda){}
  template<typename CoordinateAdaptor>
  function f(const function& f,
	     const Coord& c) const
  {CoordinateAdaptor ca;
    return ca(function(F(f, c),
		       G(f, c)),
	      c);}
};
