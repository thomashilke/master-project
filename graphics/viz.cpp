#include <iostream>
#include <fstream>

#include <renderer/renderer.hpp>
#include <utils/tuple.hpp>
#include <utils/transform.hpp>
#include <cmath>

#include "dampeddynamic.hpp"
/*
  the idea is to display a bunch of surfaces. Each surface representing a 
  dataset.
  
  Basically a surface is defined by a set of points organized in array.
  A point is caracterized by {three coordinates (three values),
  a color (three values).}
  
  We need an unified interface to generate a set of points. For example, we
  may want to plot a set of points from a datafile, generate if from a
  function, or even retrieved from an external plugin.
  
  Once we have the data, it may be needed to preprocess it, transform it, or
  select only some parts.
  
  Finally, we may choose a particular way to graphically represent this data:
  cartesian, polar, spherical coordinates, logarithmic axis, ranges, etc.


  The steps are:
   *  Acquire a set of points {c1,c2,c3,r,g,b}, which represent a set of bands.
       -  From two ranges (for s and t) which are mapped into 3d space, and a
          other function to define the color
       -  From a file, each of the componant of each point mapped from 
          each record of the file.
       -  Other ideas..
   *  Choose a mapping from the data coordinates to the cartesian coordinates
      {c1,c2,c3} -> {x,y,z}
   *  Choose the range of the data that are shown
   *  Generate a surface, and the axis.
 */

struct vertex
{
  float x,y,z;
  unsigned char r,g,b,a;
};

class Axis
{
  BasicRenderer& m_br;
  VertexBuffer<vertex>::Ptr m_a[3];

public:
  Axis(BasicRenderer& br):m_br(br)
  {
    VertexDeclaration decl[] = {{TYP_FLOAT3, USA_POSITION},
				{TYP_COLOR, USA_DIFFUSE}};
    
    vertex vbx[] = {{-1., 0., 0., 255,0,0,255},
		    { 1., 0., 0., 255,0,0,255}};
    vertex vby[] = {{0., -1., 0., 0,255,0,255},
		    {0.,  1., 0., 0,255,0,255}};
    vertex vbz[] = {{0., 0., -1., 0,0,255,255},
		    {0., 0.,  1., 0,0,255,255}};

    m_a[0] = br.CreateVertexBuffer<vertex>(2, decl, vbx);
    m_a[1] = br.CreateVertexBuffer<vertex>(2, decl, vby);
    m_a[2] = br.CreateVertexBuffer<vertex>(2, decl, vbz);
  }

  void Render()
  {
    m_br.PushMatrix(MAT_MODELVIEW);
    m_br.MultMatrix(MAT_MODELVIEW, Scale(vec3(10,10,10)));
    for(unsigned int k(0);k<3;++k)
      {
	m_br.setVertexBuffer<vertex>(m_a[k]);
	m_br.DrawPrimitives(PRI_LINES, 1);
      }
    m_br.PopMatrix(MAT_MODELVIEW);
  }
};

typedef std::vector<std::vector<Tuple<double, double, double> > > array;

inline double Clip(double z)
{ z /= 2.;
  
  double max(5.);
  return z>max?max:(z<-max?-max:z); 
}

void CopyPosition(vertex* ptr, const Tuple<double,double,double>& t)
{
  ptr->x = val<1>(t);
  ptr->y = val<2>(t);
  ptr->z = Clip(val<3>(t));
}

void ComputeColors(VertexBuffer<vertex>::Ptr& vb, vec3 lightDir)
{
  for(unsigned int tri(0);tri < vb->getSize()/3;++tri)
    {
      vec3 p1(vb->at(3*tri  )->x, vb->at(3*tri  )->y, vb->at(3*tri  )->z);
      vec3 p2(vb->at(3*tri+1)->x, vb->at(3*tri+1)->y, vb->at(3*tri+1)->z);
      vec3 p3(vb->at(3*tri+2)->x, vb->at(3*tri+2)->y, vb->at(3*tri+2)->z);
      
      vec3 n(CrossProduct(p2-p1, p3-p1));
      Normalize(n);
      Normalize(lightDir);
      double frac(DotProduct(n,lightDir));

      if(frac < 0.1) frac = 0.1;
      
      for(unsigned int i(0);i<3;++i)
	{
	  vb->at(3*tri +i)->r = 255*frac;
	  vb->at(3*tri +i)->g = 255*frac;
	  vb->at(3*tri +i)->b = 255*frac;
	  vb->at(3*tri +i)->a = 255;
	}
    }
}

class ColorPalette
{
  std::vector<Tuple<double, Color> > m_stops;
public:
  ColorPalette(){}
  
