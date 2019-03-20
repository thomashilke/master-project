class HeatEquation
{
  double alpha;
  
  double source(double x) const
  {
    double x0(0.5);
    double sigma(0.1);
    return 20*std::sin(2*M_PI*x);//20.*std::exp(-(x-x0)*(x-x0)/(2*sigma*sigma));
  }

public:
  HeatEquation(double diffCoeff): alpha(diffCoeff){}
  state operator()(double x, const state& y) const
  {
    state f(2);
    
    f[0] = y[1];
    f[1] = -source(x)/alpha;
    
    return f;
  }
  unsigned int size() const {return 2;}
};

class HeatBC: public BoundaryConditions
{
  double xa,xb;
  double ya, yb;

  double t;
public:
  HeatBC(double xa, double ya, double xb, double yb): xa(xa), ya(ya),xb(xb),yb(yb), t(1.){}

  state lowerPoint(const Matrix<double,0>& x) const
  {
    state y(2);
    y[0] = ya;
    y[1] = x(0,0);
    return y;
  }
  Matrix<double,0> higherPoint(const state& x) const
  {
    Matrix<double,0> ret(1,1);
    ret(0,0) = x[0]-(ya+(yb-ya)*t);
    return ret;
  }
  double a() const {return xa;}
  double b() const {return xa+(xb-xa)*t;}

  unsigned int size() const {return 1;}

  void setT(double t_){t = t_;}
};
