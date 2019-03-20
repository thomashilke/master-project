#ifndef SMARTPTR
#define SMARTPTR

#include <iostream>

template<typename T>
class SmartPtr
{
  template<typename U>
  friend class SmartPtr;
  
  T* m_Ptr;
  unsigned int* m_Counter;
public:
  SmartPtr():m_Ptr(NULL), m_Counter(NULL) {}

  template<typename U>
  SmartPtr(U* Ptr): m_Ptr(Ptr), m_Counter(new unsigned int(1)){}

  SmartPtr(T* Ptr): m_Ptr(Ptr), m_Counter(new unsigned int(1)){}
  
  template<typename U>
  SmartPtr(const SmartPtr<U>& Op): m_Ptr(Op.m_Ptr), m_Counter(Op.m_Counter)
  {
	if(m_Counter)
	  (*m_Counter)++;
  }

  SmartPtr(const SmartPtr<T>& Op): m_Ptr(Op.m_Ptr), m_Counter(Op.m_Counter)
  {
	if(m_Counter)
	  (*m_Counter)++;
  }

  virtual ~SmartPtr()
  {
	if(m_Counter)
	  {
		(*m_Counter)--;
		if(*m_Counter == 0)
		  {
			delete m_Counter;
			delete m_Ptr;
			m_Counter = NULL;
			m_Ptr = NULL;
		  }
	  }
  }

  template<typename U>
  SmartPtr<T>& operator=(const SmartPtr<U>& Op)
  {
	if(m_Counter)
	  {
		(*m_Counter)--;
		if(*m_Counter == 0)
		  {
			delete m_Counter;
			delete m_Ptr;
			m_Counter = NULL;
			m_Ptr = NULL;
		  }
	  }
	m_Ptr = Op.m_Ptr;
	m_Counter = Op.m_Counter;
	if(m_Counter)
	  (*m_Counter)++;
	return *this;
  }

  T& operator*() const
  {
	if(!m_Ptr)
	  throw std::string("pointeur nul");
	return *m_Ptr;
  }

  T* operator->() const
  {
	if(!m_Ptr)
	  throw std::string("pointeur nul");
	return m_Ptr;
  }

  operator bool() const {return m_Ptr;}
};

#endif
