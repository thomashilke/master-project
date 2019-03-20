#include <vector>
#include <cstdlib>

#include <renderer/renderer.hpp>
#include <tile/tile.hpp>

struct vertex
{
  float x,y;
  float u,v;
};

class Stop
{
  bool m_stop;
public:
  Stop():m_stop(false){}
  void Trigger(QuitEvent&){m_stop = !m_stop;}
  operator bool(){return m_stop;}
};

int main()
{
  try{
    const unsigned int TileSize(64);
    const unsigned int FileSize(10000);
    
    BasicRenderer br(500,500,true);
    DiskStorageFactory<PIX_L8> Df("cache", FileSize, TileSize);
    VideoStorageFactory<PIX_L8> Vf(br, TileSize);
    {
    
    Tile<PIX_L8>::Ptr tiles;
    {
      tiles = new Tile<PIX_L8>(TileSize, Df, Vf);
      tiles->setStorage(Tile<PIX_L8>::STORAGE_CENTRAL);
      tiles->setStorage(Tile<PIX_L8>::STORAGE_DISK);
    }
    
    for(unsigned int i(0);i<TileSize;++i)
      for(unsigned int j(0);j<TileSize;++j)
	tiles->setPixel(i,j, Color(4*i, 4*i,4*i,255.));
    tiles->Sync();
    
    
    
    tiles->setStorage(Tile<PIX_L8>::STORAGE_VIDEO);
    
    
    vertex quad[] = {{-0.5,-0.5, 0,0},
		     { 0.5,-0.5, 1,0},
		     { 0.5, 0.5, 1,1},
		     {-0.5,-0.5, 0,0},
		     { 0.5, 0.5, 1,1},
		     {-0.5, 0.5, 0,1}};
    VertexDeclaration decl[] = {{TYP_FLOAT2,USA_POSITION},
				{TYP_FLOAT2, USA_TEXCOORD0}};
    
    VertexBuffer<vertex>::Ptr vb(br.CreateVertexBuffer<vertex>(6, decl, quad));
    
    Stop s;
    br.getEventManager().QuitEventDispatcher(&Stop::Trigger, s);
    
    
    while(!s)
      {
	br.getEventManager().CheckEvents();
	br.BeginFrame();
	
	br.setVertexBuffer<vertex>(vb);
	br.setTexture<PIX_L8>(0,tiles->getTexture());
	br.DrawPrimitives(PRI_TRIANGLES, 2);
	
	br.EndFrame();
      }
    }
  }
  catch(std::string& e)
    {
      std::cout << e << std::endl;
    }
  return 0;
}
