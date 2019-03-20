#include <cstdlib>

#include <renderer/renderer.hpp>
#include <utils/transform.hpp>

class Stop
{
  bool m_stop;
public:
  Stop():m_stop(false){}
  void Trigger(QuitEvent&){m_stop = !m_stop;}
  operator bool(){return m_stop;}
};

struct vertex
{
  float x,y;
  float u,v;
  unsigned char r,g,b,a;
};

int main()
{
  BasicRenderer br(500,500,true);
  
  vertex quad[] = {{-0.5,-0.5, 0,0, 255,255,255,255},
		   { 0.5,-0.5, 1,0, 255,255,255,255},
		   { 0.5, 0.5, 1,1, 255,255,255,255},
		   {-0.5,-0.5, 0,0, 255,255,255,255},
		   { 0.5, 0.5, 1,1, 255,255,255,255},
		   {-0.5, 0.5, 0,1, 255,255,255,255}};
  VertexDeclaration decl[] = {{TYP_FLOAT2,USA_POSITION},
			      {TYP_FLOAT2, USA_TEXCOORD0},
			      {TYP_COLOR, USA_DIFFUSE}};
  
  VertexBuffer<vertex>::Ptr vb(br.CreateVertexBuffer<vertex>(6, decl, quad));
  
  Texture<PIX_R8G8B8>::Ptr tex(br.CreateTexture<PIX_R8G8B8>(128,128));
  if(tex->Lock())
    {
      for(unsigned int i(0);i<128;++i)
	for(unsigned int j(0);j<128;++j)
	  {
	    *(tex->at(i,j)  ) = i*2;
	    *(tex->at(i,j)+1) = 0;
	    *(tex->at(i,j)+2) = 0;
	  }
      tex->Unlock();
    }
  
  Stop s;
  br.getEventManager().QuitEventDispatcher(&Stop::Trigger, s);
  
  br.LoadMatrix(MAT_PROJECTION, Perspective(60,1,0.1,100));
  br.LoadMatrix(MAT_MODELVIEW, Translate(vec3(0,0,-1)));

  while(!s)
    {
      br.getEventManager().CheckEvents();
      br.BeginFrame();

      //br.LoadMatrix(MAT_PROJECTION, Ortho(-10,10,-10,10,-10,10));

      br.setVertexBuffer<vertex>(vb);
      br.setTexture<PIX_R8G8B8>(0,tex);
      br.DrawPrimitives(PRI_TRIANGLES, 2);
      
      br.EndFrame();
    }
  return 0;
}
