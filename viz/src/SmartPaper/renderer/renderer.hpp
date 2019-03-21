#ifndef RENDERER
#define RENDERER

#include <string>
#include <vector>
#include <GL/gl.h>

#include "x11utilities.hpp"
#include "eventmanager.hpp"

#include "openglexts.hpp"
#include "../utils/smartptr.hpp"
#include "../utils/tinymatrix/matrix.hpp"

#include "texture.hpp"
#include "vertexbuffer.hpp"

enum MatrixType{MAT_MODELVIEW = 0,
		MAT_PROJECTION,
		MAT_TEXTURE};

enum PrimitiveType{PRI_POINTS = 0,
		   PRI_LINES,
		   PRI_LINELOOP,
		   PRI_TRIANGLES};

class BasicRenderer
{
  unsigned int m_width;
  unsigned int m_height;

  VertexbufferObjectProc* m_vbo;
  
  X11Display m_disp;
  OpenGLWindow m_window;
  
  EventManager* m_eventManager;
public:
  BasicRenderer(unsigned int width,
		unsigned int height,
		bool fullscreen);
  
  virtual ~BasicRenderer();
  
  EventManager& getEventManager();

  template<PixelStructure ps>
  typename Texture<ps>::Ptr CreateTexture(unsigned int width,
					  unsigned int height);

  template<typename VertexType, size_t N>
  typename VertexBuffer<VertexType>::Ptr
  CreateVertexBuffer(unsigned int Size,
		     const VertexDeclaration (&decl)[N],
		     void* data);

  template<typename IndexType>
  typename IndexBuffer<IndexType>::Ptr
  CreateIndexBuffer(unsigned int Size, void* data);

  
  void LoadMatrix(MatrixType mt, const mat4& m);
  void MultMatrix(MatrixType mt, const mat4& m);
  void PushMatrix(MatrixType mt);
  void PopMatrix(MatrixType mt);
  
  void BeginFrame();
  void EndFrame();

  template<PixelStructure ps>
  void setTexture(unsigned int unit, typename Texture<ps>::Ptr tex);
  template<typename VertexType>
  void setVertexBuffer(typename VertexBuffer<VertexType>::Ptr buf);

  void DrawPrimitives(PrimitiveType pt,
		      unsigned int PrimitiveNumber);

  template<typename IndexType>
  void DrawIndexedPrimitives(PrimitiveType pt,
			     unsigned int PrimitiveNumber,
			     typename IndexBuffer<IndexType>::Ptr buf);
};

inline BasicRenderer::~BasicRenderer(){ delete m_eventManager;}
  
inline EventManager& BasicRenderer::getEventManager(){return *m_eventManager;}


template<typename VertexType, size_t N>
typename VertexBuffer<VertexType>::Ptr
BasicRenderer::CreateVertexBuffer(unsigned int Size,
				  const VertexDeclaration (&decl)[N],
				  void* data)
{
  std::vector<VertexDeclaration> d(decl, decl+N);
  return typename VertexBuffer<VertexType>::Ptr
    (new VertexBuffer<VertexType>(sizeof(VertexType)*Size, d, data));
}

template<typename IndexType>
typename IndexBuffer<IndexType>::Ptr
BasicRenderer::CreateIndexBuffer(unsigned int Size, void* data)
{
  return typename IndexBuffer<IndexType>::Ptr
    (new IndexBuffer<IndexType>(Size, data));
}

