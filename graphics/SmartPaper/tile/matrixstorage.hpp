#ifndef MATRIXSTORAGE_H
#define MATRIXSTORAGE_H

template<PixelStructure ps>
class MatrixStorage
{
protected:
  unsigned int Index(unsigned int i, unsigned int j) const;
public:
  MatrixStorage();
  virtual ~MatrixStorage();
  
  virtual unsigned int getSize() const = 0;

  virtual Color getPixel(unsigned int i,
			    unsigned int j) const = 0;
  virtual void setPixel(unsigned int i,
		      unsigned int j,
		      const Color&) = 0;
};

template<PixelStructure ps>
MatrixStorage<ps>::MatrixStorage(){}

template<PixelStructure ps>
MatrixStorage<ps>::~MatrixStorage(){}

template<PixelStructure ps>
unsigned int MatrixStorage<ps>::Index(unsigned int i,
				     unsigned int j) const
{ 
  if(i> getSize() || j > getSize())
    throw std::string("MatrixStorage<ps>::Index() - Out of range.");

  return (ps+1)*(i*getSize()+j);
}



#endif
