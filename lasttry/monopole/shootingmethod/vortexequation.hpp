class VortexEquation
{
  double e, v, lambda;
public:
  VortexEquation(double e, double v, double lambda): e(e),v(v),lambda(lambda){}
  state operator()(double x, const state& y) const
  {
    state r(4);
    
    r[0] = y[1];
    r[1] = y[1]/x-2*e*e*v*v*y[2]*y[2]*(1.-y[0]);
    r[2] = y[3];
    r[3] = -y[3]/x+lambda*v*v*y[2]*(y[2]*y[2]-1.)+y[2]/(x*x)*(1-y[0])*(1-y[0]);

    return r;
  }

  unsigned int size() const {return 4;}
};

class VortexBC: public BoundaryConditions
{
  double xa,xb;
  double Aa,Ab;
  double Fa,Fb;
  
  double t;
public:
  VortexBC(double Rmax): xa(1e-3), xb(Rmax), Aa(0.), Ab(1.), Fa(1e-3), Fb(1.), t(1.){}
  
  state lowerPoint(const Matrix<double,0>& x) const
  {
    state y(4);
    
    y[0] = Aa;
    y[1] = x(0,0);
    y[2] = Fa;
    y[3] = x(1,0);
    
    return y;
  }

  Matrix<double,0> higherPoint(const state& y) const
  {
    Matrix<double,0> x(2,1);
    
    x(0,0) = y[0]-(Aa+(Ab-Aa)*t);
    x(1,0) = y[2]-(Fa+(Fb-Fa)*t);

    return x;
  }
  
  double a() const { return xa; }
  double b() const { return xa+(xb-xa)*t;}
  void setT(double t_){t = t_;}
  
  virtual unsigned int size() const {return 2;}
};

