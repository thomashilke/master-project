#ifndef OUTPUTIMPL
#define OUTPUTIMPL

#include "output.hpp"

class ConsoleOutput: public FluxOutput
{
public:
  ConsoleOutput(): FluxOutput(std::cout) {}
  virtual void write(const Coord& c, const function&f, unsigned int /*Step*/)
  { FluxOutput::writeCartesian(c,f); m_flux << std::endl; }
};


class RangeConsoleOutput: public FluxOutput
{
  unsigned int m_lastStep;
  unsigned int m_stride;
public:
  RangeConsoleOutput(unsigned int lastStep,
		     unsigned int stride, std::ostream&  flux = std::cout): FluxOutput(flux),
					   m_lastStep(lastStep),
					   m_stride(stride)
  {}
  virtual void write(const Coord& c, const function&f, unsigned int Step)
  {
    if(Step%m_stride == 0)
      {
	FluxOutput::writeCartesian(c,f);
	m_flux << std::endl;
      }
    
    if(Step == m_lastStep) m_flux << std::endl;
  }
};

class PolarFluxOutput: public FluxOutput
{
public:
  PolarFluxOutput(std::ostream& flux): FluxOutput(flux)
  {}
  void write(const Coord& c, const function& f, unsigned int /*Steps*/)
  {FluxOutput::writePolar(c,f); m_flux << std::endl;}
};

class CartesianFluxOutput: public FluxOutput
{
public:
  CartesianFluxOutput(std::ostream& flux): FluxOutput(flux)
  {}
  void write(const Coord& c, const function& f, unsigned int /*Steps*/)
  {FluxOutput::writeCartesian(c,f); m_flux << std::endl;}
};


#endif
