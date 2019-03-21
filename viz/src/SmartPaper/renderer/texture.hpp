#ifndef TEXTURE
#define TEXTURE

#include <string>
#include <GL/gl.h>

#include "../utils/smartptr.hpp"
#include "../utils/color.hpp"

template<PixelStructure ps>
class Texture
{
  unsigned int m_width;
  unsigned int m_height;

  GLuint m_textureid;

  unsigned char* m_array;
  
  Texture(const Texture<ps>&);
  Texture<ps>& operator=(const Texture<ps>&);
public:
  typedef SmartPtr<Texture<ps> > Ptr;
  
  Texture(GLuint id,
	  unsigned int Width,
	  unsigned int Height);

  ~Texture();

  bool
  Lock();

  void
  Unlock();

  void
  Fill(unsigned char* data);  

  unsigned char*  
  at(unsigned int i, unsigned int j);

  const unsigned char*  
  at(unsigned int i, unsigned int j) const;
  unsigned int getWidth()const{return m_width;}
  unsigned int getHeight()const{return m_height;}
  GLuint
  getTexture();
};


template<PixelStructure ps>
inline GLuint Texture<ps>::getTexture(){return m_textureid;}

template<PixelStructure ps>
Texture<ps>::Texture(GLuint id,
		 unsigned int Width,
		 unsigned int Height): m_width(Width),
				m_height(Height),
				m_textureid(id),
				m_array(NULL)
{}
template<PixelStructure ps>
Texture<ps>::~Texture()
{
  Unlock();
  glDeleteTextures(1,&m_textureid);
}

template<PixelStructure ps>
bool Texture<ps>::Lock()
{
  if(!m_array)
    {
      static unsigned int pixelsize[] = {1,2,3,4};
      m_array = new unsigned char[m_width*m_height*pixelsize[ps]];
      return true;
    }
  return false;
}

template<PixelStructure ps>
void Texture<ps>::Unlock()
{
  if(m_array)
    {
      static GLenum intfmt[] = {GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA};
      
      glTexSubImage2D(GL_TEXTURE_2D, 0,
		      0,0, m_width, m_height,
		      intfmt[ps], GL_UNSIGNED_BYTE, m_array);
      delete [] m_array;
      m_array = NULL;
    }
}

template<PixelStructure ps>
void Texture<ps>::Fill(unsigned char* data)
{
  static GLenum intfmt[] = {GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA};
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, m_width, m_height, intfmt[ps], GL_UNSIGNED_BYTE, data);
}

template<PixelStructure ps>
unsigned char* Texture<ps>::at(unsigned int i, unsigned int j)
{
  if(!m_array)
    throw std::string("Texture::at()"
		      " - The texture must be locked, before accessed.");
  static unsigned int pixelsize[] = {1,2,3,4};
  return m_array +(j+m_width*i)*pixelsize[ps];
}
template<PixelStructure ps>
const unsigned char* Texture<ps>::at(unsigned int i, unsigned int j) const
{
  if(!m_array)
    throw std::string("Texture::at()"
		      " - The texture must be locked, before accessed.");
  static unsigned int pixelsize[] = {1,2,3,4};
  return m_array +(j+m_width*i)*pixelsize[ps];
}


#endif
