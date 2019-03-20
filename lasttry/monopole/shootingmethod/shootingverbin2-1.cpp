#include <iostream>
#include <vector>
#include <valarray>

#include "../../../utility/cmdline.hpp"

#include "rungekutta4.hpp"
#include "shooting.hpp"

#include "heatequation.hpp"
#include "vortexequation.hpp"
#include "monopoleequation.hpp"
#include "verbinequation2-1.hpp"

void LoadInitParams(const std::string& filename, Matrix<double,0>& x)
{
  std::ifstream file(filename.c_str(), std::ios::in);
  std::vector<double> y;
  double tmp(0.);

  do{
    file >> tmp;
    if(file)
      y.push_back(tmp);
  }while(file);

  x.Resize(y.size(),1);
  for(unsigned int k(0);k<x.getRows();++k)
    x(k,0) = y[k];
}

void GenInitParams(unsigned int size, Matrix<double,0>& x)
{
  x.Resize(size,1);
  for(unsigned int k(0);k<size;++k)
    x(k,0) = ((k+1)%2?1.:0.)*1.+0.;
}

template<typename Problem>
void MultipleShootingMethod(const Problem& equ, const BoundaryConditions& bc, unsigned int NIntervals,
			    Matrix<double,0> param, std::ofstream& outfunction, std::ofstream& outparam,
			    unsigned int NJobs)
{
  MultipleShootFunction<Problem, RungeKutta4<Problem> > msf(equ,bc,NIntervals);
  if(param.getRows() != msf.size())
    throw std::string("MultipleShootingMethod() - Sizes mismatch.");

  ParallelMNewton(msf,param, NJobs);  

  msf.integrate(param, outfunction);
  outparam << param;
}

int main(int argc, char** argv)
{
  CmdLine cmd(argc,argv);

  unsigned int threads(cmd.Get<unsigned int>("threads")[0]);

  std::string outfunctionname(cmd.Get<std::string>("outf")[0]);
  std::string outparamname(cmd.Get<std::string>("outp")[0]);
  std::string inparamsource(cmd.Get<std::string>("inpsource")[0]);

  double chi(cmd.Get<double>("chi")[0]);
  double xi(cmd.Get<double>("xi")[0]);
  double r(cmd.Get<double>("r")[0]);
  unsigned int nintervals(cmd.Get<double>("n")[0]);

  std::ofstream outfunction(outfunctionname.c_str(), std::ios::out);
  std::ofstream outparam(outparamname.c_str(), std::ios::out);

  Verbin21Equation p(chi,xi);
  Verbin21BC bc(r);

  Matrix<double,0> x(1,1);
  if(inparamsource == "file")
    {
      std::string inparamname(cmd.Get<std::string>("inpname")[0]);
      LoadInitParams(inparamname, x);
    }
  else if(inparamsource == "gen")
    {
      unsigned int inparamsize(cmd.Get<unsigned int>("inpsize")[0]);
      GenInitParams(inparamsize, x);
    }

  /*MonopoleEquation equ(1.,1.,1.);
    MonopoleBC bc(10.);*/
  
  /*VortexEquation equ(1.,1.,1.);
    VortexBC bc(10.);*/
  
  /*HeatEquation equ(1.);
    HeatBC bc(0.,0.,4.,1.);*/
  
  try
    { MultipleShootingMethod(p, bc, nintervals, x, outfunction, outparam, threads); }
  catch(std::string& e)
    {std::clog << "An exception occured: " << e << std::endl;}
  return 0;
}
