#include <vector>
#include <iostream>

#include "hopfield.hpp"


unsigned int max(std::vector<double>& v);
unsigned int min(std::vector<double>& v);
double avg(std::vector<double>& v);
double std_dev(std::vector<double>& v);

class Progress
{
  double m_Min;
  double m_Max;
  unsigned int m_IncrNumber;

  unsigned int current;
public:
  Progress(double min,
	   double max,
	   unsigned int increments);
  void UpdateProgress(std::ostream& Flux, double new_progress);
  void Finish(std::ostream& Flux);
};

unsigned int Alpha(HopfieldModel& hfm, MTRand& rng);
bool SafelyStored(HopfieldModel& hfm, 
		  std::vector<PatternT>& patterns, 
		  std::vector<PatternT>& dist_patterns, MTRand& rng);

std::string MakeFilename();
