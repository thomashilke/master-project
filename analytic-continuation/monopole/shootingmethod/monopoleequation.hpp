class MonopoleEquation
{
  double g, v, lambda;
public:
  MonopoleEquation(double g, double v, double lambda): g(g),v(v),lambda(lambda)
  {}
  state operator()(double x, const state& y) const
  {
    state r(4);
    
    r[0] = y[1];
    r[1] = -2.*y[1]/x+2.*y[0]*(y[2]-1.)*(y[2]-1.)/(x*x)+lambda*v*v*y[0]*(y[0]*y[0]-1.);
    r[2] = y[3];
    r[3] = y[2]*(y[2]-1.)*(y[2]-2.)/(x*x)+g*g*v*v*y[0]*y[0]*(y[2]-1.);

    return r;
  }

  unsigned int size() const {return 4;}
};

class MonopoleBC: public BoundaryConditions
{
  double xa,xb;
  double Ha,Hb;
  double Fa,Fb;
  
  double t;
public:
  MonopoleBC(double Rmax): xa(1.e-3),xb(Rmax),
			   Ha(1.e-3), Hb(1.),
			   Fa(0.), Fb(1.),
			   t(1.)
  {}
  
  state lowerPoint(const Matrix<double,0>& x) const
  {
    state y(4);
    
    y[0] = Ha;
    y[1] = x(0,0);
    y[2] = Fa;
    y[3] = x(1,0);
    
    return y;
  }

  Matrix<double,0> higherPoint(const state& y) const
  {
    Matrix<double,0> x(2,1);
    
    x(0,0) = y[0]-(Ha+(Hb-Ha)*t);
    x(1,0) = y[2]-(Fa+(Fb-Fa)*t);

    return x;
  }
  
  double a() const { return xa; }
  double b() const { return xa+(xb-xa)*t;}
  void setT(double t_){t = t_;}
  
  virtual unsigned int size() const {return 2;}
};

