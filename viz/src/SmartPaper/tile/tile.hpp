#ifndef TILE_H
#define TILE_H

#include <utils/smartptr.hpp>
#include <utils/color.hpp>

#include <tile/videostoragefactory.hpp>
#include <tile/diskstoragefactory.hpp>
#include <tile/diskstorage.hpp>
#include <tile/videostorage.hpp>
#include <tile/centralstorage.hpp>

template<PixelStructure ps>
class Tile
{
  unsigned int m_size;

  VideoStorageFactory<ps>& m_videoFactory;
  DiskStorageFactory<ps>& m_diskFactory;

  typename VideoStorage<ps>::Ptr m_video;
  typename CentralStorage<ps>::Ptr m_central;
  typename DiskStorage<ps>::Ptr m_disk;

  Tile<ps> operator=(const Tile<ps>& m);
  Tile(const Tile<ps>& m);
public:
  typedef SmartPtr<Tile<ps> > Ptr;
  enum StorageLocation{STORAGE_VIDEO = 0x1,
		       STORAGE_CENTRAL = 0x1 << 1,
		       STORAGE_DISK = 0x1 << 2};
  
  Tile(unsigned int Size,
       DiskStorageFactory<ps>& df,
       VideoStorageFactory<ps>& vf);
  virtual ~Tile();

  unsigned int getSize();
  
  Color getPixel(unsigned int i, unsigned int j) const ;
  void setPixel(unsigned int i, unsigned int j, const Color&);
  
  void setStorage(StorageLocation loc);
  void Sync();
  
  typename Texture<ps>::Ptr getTexture(){return m_video->getTexture();}
};

template<PixelStructure ps>
Tile<ps>::Tile(unsigned int Size,
	       DiskStorageFactory<ps>& df,
	       VideoStorageFactory<ps>& vf): m_size(Size),
					      m_videoFactory(vf),
					      m_diskFactory(df)
{
  m_central = new CentralStorage<ps>(m_size);
}

template<PixelStructure ps>
Tile<ps>::~Tile(){}

template<PixelStructure ps>
unsigned int Tile<ps>::getSize(){ return m_size; }

template<PixelStructure ps>
Color Tile<ps>::getPixel(unsigned int i, unsigned int j) const
{
  if(!m_central)
    {
      if(m_disk)
	m_central = new CentralStorage<ps>(m_disk);
      else
	throw std::string("Tile<ps>::getPixel() const"
			  " - No valid data source to fill central storage.");
    }
  return Color(m_central->getPixel(i,j));
}

template<PixelStructure ps>
void Tile<ps>::setPixel(unsigned int i, unsigned int j, const Color& c)
{ 
  if(!m_central)
    {
      if(m_disk)
	m_central = new CentralStorage<ps>(*m_disk.RawPtr());
      else
	throw std::string("Tile<ps>::getPixel() const"
			  " - No valid data source to fill central storage.");
    }
    m_central->setPixel(i,j,c);
}

template<PixelStructure ps>  
void Tile<ps>::setStorage(StorageLocation loc)
{
  bool V(loc & STORAGE_VIDEO),
    C(loc & STORAGE_CENTRAL),
    D(loc & STORAGE_DISK);
  
  if(V && !m_video)
    {
      if(m_central)
	m_video = m_videoFactory.AllocateStorage(*m_central);
      else if(m_disk)
	m_video = m_videoFactory.AllocateStorage(*m_disk);
      else
	throw std::string("Tile<ps>::setStorage()"
			  " - No valid data source to fill video storage.");
    }
  if(C && !m_central)
    {
      if(m_disk)
	m_central = new CentralStorage<ps>(*m_disk);
      else
	throw std::string("Tile<ps>::setStorage()"
			  " - No valid data source to fill central storage.");
    }
  if(D && !m_disk)
    {
      if(m_central)
	m_disk = m_diskFactory.AllocateStorage(*m_central);
      else
	throw std::string("Tile<ps>::setStorage()"
			  " - No valid data source to fill disk storage.");
    }
  
  if(!V)
    m_video = 0;
  if(!C)
    m_central = 0;
  if(!D)
    m_disk = 0;
}

template<PixelStructure ps>
void Tile<ps>::Sync()
{
  if(m_central)
    {
      if(m_video)
	{
	  m_video->lock();
	  for(unsigned int i(0);i<m_size;++i)
	    for(unsigned int j(0);j<m_size;++j)
	      m_video->setPixel(i,j, m_central->getPixel(i,j));
	  m_video->unlock();
	}
      if(m_disk)
	{
	  for(unsigned int i(0);i<m_size;++i)
	    for(unsigned int j(0);j<m_size;++j)
	      m_disk->setPixel(i,j, m_central->getPixel(i,j));
	}
    }
}

#endif
