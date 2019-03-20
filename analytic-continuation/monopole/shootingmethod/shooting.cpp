#include <iostream>
#include <vector>
#include <valarray>

#include "../../../utility/cmdline.hpp"

#include "rungekutta4.hpp"
#include "shooting.hpp"

#include "heatequation.hpp"
#include "vortexequation.hpp"
#include "monopoleequation.hpp"
#include "verbinequation.hpp"

void LoadInitParams(const std::string& filename, Matrix<double,0>& x)
{
  std::ifstream file(filename.c_str(), std::ios::in);
  for(unsigned int k(0);k<x.getRows();++k)
    file >> x(k,0);
}

template<typename Problem>
void MultipleShootingMethod(const Problem& equ, const BoundaryConditions& bc, unsigned int NIntervals,
			    const std::string& paramfile, std::ofstream& outfunction, std::ofstream& outparam)
{
  MultipleShootFunction<Problem, RungeKutta4<Problem> > msf(equ,bc,NIntervals);
  Matrix<double,0> param(msf.size(),1);
  /*for(unsigned int i(0);i<equ.size()*(NIntervals-1)+bc.size();++i)
    param(i,0) = 1. *((i+1)%2?1:0);*/
  
  LoadInitParams(paramfile, param);
  ParallelMNewton(msf,param);  

  msf.integrate(param, outfunction);
  outparam << param;
}

int main(int argc, char** argv)
{
  CmdLine cmd(argc,argv);

  std::string outfunctionname(cmd.Get<std::string>("outf")[0]);
  std::string outparamname(cmd.Get<std::string>("outp")[0]);
  std::string inparamname(cmd.Get<std::string>("inp")[0]);

  double chi(cmd.Get<double>("chi")[0]);
  double xi(cmd.Get<double>("xi")[0]);
  double r(cmd.Get<double>("r")[0]);
  unsigned int nintervals(cmd.Get<double>("n")[0]);

  std::ofstream outfunction(outfunctionname.c_str(), std::ios::out);
  std::ofstream outparam(outparamname.c_str(), std::ios::out);

  VerbinEquation p(chi,xi);
  VerbinBC bc(r);

  /*MonopoleEquation equ(1.,1.,1.);
    MonopoleBC bc(10.);*/
  
  /*VortexEquation equ(1.,1.,1.);
    VortexBC bc(10.);*/
  
  /*HeatEquation equ(1.);
    HeatBC bc(0.,0.,4.,1.);*/
  
  try
    { MultipleShootingMethod(p, bc, nintervals, inparamname, outfunction, outparam); }
  catch(std::string& e)
    {std::clog << "An exception occured: " << e << std::endl;}
  return 0;
}
