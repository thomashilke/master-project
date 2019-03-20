class Verbin31Equation
{
  double alpha, gamma;
public:
  Verbin31Equation(double alpha, double gamma): alpha(alpha), gamma(gamma)
  {}
  state operator()(double x, const state& y) const
  {
    state r(8);
    
    r[0] = y[1];
    r[1] = (-y[1]*y[1]/y[0]-y[3]*y[1]/y[2]
	    + gamma*y[0]*(y[7]*y[7]/(2.*alpha*y[2]*y[2]) -1./4.*(1.-y[4]*y[4])*(1.-y[4]*y[4])));
    r[2] = y[3];
    r[3] = (-2.*y[1]*y[3]/y[0]
	    - gamma*(y[7]*y[7]/(2.*alpha*y[2]) + y[6]*y[6]*y[4]*y[4]/y[2] + 1./4.*y[2]*(1.-y[4]*y[4])*(1.-y[4]*y[4])));
    r[4] = y[5];
    r[5] = (-y[5]*(2.*y[1]/y[0]+y[3]/y[2])-y[4]*(1.-y[4]*y[4]-y[6]*y[6]/(y[2]*y[2])));
    r[6] = y[7];
    r[7] = (-y[7]*(2.*y[1]/y[0]-y[3]/y[2])+alpha*y[4]*y[4]*y[6]);

    return r;
  }

  unsigned int size() const {return 8;}
};

class Verbin31BC: public BoundaryConditions
{
  double xa,xb;
  double N,dN;
  double L,dL;
  double fa, fb;
  double Pa, Pb;
  
public:
  Verbin31BC(double Rmax): xa(1.e-4),xb(Rmax),
			   N(1.), dN(0.),
			   L(xa), dL(1.),
			   fa(0.), fb(1.),
			   Pa(1.), Pb(0.)
  {}
  
  state lowerPoint(const Matrix<double,0>& x) const
  {
    state y(8);
    
    y[0] = N;
    y[1] = dN;
    y[2] = L;
    y[3] = dL;
    y[4] = fa;
    y[5] = x(0,0);
    y[6] = Pa;
    y[7] = x(1,0);
    
    return y;
  }

  Matrix<double,0> higherPoint(const state& y) const
  {
    Matrix<double,0> x(size(),1);
    
    x(0,0) = y[4]-(fb);
    x(1,0) = y[6]-(Pb);

    return x;
  }
  
  double a() const { return xa; }
  double b() const { return xb;}
  virtual unsigned int size() const {return 2;}
};

