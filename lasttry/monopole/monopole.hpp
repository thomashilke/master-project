class MyProblem
{
  double m_g;
  double m_v;
  double m_lambda;

  std::valarray<double> Re(const function& f,
			  const Coord& c) const
  {
    std::complex<double> F(f.real()[0], f.imag()[0]);
    std::complex<double> G(f.real()[1], f.imag()[1]);
    
    std::complex<double> H(f.real()[2], f.imag()[2]);
    std::complex<double> I(f.real()[3], f.imag()[3]);

    std::complex<double> z(c.x(), c.y());

    std::valarray<double> r(4);
    r[0] = G.imag();
    r[1] = (F*(F*F-1.)/(z*z)+m_g*m_v*m_v*H*H*F/(z*z)).imag();
    r[2] = I.imag();
    r[3] = (2.*H*F*F/(z*z)+m_lambda*m_v*m_v*(H*(H*H-z*z))/(z*z)).imag();

    return r;
  }
  
  std::valarray<double> Im(const function& f,
			  const Coord& c) const
  {
    std::complex<double> F(f.real()[0], f.imag()[0]);
    std::complex<double> G(f.real()[1], f.imag()[1]);
    
    std::complex<double> H(f.real()[2], f.imag()[2]);
    std::complex<double> I(f.real()[3], f.imag()[3]);

    std::complex<double> z(c.x(), c.y());

    std::valarray<double> i(4);
    i[0] = G.real();
    i[1] = (F*(F*F-1.)/(z*z)+m_g*m_v*m_v*H*H*F/(z*z)).real();
    i[2] = I.real();
    i[3] = (2.*H*F*F/(z*z)+m_lambda*m_v*m_v*(H*(H*H-z*z))/(z*z)).real();

    return i;
  }
  
public:
  MyProblem(double lambda = 1., double g = 1., double v = 1.): m_g(g), m_v(v), m_lambda(lambda){}
  template<typename CoordinateAdaptor>
  function f(const function& f,
	     const Coord& c) const
  {CoordinateAdaptor ca;
    return ca(function(Re(f, c),
		       Im(f, c)),
	      c);}
};
