class Verbin21Equation
{
  double chi, xi;
public:
  Verbin21Equation(double chi, double xi): chi(chi), xi(xi)
  {}
  state operator()(double x, const state& y) const
  {
    state r(8);
    
    r[0] = y[1];
    r[1] = ( -y[1]*y[3]/y[2]
	     -chi*y[0]*( (1.-y[4]*y[4])*(1.-y[4]*y[4])/2.
			 -xi*y[7]*y[7]/(y[2]*y[2]) ) );
    r[2] = y[3];
    r[3] = ( -y[1]*y[3]/y[0]
	     -chi*y[2]*((1.-y[4]*y[4])*(1.-y[4]*y[4])/2.
			+y[4]*y[4]*y[6]*y[6]/(2.*y[2]*y[2])) );
    r[4] = y[5];
    r[5] = ( -y[5]*(y[1]/y[0]+y[3]/y[2])
	     +y[4]*(y[4]*y[4]-1.+y[6]*y[6]/(y[2]*y[2])) );
    r[6] = y[7];
    r[7] = ( -y[7]*(y[1]/y[0]-y[3]/y[2])
	     +y[4]*y[4]*y[6]/xi );

    return r;
  }

  unsigned int size() const {return 8;}
};

class Verbin21BC: public BoundaryConditions
{
  double xa,xb;
  double Alpha,dAlpha;
  double Beta,dBeta;
  double fa, fb;
  double Pa, Pb;
  
  double t;
public:
  Verbin21BC(double Rmax): xa(1.e-3),xb(Rmax),
			   Alpha(1.), dAlpha(xa),
			   Beta(xa), dBeta(1.),
			   fa(xa), fb(1.),
			   Pa(1.), Pb(0.),
			   t(1.)
  {}
  
  state lowerPoint(const Matrix<double,0>& x) const
  {
    //if(x.getRows() != size())
    //throw std::string("VerbinBC::lowerPoint(Matrix) - sizes doesn't match.");
    state y(8);
    
    y[0] = Alpha;
    y[1] = dAlpha;
    y[2] = Beta;
    y[3] = dBeta;
    y[4] = fa;
    y[5] = x(0,0);
    y[6] = Pa;
    y[7] = x(1,0);
    
    return y;
  }

  Matrix<double,0> higherPoint(const state& y) const
  {
    Matrix<double,0> x(size(),1);
    
    x(0,0) = y[4]-(fa+(fb-fa)*t);
    x(1,0) = y[6]-(Pa+(Pb-Pa)*t);

    return x;
  }
  
  double a() const { return xa; }
  double b() const { return xa+(xb-xa)*t;}
  //void setT(double t_){t = t_;}
  
  virtual unsigned int size() const {return 2;}
};

