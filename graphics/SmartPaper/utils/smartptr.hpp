#ifndef SMART_PTR
#define SMART_PTR

template<typename T>
class SmartPtr
{
  T* m_ptr;
  unsigned int* m_counter;

  void Release();
public:
  SmartPtr();
  SmartPtr(const SmartPtr<T>& p);
  explicit SmartPtr(T* p);

  ~SmartPtr();
  SmartPtr<T>& operator=(const SmartPtr<T>& p);
  SmartPtr<T>& operator=(T* p);
  
  T& operator*();
  const T& operator*() const;

  T* operator->();
  const T* operator->() const;

  /*operator T*();
    operator const T*();*/

  operator bool();

  T* RawPtr();
  const T* RawPtr() const;
};

template<typename T>
SmartPtr<T>::SmartPtr(): m_ptr(0), m_counter(0) {}

template<typename T>
SmartPtr<T>::SmartPtr(const SmartPtr<T>& p): m_ptr(p.m_ptr),
					     m_counter(p.m_counter)
{
  if(m_counter)
    ++(*m_counter);
}

template<typename T>
SmartPtr<T>::SmartPtr(T* p): m_ptr(p),
			     m_counter(0)
{ m_counter = new unsigned int(1); }

template<typename T>
void SmartPtr<T>::Release()
{
  if(m_counter && --(*m_counter) == 0)
    {
      delete m_ptr;
      delete m_counter;
    }
  m_ptr = 0;
  m_counter = 0;
}

template<typename T>
SmartPtr<T>::~SmartPtr()
{
  Release();
}

template<typename T>
SmartPtr<T>& SmartPtr<T>::operator=(const SmartPtr<T>& p)
{
  Release();
  m_ptr = p.m_ptr;
  m_counter = p.m_counter;
  
  if(m_counter)
    ++(*m_counter);
  return *this;
}

template<typename T>
SmartPtr<T>& SmartPtr<T>::operator=(T* p)
{
  Release();
  m_ptr = p;

  if(m_ptr)
    m_counter = new unsigned int(1);
  return *this;
}

template<typename T>
T& SmartPtr<T>::operator*()
{ return *m_ptr; }

template<typename T>
const T& SmartPtr<T>::operator*() const
{ return *m_ptr; }

template<typename T>
T* SmartPtr<T>::operator->()
{ return m_ptr; }

template<typename T>
const T* SmartPtr<T>::operator->() const
{ return m_ptr; }

/*template<typename T>
SmartPtr<T>::operator T*()
{ return m_ptr; }

template<typename T>
SmartPtr<T>::operator const T*()
{ return m_ptr; }*/

template<typename T>
SmartPtr<T>::operator bool()
{ return m_ptr; }

template<typename T>
T* SmartPtr<T>::RawPtr()
{ return m_ptr; }

template<typename T>
const T* SmartPtr<T>::RawPtr() const
{ return m_ptr; }


#endif
