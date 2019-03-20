Matrix<double,0> ModifiedShootingMethod()
{
  VortexEquation ve(1.,1.,1.);
  VortexBC vbc(4.);
  ShootFunction<VortexEquation> sf(ve,vbc);
  
  Matrix<double,0> param(2,1);
  param(0,0) = 0.;
  param(1,0) = 1.;
  
  unsigned int N(50);
  for(unsigned int k(1);k<=N;++k)
    {
      vbc.setT(k*1./(double)N);
      Newton(sf,param);
    }
  return param;
}

Matrix<double,0> ShootingMethod()
{
  VerbinEquation equ(1.,1.);
  VerbinBC bc(2.);
  ShootFunction<VerbinEquation> sf(equ,bc);
  
  Matrix<double,0> param(bc.size(),1);
  for(unsigned int i(0);i<bc.size();++i)
    param(i,0) = -0.0001;

  param(0,0) = 1.0562;
  param(1,0) = -0.00245;
  
  integrate(equ,bc,param);
  Newton(sf,param);
  integrate(equ,bc,param);
  
  return param;
}
