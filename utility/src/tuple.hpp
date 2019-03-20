#ifndef TUPLE
#define TUPLE

#include <string>
#include <iostream>

template<typename T1, typename T2>
class Pair
{
  T1 m_left;
  T2 m_right;
public:
  Pair(const T1& left, const T2& right): m_left(left), m_right(right) {}

  template<typename U1, typename U2>
  Pair(const Pair<U1,U2>& p): m_left(p.left()), m_right(p.right()) {}

  template<typename U1, typename U2>
  Pair<T1,T2>& operator=(const Pair<U1,U2>& p)
  {
    m_left = p.left();
    m_right = p.right();
    return *this;
  }
  
  const T1& left() const {return m_left;}
  const T1& right() const {return m_right;}
  T1& left() {return m_left;}
  T1& right() {return m_right;}
};


template<typename T1, typename T2,
	 typename U1, typename U2>
inline
bool operator==(const Pair<T1,T2>& op1,const Pair<U1,U2>& op2)
{ return op1.left() == op2.left() && op1.right() == op2.right(); }

template<typename T1, typename T2,
	 typename U1, typename U2>
inline
bool operator!=(const Pair<T1,T2>& op1,const Pair<U1,U2>& op2)
{ return !(op1 == op2); }

template<typename T1, typename T2>
inline
Pair<T1,T2> Make_pair(const T1& p1, const T2& p2)
{ return Pair<T1,T2>(p1,p2); }

template<typename T1>
inline
Pair<T1,void> Make_pair(const T1& p1)
{ return Pair<T1,void>(p1); }

template<typename A, typename B, typename C>
class Pair<A, Pair<B,C> >
{
public:
  typedef A T1;
  typedef Pair<B,C> T2;
  enum{N = Pair<B,C>::N + 1};
  
private:
  T1 m_left;
  T2 m_right;
public:
  Pair(const T1& left, const T2& right): m_left(left), m_right(right) {}

  template<typename U1, typename U2>
  Pair(const Pair<U1,U2>& p): m_left(p.left()), m_right(p.right()) {}

  template<typename U1, typename U2>
  Pair<T1,T2>& operator=(const Pair<U1,U2>& p)
  {
    m_left = p.left();
    m_right = p.right();
    return *this;
  }
  
  const T1& left() const {return m_left;}
  const T2& right() const {return m_right;}
  T1& left() {return m_left;}
  T2& right() {return m_right;}
};

template<typename A>
class Pair<A, void>
{
public:
  typedef A T1;
  typedef void T2;
  enum{N = 1};
  
private:
  T1 m_left;
public:
  Pair(const T1& left): m_left(left) {}

  template<typename U1>
  Pair(const Pair<U1,void>& p): m_left(p.m_left) {}

  template<typename U1>
  Pair<T1,void>& operator=(const Pair<U1,void>& p)
  {
    m_left = p.m_left;
    return *this;
  }
  
  const T1& left() const {return m_left;}
  void right() const {}
  T1& left() {return m_left;}
  void right() {}
};


template<unsigned int N, typename T>
class PairType
{
public:
  typedef void ResultT;
};

template<typename A, typename B>
class PairType<1, Pair<A,B> >
{
public:
  typedef A ResultT;
};

template<typename A, typename B>
class PairType<2, Pair<A,B> >
{
public:
  typedef B ResultT;
};

template<unsigned int N, typename A, typename B, typename C>
class PairType<N, Pair<A,Pair<B,C> > >
{
public:
  typedef typename PairType<N-1, Pair<B,C> >::ResultT ResultT;
};

template<typename A, typename B, typename C>
class PairType<1, Pair<A,Pair<B,C> > >
{
public:
  typedef A ResultT;
};

template<typename A, typename B, typename C>
class PairType<2, Pair<A,Pair<B,C> > >
{
public:
  typedef B ResultT;
};

template<unsigned int N, typename T>
class PairValue
{
public:
  static void get(T& p){}
  static void get(const T& p){}
};

template<typename A, typename B>
class PairValue<1, Pair<A,B> >
{
public:
  static A& get(Pair<A,B>& p)
  { return p.left();}
  static const A& get(const Pair<A,B>& p)
  {return p.left();}
};

