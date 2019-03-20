#include <cmath>
#include <iomanip>
#include <sstream>

#include "utils.hpp"

unsigned int max(std::vector<double>& v)
{
  unsigned int idx(0);
  double max(v.front());
  for(unsigned int i(0);i<v.size();++i)
    if(v[i]>max)
      {max = v[i]; idx = i;}
  return idx;
}
unsigned int min(std::vector<double>& v)
{
  unsigned int idx(0);
  double min(v.front());
  for(unsigned int i(0);i<v.size();++i)
    if(v[i]<min)
      {min = v[i]; idx = i;}
  return idx;
}
double avg(std::vector<double>& v)
{
  double sum(0.);
  for(unsigned int i(0);i<v.size();++i)
    sum += v[i];
  return sum/v.size();
}
double std_dev(std::vector<double>& v)
{
  double average(avg(v));
  double sum(0.);
  
  for(unsigned int i(0);i<v.size();++i)
    sum += (v[i] - average)*(v[i] - average);
  
  return sqrt(sum/(v.size()-1)); // see http://en.wikipedia.org/wiki/Standard_deviation
}


Progress::Progress(double min,
		   double max,
		   unsigned int increments): m_Min(min),
					     m_Max(max),
					     m_IncrNumber(increments),
					     current(0)
{}

void Progress::UpdateProgress(std::ostream& Flux, double new_progress)
{
  if(new_progress > current*(m_Max-m_Min)/m_IncrNumber+m_Min)
    {
      Flux << static_cast<unsigned int>((new_progress-m_Min)/(m_Max-m_Min)*100.) << "% " << std::flush;
      ++current;
    }
}

void Progress::Finish(std::ostream& Flux)
{
  Flux << "100% " << std::flush;
}

//détermine si tous les patterns sont correctement stockés dans le
//réseau de neurones:
bool SafelyStored(HopfieldModel& hfm, 
		  std::vector<PatternT>& patterns, 
		  std::vector<PatternT>& dist_patterns,
		  MTRand& rng)
{
  double Tolerance(0.05);

  bool failed(false);
  for(unsigned int i(0); i<patterns.size() && !failed; ++i)
    {
      PatternT fp;
      if(FixedPoint(hfm, dist_patterns[i], fp) == 20)
	failed = true;
      if(static_cast<double>(dist(fp,patterns[i]))
	 /static_cast<double>(hfm.NeuronCount()) > Tolerance)
	failed = true;
    }
  //Si la boucle s'est deroulee jusqu'au bout, renvoie !false ( = true):
  return !failed;
}

//Essaye de stocker autant de patterns que possible dans un reseau de
//neurones, jusqu'a ce que ca ne marche plus:
unsigned int Alpha(HopfieldModel& hfm, MTRand& rng)
{
  std::vector<PatternT> ps;
  std::vector<PatternT> dist_ps;

  do{
    ps.push_back(GenerateRandomPattern(hfm.NeuronCount(), rng));
    dist_ps.push_back(ps.back());
    AlterPattern(dist_ps.back(), 0.1, rng);
    hfm.Train(ps);
  }while(SafelyStored(hfm,ps,dist_ps, rng));
  
  //La boucle s'est terminee au moment ou on a dépassé le nombre
  //maximal de pattern. Donc ps contient un pattern de trop.
  return ps.size()-1;
}

std::string MakeFilename()
{
  std::ostringstream oss;
  oss << "data_";
  time_t t;
  time(&t);
  tm* ti(localtime(&t));
  oss << ti->tm_year+1900 
      << "." << std::setw(2) << std::setfill('0') << ti->tm_mon 
      << "." << std::setw(2) << std::setfill('0') << ti->tm_mday 
      << "-" << std::setw(2) << std::setfill('0') << ti->tm_hour 
      << "." << std::setw(2) << std::setfill('0') << ti->tm_min 
      << "." << std::setw(2) << std::setfill('0') << ti->tm_sec;
  oss << ".dat";
  return oss.str();
}