  void addStop(double level, Color c)
  { m_stops.push_back(MakeTuple(level, c)); }
  Color getColor(double level)
  {
    if(m_stops.empty())
      throw std::string("ColorPalette::getColor() - empty stops.");

    std::vector<Tuple<double,Color> >::iterator it(m_stops.begin());
    if(val<1>(*it)>level)
      return val<2>(*it);

    while(it != m_stops.end() && val<1>(*it)<level) ++it;

    if(it == m_stops.end())
      return val<2>(m_stops.back());
    
    double l2(val<1>(*it));
    Color c2(val<2>(*it));
    --it;
    double l1(val<1>(*it));
    Color c1(val<2>(*it));
    
    return Color::LinInterp(c1,c2,(l2-level)/(l2-l1));
  }
};

void ComputeColorsLevel(VertexBuffer<vertex>::Ptr& vb)
{
  ColorPalette cp;
  double max(5.);
  cp.addStop(-max*2, Color(0,0,0,255));
  cp.addStop(-max/3., Color(255,0,0,255));

  cp.addStop(0., Color(0,255,0,255));

  cp.addStop(max/3., Color(0,0,255,255));
  cp.addStop(max*2, Color(0,0,0,255));


  
  for(unsigned int tri(0);tri < vb->getSize()/3;++tri)
    {
      vec3 p1(vb->at(3*tri  )->x, vb->at(3*tri  )->y, vb->at(3*tri  )->z);
      vec3 p2(vb->at(3*tri+1)->x, vb->at(3*tri+1)->y, vb->at(3*tri+1)->z);
      vec3 p3(vb->at(3*tri+2)->x, vb->at(3*tri+2)->y, vb->at(3*tri+2)->z);
      
      vec3 n(CrossProduct(p3-p1, p2-p1));
      Normalize(n);
      
      double Slope(std::acos(DotProduct(n,vec3(0.,0.,1.))));
      if(Slope < 0.95*M_PI/2. || Slope > 1.05*M_PI/2.)
	{
	  for(unsigned int k(0);k<3;++k)
	    {
	      Color c(cp.getColor(vb->at(3*tri+k)->z));
	      vb->at(3*tri+k)->r = c.red();
	      vb->at(3*tri+k)->g = c.green();
	      vb->at(3*tri+k)->b = c.blue();
	      vb->at(3*tri+k)->a = 255;
	    }
	}
      else
	{
	  for(unsigned int k(0);k<3;++k){
	    vb->at(3*tri+k)->r = 0;
	    vb->at(3*tri+k)->g = 0;
	    vb->at(3*tri+k)->b = 0;
	    vb->at(3*tri+k)->a = 255;
	  }
	}
    }
}

VertexBuffer<vertex>::Ptr CreateSurface(BasicRenderer& br,const array& a)
{
  const unsigned int bands(a.size()-1);
  const unsigned int ranges(a.front().size()-1);
    
  VertexDeclaration decl[] = {{TYP_FLOAT3, USA_POSITION},
			      {TYP_COLOR, USA_DIFFUSE}};

  VertexBuffer<vertex>::Ptr vb(br.CreateVertexBuffer<vertex>(bands*ranges*2*3, decl, NULL));
  if(vb->Lock())
    {
      unsigned int k(0);
      for(unsigned int band(0); band < bands;++band)
	{
	  for(unsigned int range(0);range < ranges;++range)
	    {
	      CopyPosition(vb->at(6*k)  , a[band][range]);
	      CopyPosition(vb->at(6*k)+1, a[band][range+1]);
	      CopyPosition(vb->at(6*k)+2, a[band+1][range+1]);
	      CopyPosition(vb->at(6*k)+3, a[band][range]);
	      CopyPosition(vb->at(6*k)+4, a[band+1][range+1]);
	      CopyPosition(vb->at(6*k)+5, a[band+1][range]);
	      
	      ++k;
	    }
	}
      ComputeColorsLevel(vb);
      vb->Unlock();
    }
  else
    throw std::string("Failed to lock the vertex buffer.");

  return vb;
}

std::vector<double> VectorizeLine(const std::string& l)
{
  std::vector<double> r;
  double tmp;
  std::istringstream iss(l);
  while(iss)
    {
      iss >> tmp;
      if(iss)
	r.push_back(tmp);
    }
  //std::cout << "Size of line: " << r.size() << std::endl;
  return r;
}


bool CheckArrayStructure(const array& a)
{
  if(a.size())
    {
      unsigned int N(a.front().size());
      for(unsigned int i(0);i<a.size();++i)
	{
	  if(a[i].size() != N)
	    return false;
	}
      return true;
    }
  return false;
}

bool ReadDatum(const std::string& filename,
	       array& v,
	       unsigned int c1,
	       unsigned int c2,
	       unsigned int c3)
{
  std::ifstream ifs(filename.c_str(), std::ios::in);
  std::vector<Tuple<double,double,double> > row;
  if(ifs)
    {
      std::string line;
      while(std::getline(ifs, line))
	{
	  if(line.size() == 0)
	    {
	      v.push_back(row);
	      row.clear();
	    }
	  else if(line[0] != '#')
	    {
	      std::vector<double> l(VectorizeLine(line));
	      row.push_back(MakeTuple(l[c1], l[c2], l[c3]));
	    }
	}
      if(!row.empty()) v.push_back(row);
      return CheckArrayStructure(v);
    }
  return false;
}

