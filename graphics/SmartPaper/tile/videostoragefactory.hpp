#ifndef VIDEO_STORAGE_FACTORY
#define VIDEO_STORAGE_FACTORY

#include <utils/singleton.hpp>

#include <tile/matrixstorage.hpp>
#include <renderer/renderer.hpp>

template<PixelStructure ps>
class VideoStorage;

template<PixelStructure ps>
class VideoStorageFactory
{
  BasicRenderer& m_rend;
  unsigned int m_size;
public:
  VideoStorageFactory(BasicRenderer& rend, unsigned int Size);// = 10);
  virtual ~VideoStorageFactory();
  
  typename VideoStorage<ps>::Ptr AllocateStorage();
  typename VideoStorage<ps>::Ptr AllocateStorage(const MatrixStorage<ps>& init);
};

template<PixelStructure ps>
VideoStorageFactory<ps>::VideoStorageFactory(BasicRenderer& rend,
      					    unsigned int Size): m_rend(rend),
								m_size(Size)
{}

template<PixelStructure ps>
VideoStorageFactory<ps>::~VideoStorageFactory()
{}

template<PixelStructure ps>
typename VideoStorage<ps>::Ptr VideoStorageFactory<ps>::AllocateStorage()
{
  return typename VideoStorage<ps>::Ptr(new VideoStorage<ps>(m_rend.CreateTexture<ps>(m_size, m_size)));
}

template<PixelStructure ps>
typename VideoStorage<ps>::Ptr VideoStorageFactory<ps>::AllocateStorage(const MatrixStorage<ps>& init)
{
  typename Texture<ps>::Ptr tex(m_rend.CreateTexture<ps>(m_size, m_size));
  typename VideoStorage<ps>::Ptr p(new VideoStorage<ps>(tex));

  p->lock();

  for(unsigned int i(0);i<m_size;++i)
    for(unsigned int j(0);j<m_size;++j)
      p->setPixel(i,j,init.getPixel(i,j));

  p->unlock();
  return p;
}

#endif
