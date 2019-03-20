#ifndef VIDEO_STORAGE
#define VIDEO_STORAGE

#include <string>

#include <utils/smartptr.hpp>
#include <tile/matrixstorage.hpp>
#include <renderer/texture.hpp>

template<PixelStructure ps>
class VideoStorage: public MatrixStorage<ps>
{
  typename Texture<ps>::Ptr m_tex;

public:
  typedef SmartPtr<VideoStorage<ps> > Ptr;

  VideoStorage(typename Texture<ps>::Ptr t);

  virtual ~VideoStorage();
  
  virtual unsigned int getSize() const;

  virtual Color getPixel(unsigned int i,
			    unsigned int j) const;
  virtual void setPixel(unsigned int i,
		      unsigned int j,
		      const Color&);
  
  void lock();
  void unlock();
  typename Texture<ps>::Ptr getTexture(){return m_tex;}
};

template<PixelStructure ps>
VideoStorage<ps>::VideoStorage(typename Texture<ps>::Ptr t): m_tex(t)
{}

template<PixelStructure ps>
VideoStorage<ps>::~VideoStorage()
{}
  
template<PixelStructure ps>
unsigned int VideoStorage<ps>::getSize() const
{ return m_tex->getWidth(); }

template<PixelStructure ps>
Color VideoStorage<ps>::getPixel(unsigned int i,
				 unsigned int j) const
{ return MakeColor<ps>(m_tex->at(i,j)); }

template<PixelStructure ps>
void VideoStorage<ps>::setPixel(unsigned int i,
				unsigned int j,
				const Color& c)
{ ConvertPixel<ps, PIX_R8G8B8A8>(m_tex->at(i,j), c.Raw()); }
  
template<PixelStructure ps>
void VideoStorage<ps>::lock()
{ m_tex->Lock(); }

template<PixelStructure ps>
void VideoStorage<ps>::unlock()
{ m_tex->Unlock(); }

#endif
