#ifndef DISK_STORAGE
#define DISK_STORAGE

#include <utils/smartptr.hpp>
#include <utils/color.hpp>
#include <tile/matrixstorage.hpp>
#include <tile/diskstoragefactory.hpp>

template<PixelStructure ps>
class DiskStorage: public MatrixStorage<ps>
{
  DiskStorageFactory<ps>& m_diskFactory;
  unsigned int m_cacheID;

  unsigned char* m_array;
  
  unsigned int m_size;
public:
  typedef SmartPtr<DiskStorage<ps> > Ptr;

  DiskStorage(unsigned int CacheID,
	      unsigned char* ptr,
	      unsigned int Size,
	      DiskStorageFactory<ps>& Df);
  virtual ~DiskStorage();
  
  virtual unsigned int getSize() const;

  virtual Color getPixel(unsigned int i,
			    unsigned int j) const;
  virtual void setPixel(unsigned int i,
		      unsigned int j,
		      const Color&);
};

template<PixelStructure ps>
DiskStorage<ps>::DiskStorage(unsigned int CacheID,
			     unsigned char* ptr,
			    unsigned int Size,
			    DiskStorageFactory<ps>& Df): m_diskFactory(Df),
							 m_cacheID(CacheID),
							 m_array(ptr),
							 m_size(Size)
{}

template<PixelStructure ps>
DiskStorage<ps>::~DiskStorage()
{
  m_diskFactory.ReleaseStorage(m_cacheID);
  m_array = 0;
}
  
template<PixelStructure ps>
unsigned int DiskStorage<ps>::getSize() const
{ return m_size; }

template<PixelStructure ps>
Color DiskStorage<ps>::getPixel(unsigned int i,
				  unsigned int j) const
{ return MakeColor<ps>(m_array+MatrixStorage<ps>::Index(i,j)); }

template<PixelStructure ps>
void DiskStorage<ps>::setPixel(unsigned int i,
			       unsigned int j, const Color& c)
{
  //double Offset(MatrixStorage<ps>::Index(i,j));
  //std::cout <<"Offset "<< Offset << std::endl;
ConvertPixel<ps, PIX_R8G8B8A8>(m_array+MatrixStorage<ps>::Index(i,j), c.Raw()); }

#endif
