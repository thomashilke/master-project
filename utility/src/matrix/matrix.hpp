#ifndef MATRIX
#define MATRIX

#include <string>

template<typename Storage, int Zero = 1>
class Matrix
{
  Storage* m_array;
  
  unsigned int m_rows;
  unsigned int m_columns;

  //column major ordering:
  Storage& at(unsigned int i, unsigned int j)
  { return m_array[j*m_rows+i]; }

  const Storage& at(unsigned int i, unsigned int j) const
  { return m_array[j*m_rows+i]; }

  //row major ordering
  /*Storage& at(unsigned int i, unsigned int j)
  { return m_array[j+i*m_columns]; }

  const Storage& at(unsigned int i, unsigned int j) const
  { return m_array[j+i*m_columns]; }*/

public:
  typedef Storage storage;

  Matrix(unsigned int rows, unsigned int columns): m_array(NULL),
						   m_rows(rows),
						   m_columns(columns)
  { m_array = new Storage[m_rows*m_columns]; }
  
  Matrix(const Matrix<Storage,Zero>& m): m_array(NULL),
				    m_rows(m.m_rows),
				    m_columns(m.m_columns)
  {
    m_array = new Storage[m_rows*m_columns];

    for(unsigned int i(0); i<m_rows;++i)
      for(unsigned int j(0); j<m_columns;++j)
	at(i,j) = m.at(i,j);
  }
  ~Matrix()
  { delete [] m_array; }
  
  Matrix<Storage, Zero>& operator=(const Matrix<Storage, Zero>& m)
  {
    if(m.m_rows != m_rows || m.m_columns != m_columns)
      Resize(m.m_rows, m.m_columns);
    
    for(unsigned int i(0); i<m_rows;++i)
      for(unsigned int j(0); j<m_columns;++j)
	at(i,j) = m.at(i,j);
    return *this;
  }

  void Resize(unsigned int rows, unsigned int columns)
  {
    delete [] m_array;
    m_array = new Storage[rows*columns];
    m_rows = rows;
    m_columns = columns;
  }
  
  unsigned int getColumns() const {return m_columns;}
  unsigned int getRows() const {return m_rows;}

  Storage& operator()(unsigned int i, unsigned int j)
  {
    if(i >= m_rows+Zero || j >= m_columns+Zero)
      throw std::string("Matrix::operator() - Out of range.");

    return at(i-Zero,j-Zero);
  }
  
  const Storage& operator()(unsigned int i, unsigned int j) const
  {
    if(i >= m_rows+Zero || j >= m_columns+Zero)
      throw std::string("Matrix::operator() - Out of range.");
  
    return at(i-Zero,j-Zero);
  }

  Storage& operator()(unsigned int i)
  {
    if(i >= m_rows+Zero)
      throw std::string("Matrix::operator() - Out of range.");

    return at(i-Zero,Zero);
  }
  
  const Storage& operator()(unsigned int i) const
  {
    if(i >= m_rows+Zero)
      throw std::string("Matrix::operator() - Out of range.");
  
    return at(i-Zero,Zero);
  }
};

typedef Matrix<double,1> Mat;

#endif
