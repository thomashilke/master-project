#ifndef CANEVAS
#define CANEVAS

#include <vector>
#include <utils/tuple.hpp>
#include <utils/tinymatrix/vector.hpp>
#include <tile/tile.hpp>

class rectangle
{
  vec2 m_lowerleft;
  vec2 m_topright;
public:
  rectangle(const vec2& ll = vec2(),
	    const vec2& tr = vec2()): m_lowerleft(ll),
			     m_topright(tr)
  {}
};


class Canevas
{
  enum{TilePtr = 1, Position = 2, Probability = 3};
  typedef std::vector<Tuple<Tile<unsigned char>::Ptr,
			    vec2,
			    double> > TileCollection;
  TileCollection m_tiles;
  vec2 m_position;
  rectangle m_lastVisibleRegion;

  struct vertex { float x,y; float u,v; };
  VertexBuffer<vertex>::Ptr m_quadMesh;
  
  void UpdateProbabilities();
  
public:
  typedef SmartPtr<Canevas> Ptr;
  Canevas(BasicRenderer& rend)
  {
    vertex quad[] = {{0.,0., 0.,0.},
		     {1.,0., 1.,0.},
		     {1.,1., 1.,1.},
		     {0.,0., 0.,0.},
		     {1.,1., 1.,1.},
		     {0.,1., 0.,1.}};
    VertexDeclaration decl[] = {{TYP_FLOAT2, USA_POSITION},
				{TYP_FLOAT2, USA_TEXCOORD0}};
    m_quadMesh = rend.CreateVertexBuffer<vertex>(6,decl,quad);
  }

  void Draw(const rectangle& visibleRegion)
  {
    m_rend.setVertexBuffer<vertex>(m_quadMesh);
    for(TileCollection::iterator it(m_tiles.begin());
	it!=m_tiles.end();++it)
      {
	if(IsVisible(val<TilePtr>(*it)->Rectangle(), visibleRegion))
	  {
	    m_rend.pushMatrix(MAT_MODELVIEW);
	    m_rend.multMatrix(MAT_MODELVIEW,
			      transform::Translation(val<Position>(*it)));
	    m_rend.setTexture(0, val<TilePtr>(*it)->getTexture());
	    m_rend.drawPrimitive(PRI_TRIANGLES, 2);
	    m_rend.popMatrix(MAT_MODELVIEW);
	  }
      }
  }
};

#endif
