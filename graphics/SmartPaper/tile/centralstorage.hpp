#ifndef CENTRAL_STORAGE
#define CENTRAL_STORAGE

#include <utils/smartptr.hpp>
#include <tile/matrixstorage.hpp>

template<PixelStructure ps>
class CentralStorage: public MatrixStorage<ps>
{
  unsigned char* m_array;

  unsigned int m_size;

public:
  typedef SmartPtr<CentralStorage<ps> > Ptr;

  CentralStorage(unsigned int Size);
  template<PixelStructure psp>
  CentralStorage(const MatrixStorage<psp>& init);
  virtual ~CentralStorage();
  
  virtual unsigned int getSize() const;

  virtual Color getPixel(unsigned int i,
			 unsigned int j) const;
  virtual void setPixel(unsigned int i,
			unsigned int j,
			const Color&);
};

template<PixelStructure ps>
CentralStorage<ps>::CentralStorage(unsigned int Size): m_array(0),
						       m_size(Size)
{ m_array = new unsigned char[m_size*m_size*(ps+1)]; }


template<PixelStructure ps>
template<PixelStructure psp>
CentralStorage<ps>::CentralStorage(const MatrixStorage<psp>& init): m_array(0),
								    m_size(init.getSize())
{
  m_array = new unsigned char[m_size*m_size*(ps+1)];
  for(unsigned int i(0);i<m_size;++i)
    for(unsigned int j(0);j<m_size;++j)
      setPixel(i,j, init.getPixel(i,j));
}


template<PixelStructure ps>
CentralStorage<ps>::~CentralStorage()
{
  delete [] m_array;
}
  
template<PixelStructure ps>
unsigned int CentralStorage<ps>::getSize() const
{ return m_size; }

template<PixelStructure ps>
Color CentralStorage<ps>::getPixel(unsigned int i,
				   unsigned int j) const
{ return MakeColor<ps>(m_array+ MatrixStorage<ps>::Index(i,j)); }

template<PixelStructure ps>
void CentralStorage<ps>::setPixel(unsigned int i,
				  unsigned int j,
				  const Color& c)
{ return ConvertPixel<ps,PIX_R8G8B8A8>(m_array+MatrixStorage<ps>::Index(i,j), c.Raw()); }

#endif