class Stop
{
  bool m_shouldStop;
public:
  Stop(): m_shouldStop(false){}
  void Trigger(QuitEvent& e){m_shouldStop = !m_shouldStop;}
  operator bool(){return m_shouldStop;}
};

class TrackBallCamera
{
  double m_theta;
  double m_phi;

  UnixChrono m_time;
  DampedDynamic m_dampr;
  double m_r;
  
  double m_fov;

  bool m_moving;
public:
  TrackBallCamera(): m_r(15),
		     m_dampr(&m_time),
		     m_fov(50),
		     m_theta(90),
		     m_phi(0),
		     m_moving(false)
  {}
  
  mat4 getMatrix()
  {
    m_dampr.Update();
    return 
      Ortho(-m_dampr.getValue(),m_dampr.getValue(),
	    -0.5*m_dampr.getValue(),0.5*m_dampr.getValue(),
	    -1000,1000.)
      /*Perspective(m_fov, 2, 0.1,1000)
       *Translate(vec3(0,0,-m_dampr.getValue()))*/
      *RotateX(-m_theta)
      *RotateZ(m_phi);
  }
  
  void OnMouseMoveEvent(MouseMoveEvent& e)
  {
    if(m_moving)
      {
	m_theta += e.GetDelta().at(1);
	m_phi += e.GetDelta().at(0);
	
	if(m_theta < 0)
	  m_theta  = 0;
	
	if(m_theta > 180)
	  m_theta  = 180;
      }
  }
  void OnMousePressEvent(MousePressEvent& e)
  { m_moving = e.Type() == BTN_PRESSED ? true:false; }
  void OnScrollEvent(ScrollEvent& e)
  {
    m_dampr.addSlope(SlopeScrollFunction(m_time.getTimeFromInit(), 0.5*e.DeltaScroll()));
  }
};

std::vector<std::string> split(const std::string& str, char c)
{
  std::vector<std::string> f;
  size_t pos(0);
  size_t tmp(0);
  while((tmp = str.find(c,pos)) != std::string::npos)
    {
      f.push_back(str.substr(pos, tmp-pos));
      pos = tmp+1;
    }
  f.push_back(str.substr(pos, str.size()-pos));
  return f;
}

std::ostream& operator<<(std::ostream& flux, const std::vector<std::string>& f)
{
  for(unsigned int i(0);i<f.size();++i)
    flux << f[i] << " ";
  return flux;
}

void ExtractFileInfo(const std::string& fileinfo,
		      std::string& filename,
		      unsigned int& x,
		      unsigned int& y,
		      unsigned int& z)
{
  std::vector<std::string> f(split(fileinfo, ':'));
  std::istringstream iss;
  if(f.size() == 4)
    {
      filename = f[0];
      iss.str(f[1]); iss>> x; iss.clear();
      iss.str(f[2]); iss>> y; iss.clear();
      iss.str(f[3]); iss>> z; iss.clear();
    }
  else
    throw std::string("File description error");
}

int main(int argc, char** argv)
{
  try{
  BasicRenderer br(1000,500,true);
  std::vector<VertexBuffer<vertex>::Ptr> vbs;
  
  for(unsigned int i(1);i<argc;++i)
    {
      std::string filename;
      unsigned int x,y,z;
      ExtractFileInfo(argv[i], filename,x,y,z);

      array a;
      if(!ReadDatum(filename.c_str(), a, x,y,z))
	throw std::string("ReadDatum failed.");
      
      vbs.push_back(CreateSurface(br, a));
    }      
  
  Stop s;
  br.getEventManager().QuitEventDispatcher(&Stop::Trigger, s);

  TrackBallCamera cam;
  br.getEventManager().MouseMoveEventDispatcher(&TrackBallCamera::OnMouseMoveEvent, cam);
  br.getEventManager().MousePressEventDispatcher(&TrackBallCamera::OnMousePressEvent, cam);
  br.getEventManager().ScrollEventDispatcher(&TrackBallCamera::OnScrollEvent, cam);

  Axis axis(br);
  while(!s)
    {
      br.getEventManager().CheckEvents();
      br.BeginFrame();
      
      br.LoadMatrix(MAT_PROJECTION, cam.getMatrix());      
      axis.Render();
      for(unsigned int i(0);i<vbs.size();++i)
	{
	  br.setVertexBuffer<vertex>(vbs[i]);
	  br.DrawPrimitives(PRI_POINTS, vbs[i]->getSize());
	  //br.DrawPrimitives(PRI_TRIANGLES, vbs[i]->getSize()/3);
	}

      br.EndFrame();
      usleep(30000);
    }
  }
  catch(std::string& e)
    {
      std::cout << e << std::endl;
    }
  
  return 0;
}
