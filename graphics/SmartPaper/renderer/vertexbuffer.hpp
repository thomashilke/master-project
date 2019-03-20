#ifndef VERTEXBUFFER
#define VERTEXBUFFER

#include <vector>
#include <string>

#include <GL/gl.h>
#include "openglexts.hpp"

enum VertexType{TYP_FLOAT1 = 0,
		TYP_FLOAT2,
		TYP_FLOAT3,
		TYP_FLOAT4,
		TYP_COLOR};

enum VertexUsage{USA_POSITION = 0,
		 USA_NORMAL,
		 USA_DIFFUSE,
		 USA_TEXCOORD0,
		 USA_TEXCOORD1,
		 USA_TEXCOORD2,
		 USA_TEXCOORD3};

struct VertexDeclaration
{
  VertexType vt; VertexUsage vu;
};

template<GLenum Target>
class BufferBase
{
  BufferBase(const BufferBase&);
  BufferBase& operator=(const BufferBase&);
protected:
  VertexbufferObjectProc* vbo;

  GLuint m_bufferid;
  unsigned int m_size;

  BufferBase(unsigned int Size, void* data): vbo(0),
					     m_bufferid(0),
					     m_size(Size)
  {
    vbo = VertexbufferObjectProc::Instance();
    vbo->glGenBuffersARB(1,&m_bufferid);
    vbo->glBindBufferARB(Target, m_bufferid);
    vbo->glBufferDataARB(Target, m_size, data, GL_STREAM_DRAW);
  }
  virtual ~BufferBase()
  { vbo->glDeleteBuffersARB(1,&m_bufferid); }
  
  void* Lock()
  {
    vbo->glBindBufferARB(Target, m_bufferid);
    return vbo->glMapBufferARB(Target, GL_READ_WRITE);
  }
  void Unlock()
  {
    vbo->glBindBufferARB(Target, BufferBase::m_bufferid);
    vbo->glUnmapBufferARB(Target);
  }
  unsigned int getSize(){return m_size;}
public:
  GLuint getBuffer(){return m_bufferid;}
};

template<typename VertexType>
class VertexBuffer: public BufferBase<GL_ARRAY_BUFFER>
{
  VertexType* m_array;
  std::vector<VertexDeclaration> m_decl;

  VertexBuffer(const VertexBuffer&);
  VertexBuffer& operator=(const VertexBuffer&);
public:
  typedef SmartPtr<VertexBuffer<VertexType> > Ptr;
  VertexBuffer(unsigned int Size,
	       const std::vector<VertexDeclaration>& decl,
	       void* data): BufferBase<GL_ARRAY_BUFFER>(Size, data),
			    m_array(NULL),
			    m_decl(decl)
  {}
  virtual ~VertexBuffer()
  {}
  
  const std::vector<VertexDeclaration>& getDeclaration(){return m_decl;}
  
  bool Lock()
  {
    if(m_array = reinterpret_cast<VertexType*>(BufferBase<GL_ARRAY_BUFFER>::Lock()))
      return true;
    return false;
  }
  void Unlock()
  {
    if(m_array)
      {
	BufferBase<GL_ARRAY_BUFFER>::Unlock();
	m_array = 0;
      }
  }
  VertexType* at(unsigned int i)
  {
    if(!m_array)
      throw std::string("VertexBuffer<VertexType>::at()"
			" - The buffer must be locked, before accessed");
    if(i>=m_size)
      throw std::string("VertexBuffer<VertexType>::at()"
			" - Index out of range.");
    return m_array+i;
  }
  unsigned int getSize(){return BufferBase<GL_ARRAY_BUFFER>::getSize()/sizeof(VertexType);}
};

template<typename IndexType>
class IndexBuffer: public BufferBase<GL_ELEMENT_ARRAY_BUFFER>
{
  IndexType* m_array;
  
  IndexBuffer(const IndexBuffer&);
  IndexBuffer& operator=(const IndexBuffer&);
public:
  typedef SmartPtr<IndexBuffer<IndexType> > Ptr;
  IndexBuffer(unsigned int Size, void* data): BufferBase<GL_ELEMENT_ARRAY_BUFFER>(Size, data), m_array(NULL)
  {}
  virtual ~IndexBuffer()
  {}
  
  bool Lock()
  {
    if(m_array = reinterpret_cast<IndexType*>(BufferBase<GL_ELEMENT_ARRAY_BUFFER>::Lock()))
      return true;
    return false;
  }
  void Unlock()
  {
    if(m_array)
      {
	BufferBase<GL_ELEMENT_ARRAY_BUFFER>::Unlock();
	m_array = 0;
      }
  }
  IndexType* at(unsigned int i)
  {
    if(!m_array)
      throw std::string("IndexBuffer<IndexType>::at()"
			" - The buffer must be locked, before accessed");
    if(i>=m_size)
      throw std::string("IndexBuffer<IndexType>::at()"
			" - Index out of range.");
    return m_array+i;
  }
  unsigned int getSize(){return BufferBase<GL_ELEMENT_ARRAY_BUFFER>::getSize()/sizeof(IndexType);}
};

#endif
