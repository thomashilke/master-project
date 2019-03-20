#include <iostream>
#include <vector>
#include <fstream>

#include "../../utility/cmdline.hpp"
#include "../rungekutta4.hpp"
#include "../outputimpl.hpp"

#include "monopole.hpp"

double LoadDatafile(const std::string& filename, std::vector<double>& F, std::vector<double>& H)
{
  F.clear();
  H.clear();
  std::ifstream ifs(filename.c_str(), std::ios::in);
  double r(0.), f(0.), h(0.);
  while(ifs)
    {
      ifs >> r >> f >> h;
      F.push_back(f);
      H.push_back(h);
    }
  std::clog << "Loaded: " << filename << " (" << F.size() << " entries, Rmax: " << r << ")" << std::endl;
  return r;
}

void ComputeDerivative(const std::vector<double>& fnc, std::vector<double>& deriv, n_range radius)
{
  double h(radius.h());

  deriv.clear();
  deriv.resize(fnc.size());
  deriv[0] = (fnc[1]-fnc[0])/h;
  for(unsigned int i(1);i<fnc.size()-1;++i)
    deriv[i] = (fnc[i+1]-fnc[i-1])/(2.*h);
  deriv.back() = (fnc.back()-fnc[fnc.size()-2])/h;
}

int main(int argc, char** argv)
{
  CmdLine cmd(argc,argv);
  std::string filename(cmd.Get<std::string>("sol")[0]);

  std::cout.precision(15);

  std::vector<double> SolutionOnTheRealLineF;
  std::vector<double> DerivativeOnTheRealLineF;

  std::vector<double> SolutionOnTheRealLineH;
  std::vector<double> DerivativeOnTheRealLineH;

  double R(LoadDatafile(filename, SolutionOnTheRealLineF, SolutionOnTheRealLineH));
  unsigned int rSteps(SolutionOnTheRealLineF.size()-1);
  unsigned int tSteps(cmd.Get<unsigned int>("steps")[0]);
  unsigned int Stride(cmd.Get<unsigned int>("stride")[0]);

  n_range radius(0,R,rSteps);
  n_range theta(0.,cmd.Get<double>("theta")[0],tSteps);
  //n_range theta(0.,3.*M_PI/4.,tSteps);
      
  ComputeDerivative(SolutionOnTheRealLineF, DerivativeOnTheRealLineF, radius);
  ComputeDerivative(SolutionOnTheRealLineH, DerivativeOnTheRealLineH, radius);
  
  std::valarray<double> initialrealpart(4);
  
  MyProblem p(cmd.Get<double>("lambda")[0],1.,1.);
  RungeKutta4<MyProblem> rk4(p);
  RangeConsoleOutput out(tSteps, Stride);
  
  unsigned int increment(cmd.Get<unsigned int>("incr")[0]);
  double rmax(cmd.Get<unsigned int>("r")[0]);
  for(unsigned int k(0);k<=rSteps && radius[k] < rmax; k+=increment)
    {
      initialrealpart[0] = SolutionOnTheRealLineF[k]; //Ar
      initialrealpart[1] = DerivativeOnTheRealLineF[k];//Br
      initialrealpart[2] = SolutionOnTheRealLineH[k];  //Fr
      initialrealpart[3] = DerivativeOnTheRealLineH[k];//Gr
      
      const function ic(initialrealpart,
			std::valarray<double>(0., 4));
      
      rk4.GenericCoordinatesIntegration<RangeConsoleOutput, PolarCoord>(radius[k], theta, ic, out);
    }

return 0;
}