template<typename A, typename B>
class PairValue<2, Pair<A,B> >
{
public:
  static B& get(Pair<A,B>& p)
  { return p.right();}
  static const B& get(const Pair<A,B>& p)
  {return p.right();}
};

template<unsigned int N, typename A, typename B, typename C>
class PairValue<N, Pair<A, Pair<B,C> > >
{
public:
  static typename PairType<N, Pair<A, Pair<B,C> > >::ResultT&
  get( Pair<A,Pair<B,C> >& p)
	 { return PairValue<N-1, Pair<B,C> >::get(p.right());}
  static const typename PairType<N, Pair<A, Pair<B,C> > >::ResultT&
  get(const Pair<A,Pair<B,C> > & p)
  {return PairValue<N-1, Pair<B,C> >::get(p.right());}
};

template<typename A, typename B, typename C>
class PairValue<1, Pair<A, Pair<B,C> > >
{
public:
  static A&
  get( Pair<A,Pair<B,C> >& p)
  { return p.left();}
  static const A&
  get(const Pair<A,Pair<B,C> > & p)
  {return p.left();}
};

template<typename A, typename B, typename C>
class PairValue<2, Pair<A, Pair<B,C> > >
{
public:
  static B&
  get( Pair<A,Pair<B,C> >& p)
  { return p.right().left();}
  static const B&
  get(const Pair<A,Pair<B,C> > & p)
  {return p.right().left();}
};
       
  template<unsigned int N, typename A, typename B>
  const typename PairType<N, Pair<A,B> >::ResultT& val(Pair<A,B> const& p)
  {
    return PairValue<N, Pair<A,B> >::get(p);
  }
  
  template<unsigned int N, typename A, typename B>
  typename PairType<N, Pair<A,B> >::ResultT& val(Pair<A,B>& p)
  {
    return PairValue<N, Pair<A,B> >::get(p);
  }
  
  
  template<typename T>
  struct TypeOp
  {
    typedef T& Ref;
    typedef T const& ConstRef;
    typedef T Base;
    typedef T* Ptr;
    typedef const T* ConstPtr;
  };

  template<typename T>
  struct TypeOp<T&>
  {
    typedef T& Ref;
    typedef T const& ConstRef;
    typedef T Base;
    typedef T* Ptr;
    typedef const T* ConstPtr;
  };

  template<typename T>
  struct TypeOp<const T&>
  {
    typedef T& Ref;
    typedef T const& ConstRef;
    typedef T Base;
    typedef T* Ptr;
    typedef const T* ConstPtr;
  };

  template<typename T>
  struct TypeOp<T*>
  {
    typedef T& Ref;
    typedef T const& ConstRef;
    typedef T Base;
    typedef T* Ptr;
    typedef const T* ConstPtr;
  };

  template<typename  T>
  struct TypeOp<const T*>
  {
    typedef T& Ref;
    typedef T const& ConstRef;
    typedef T Base;
    typedef T* Ptr;
    typedef const T* ConstPtr;
  };



class NullT{};

template<typename T1,
	 typename T2 = NullT,
	 typename T3 = NullT,
	 typename T4 = NullT,
	 typename T5 = NullT>
class Tuple: public Pair<T1, typename Tuple<T2, T3, T4, T5, NullT>::BaseT >
{
public:
  typedef Pair<T1, typename Tuple<T2,T3,T4, T5, NullT>::BaseT > BaseT;
  Tuple(){}
  Tuple(typename TypeOp<T1>::ConstRef v1,
	typename TypeOp<T2>::ConstRef v2,
	typename TypeOp<T3>::ConstRef v3 = NullT(),
	typename TypeOp<T4>::ConstRef v4 = NullT(),
	typename TypeOp<T5>::ConstRef v5 = NullT()):
    BaseT(v1, Tuple<T2,T3,T4,T5,NullT>(v2,v3,v4,v5)){}
};

template<typename T1,
	 typename T2>