template<typename VertexType>
void BasicRenderer::setVertexBuffer(typename VertexBuffer<VertexType>::Ptr buf)
{
  glColor4d(1.,1.,1.,1.);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  
  for(unsigned int i(0);i<4;++i)
    {
      glActiveTextureARB(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, 0);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

  m_vbo->glBindBufferARB(GL_ARRAY_BUFFER, buf->getBuffer());

  static GLsizei Stride(sizeof(VertexType));
  static unsigned int ByteSizes[] = {sizeof(float),
				     2*sizeof(float),
				     3*sizeof(float),
				     4*sizeof(float),
				     4*sizeof(char)};
  static GLint Sizes[] = {1,2,3,4,4};
  static GLenum Types[] = {GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_UNSIGNED_BYTE};
  unsigned int Offset(0);

  const std::vector<VertexDeclaration>& decl(buf->getDeclaration());

  for(unsigned int i(0);i<decl.size();++i)
    {
      switch(decl[i].vu)
	{
	case USA_POSITION:
	  glEnableClientState(GL_VERTEX_ARRAY);
	  glVertexPointer(Sizes[decl[i].vt], Types[decl[i].vt], Stride, (char*)NULL + Offset);
	  break;

	case USA_NORMAL:
	  glEnableClientState(GL_NORMAL_ARRAY);
	  glNormalPointer(Types[decl[i].vt], Stride, (char*)NULL + Offset);
	  break;

	case USA_DIFFUSE:
	  glEnableClientState(GL_COLOR_ARRAY);
	  glColorPointer(Sizes[decl[i].vt], Types[decl[i].vt], Stride, (char*)NULL + Offset);
	  break;

	case USA_TEXCOORD0:
	  glActiveTextureARB(GL_TEXTURE0_ARB);
	  glEnable(GL_TEXTURE_2D);
	  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	  glClientActiveTexture(GL_TEXTURE0);
	  glTexCoordPointer(Sizes[decl[i].vt], Types[decl[i].vt], Stride, (char*)NULL + Offset);
	  break;
	case USA_TEXCOORD1:
	  glActiveTextureARB(GL_TEXTURE1_ARB);
	  glEnable(GL_TEXTURE_2D);
	  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	  glClientActiveTexture(GL_TEXTURE1);
	  glTexCoordPointer(Sizes[decl[i].vt], Types[decl[i].vt], Stride, (char*)NULL + Offset); 
	  break;
	case USA_TEXCOORD2:
	  glActiveTextureARB(GL_TEXTURE2_ARB);
	  glEnable(GL_TEXTURE_2D);
	  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	  glClientActiveTexture(GL_TEXTURE2);
	  glTexCoordPointer(Sizes[decl[i].vt], Types[decl[i].vt], Stride, (char*)NULL + Offset);
	  break;
	case USA_TEXCOORD3:
	  glActiveTextureARB(GL_TEXTURE3_ARB);
	  glEnable(GL_TEXTURE_2D);
	  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	  glClientActiveTexture(GL_TEXTURE3);
	  glTexCoordPointer(Sizes[decl[i].vt], Types[decl[i].vt], Stride, (char*)NULL + Offset);
	  break;
	}
      Offset += ByteSizes[decl[i].vt];
    }
}

template<typename IndexType>
void BasicRenderer::DrawIndexedPrimitives(PrimitiveType pt,
					  unsigned int PrimitiveNumber,
					  typename IndexBuffer<IndexType>::Ptr buf)
{
  glEnableClientState(GL_INDEX_ARRAY);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf->getBuffer());
  
  GLenum type(sizeof(IndexType) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT);
  switch(pt)
    {
    case PRI_POINTS:	glDrawElements(GL_POINTS,    PrimitiveNumber,   type, 0); break;
    case PRI_LINES:	glDrawElements(GL_LINES,     PrimitiveNumber+1, type, 0); break;
    case PRI_LINELOOP:  glDrawElements(GL_LINES,     PrimitiveNumber*2, type, 0); break;
    case PRI_TRIANGLES: glDrawElements(GL_TRIANGLES, PrimitiveNumber*3, type, 0); break;
    }
}

template<PixelStructure ps>
typename Texture<ps>::Ptr BasicRenderer::CreateTexture(unsigned int width,
						       unsigned int height)
{
  static GLint texfmt[] = {GL_LUMINANCE8,
			   GL_LUMINANCE8_ALPHA8,
			   GL_RGB8,
			   GL_RGBA8}; //In this list we can add types to support float textures. (if supported.)

  GLuint texid;
  glGenTextures(1,&texid);
  glBindTexture(GL_TEXTURE_2D, texid);
  glTexImage2D(GL_TEXTURE_2D, 0, texfmt[ps],
	       width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, 0);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  return typename Texture<ps>::Ptr(new Texture<ps>(texid, width, height));
}

template<PixelStructure ps>
void BasicRenderer::setTexture(unsigned int unit, typename Texture<ps>::Ptr tex)
{
  glActiveTextureARB(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, tex->getTexture());
}

#endif