class Tuple<T1,T2, NullT, NullT, NullT>: public Pair<T1, Pair<T2,void> >
{
public:
  typedef Pair<T1, Pair<T2,void> > BaseT;
  Tuple(){}
  Tuple(typename TypeOp<T1>::ConstRef v1,
	typename TypeOp<T2>::ConstRef v2,
	typename TypeOp<NullT>::ConstRef = NullT(),
	typename TypeOp<NullT>::ConstRef = NullT(),
	typename TypeOp<NullT>::ConstRef = NullT()):
    BaseT(v1, v2){}
};

template<typename T1>
class Tuple<T1,NullT, NullT, NullT, NullT>: public Pair<T1,void >
{
public:
  typedef Pair<T1,void > BaseT;
  Tuple(){}
  Tuple(typename TypeOp<T1>::ConstRef v1,
	typename TypeOp<NullT>::ConstRef = NullT(),
	typename TypeOp<NullT>::ConstRef = NullT(),
	typename TypeOp<NullT>::ConstRef = NullT(),
	typename TypeOp<NullT>::ConstRef = NullT()):
    BaseT(v1){}
};


template<typename T1>
Tuple<T1> MakeTuple(const T1& t1)
{ return Tuple<T1>(t1); }

template<typename T1,
	 typename T2>
Tuple<T1,T2> MakeTuple(const T1& t1,
		       const T2& t2)
{ return Tuple<T1, T2>(t1, t2); }

template<typename T1,
	 typename T2,
	 typename T3>
Tuple<T1,T2, T3> MakeTuple(const T1& t1,
			    const T2& t2,
			    const T3& t3)
{ return Tuple<T1, T2, T3>(t1, t2, t3); }

template<typename T1,
	 typename T2,
	 typename T3,
	 typename T4>
Tuple<T1,T2, T3, T4> MakeTuple( const T1& t1,
				const T2& t2,
				const T3& t3,
				const T4& t4)
{ return Tuple<T1, T2, T3, T4>(t1, t2, t3, t4); }

template<typename T1,
	 typename T2,
	 typename T3,
	 typename T4,
	 typename T5>
Tuple<T1,T2, T3, T4, T5> MakeTuple(const T1& t1,
				   const T2& t2,
				   const T3& t3,
				   const T4& t4,
				   const T5& t5)
{ return Tuple<T1, T2, T3, T4, T5>(t1, t2, t3, t4, t5); }


template<typename T>
struct TypeName
{ static const char* name(){ return "[type unknown]";} };

template<>
struct TypeName<int>
{ static const char* name(){ return "int";} };

template<>
struct TypeName<unsigned int>
{ static const char* name(){ return "unsigned int";} };

template<>
struct TypeName<char>
{ static const char* name(){ return "char";} };

template<>
struct TypeName<unsigned char>
{ static const char* name(){ return "unsigned char";} };

template<>
struct TypeName<short>
{ static const char* name(){ return "short";} };

template<>
struct TypeName<unsigned short>
{ static const char* name(){ return "unsigned short";} };

template<>
struct TypeName<float>
{ static const char* name(){ return "float";} };

template<>
struct TypeName<double>
{ static const char* name(){ return "double";} };

template<>
struct TypeName<bool>
{ static const char* name(){ return "bool";} };

template<>
struct TypeName<long>
{ static const char* name(){ return "long";} };

template<typename A, typename B>
struct PTuple
{
  static void pr(const Pair<A,B>&){}
};

template<typename A>
struct PTuple<A,void>
{
  static void pr(std::ostream& flux, const Pair<A,void>& p)
  { flux << TypeName<A>::name() << ": " << p.left()<< "]"; }
};

template<typename A,
	 typename B,
	 typename C>
struct PTuple<A, Pair<B,C> >
{
  static void pr(std::ostream& flux, const Pair<A, Pair<B,C> >& p)
  {
    flux << TypeName<A>::name() << ": " << p.left() << ", ";
    PTuple<B,C>::pr(flux, p.right());
  }
};


template<typename A, typename B>
void PrintTuple(std::ostream& flux, const Pair<A,B>& p)
{
  flux << "Tuple[";
  PTuple<A,B>::pr(flux, p);
}

template<typename T1,
	 typename T2,
	 typename T3,
	 typename T4,
	 typename T5>
std::ostream& operator<<(std::ostream& flux, const Tuple<T1,T2,T3,T4,T5>& p)
{
  PrintTuple(flux, p);
  return flux;
}

#endif
