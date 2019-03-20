#ifndef FUNC
#define FUNC

namespace func
{
  template<typename UpperRule, typename T1>
  class FunctorBase1
  {
  public:
    virtual ~FunctorBase1(){}
    
    virtual void operator()(UpperRule&, const T1&) = 0;
  };

  template<typename UpperRule, typename T1, typename NestedClass>
  class FunctorMem1: public FunctorBase1<UpperRule, T1>
  {
    NestedClass m_Instance;
    void (NestedClass::*m_Func)(UpperRule&, T1 const&);
  public:
    FunctorMem1(const NestedClass Instance, void (NestedClass::*Func)(UpperRule&, T1 const&)): m_Instance(Instance), m_Func(Func){}
    virtual ~FunctorMem1(){}
    
    void operator()(UpperRule& U, const T1& Arg1)
    {
      (m_Instance.*m_Func)(U, Arg1);
    }
  };

  template<typename UpperRule, typename T1>
  class FunctorFree1: public FunctorBase1<UpperRule,T1>
  {
    void (*m_Func)(UpperRule&, const T1&);
  public:
    FunctorFree1(void (*Func)(UpperRule&, const T1&)):m_Func(Func){}
    virtual ~FunctorFree1(){}

    void operator()(UpperRule& U, const T1& Arg1)
    {
      m_Func(U, Arg1);
    }
  };

  //-----------------------------------------------

  template<typename UpperRule, typename T1, typename T2>
  class FunctorBase2
  {
  public:
    virtual ~FunctorBase2(){}
    
    virtual void operator()(UpperRule&, const T1&, const T2&) = 0;
  };

  template<typename UpperRule, typename T1, typename T2, typename NestedClass>
  class FunctorMem2: public FunctorBase2<UpperRule, T1, T2>
  {
    NestedClass m_Instance;
    void (NestedClass::*m_Func)(UpperRule&, T1 const&, T2 const&);
  public:
    FunctorMem2(const NestedClass Instance, void (NestedClass::*Func)(UpperRule&, T1 const&, T2 const&)): m_Instance(Instance), m_Func(Func){}
    virtual ~FunctorMem2(){}
    
    void operator()(UpperRule& U, const T1& Arg1, const T2& Arg2)
    {
      (m_Instance.*m_Func)(U, Arg1, Arg2);
    }
  };

  template<typename UpperRule, typename T1, typename T2>
  class FunctorFree2: public FunctorBase2<UpperRule,T1, T2>
  {
    void (*m_Func)(UpperRule&, const T1&, const T2&);
  public:
    FunctorFree2(void (*Func)(UpperRule&, const T1&, const T2&)):m_Func(Func){}
    virtual ~FunctorFree2(){}

    void operator()(UpperRule& U, const T1& Arg1, const T2& Arg2)
    {
      m_Func(U, Arg1, Arg2);
    }
  };

  //------------------------------------------------------

  template<typename UpperRule, typename T1, typename T2, typename T3>
  class FunctorBase3
  {
  public:
    virtual ~FunctorBase3(){}
    
    virtual void operator()(UpperRule&, const T1&, const T2&, const T3&) = 0;
  };

  template<typename UpperRule, typename T1, typename T2, typename T3, typename NestedClass>
  class FunctorMem3: public FunctorBase3<UpperRule, T1, T2, T3>
  {
    NestedClass m_Instance;
    void (NestedClass::*m_Func)(UpperRule&, T1 const&, T2 const&, T3 const&);
  public:
    FunctorMem3(const NestedClass Instance, void (NestedClass::*Func)(UpperRule&, T1 const&, T2 const&, T3 const&)): m_Instance(Instance), m_Func(Func){}
    virtual ~FunctorMem3(){}
    
    void operator()(UpperRule& U, const T1& Arg1, const T2& Arg2, const T3& Arg3)
    {
      (m_Instance.*m_Func)(U, Arg1, Arg2, Arg3);
    }
  };

  template<typename UpperRule, typename T1, typename T2, typename T3>
  class FunctorFree3: public FunctorBase3<UpperRule,T1, T2, T3>
  {
    void (*m_Func)(UpperRule&, const T1&, const T2&, const T3&);
  public:
    FunctorFree3(void (*Func)(UpperRule&, const T1&, const T2&, const T3&)):m_Func(Func){}
    virtual ~FunctorFree3(){}

    void operator()(UpperRule& U, const T1& Arg1, const T2& Arg2, const T3& Arg3)
    {
      m_Func(U, Arg1, Arg2, Arg3);
    }
  };

  //------------------------------------------------------

  template<typename UpperRule, typename T1, typename T2, typename T3, typename T4>
  class FunctorBase4
  {
  public:
    virtual ~FunctorBase4(){}
    
    virtual void operator()(UpperRule&, const T1&, const T2&, const T3&, const T4&) = 0;
  };
  
  template<typename UpperRule, typename T1, typename T2, typename T3, typename T4, typename NestedClass>
  class FunctorMem4: public FunctorBase4<UpperRule, T1, T2, T3, T4>
  {
    NestedClass m_Instance;
    void (NestedClass::*m_Func)(UpperRule&, T1 const&, T2 const&, T3 const&, T4 const&);
  public:
    FunctorMem4(const NestedClass Instance, void (NestedClass::*Func)(UpperRule&, T1 const&, T2 const&, T3 const&, T4 const&)): m_Instance(Instance), m_Func(Func){}
    virtual ~FunctorMem4(){}
    
    void operator()(UpperRule& U, const T1& Arg1, const T2& Arg2, const T3& Arg3, const T4& Arg4)
    {
      (m_Instance.*m_Func)(U, Arg1, Arg2, Arg3, Arg4);
    }
  };

  template<typename UpperRule, typename T1, typename T2, typename T3, typename T4>
  class FunctorFree4: public FunctorBase4<UpperRule,T1, T2, T3, T4>
  {
    void (*m_Func)(UpperRule&, const T1&, const T2&, const T3&, const T4&);
  public:
    FunctorFree4(void (*Func)(UpperRule&, const T1&, const T2&, const T3&, const T4&)):m_Func(Func){}
    virtual ~FunctorFree4(){}

    void operator()(UpperRule& U, const T1& Arg1, const T2& Arg2, const T3& Arg3, const T4& Arg4)
    {
      m_Func(U, Arg1, Arg2, Arg3, Arg4);
    }
  };

  //------------------------------------------------------

  template<typename UpperRule, typename T1, typename T2, typename T3, typename T4, typename T5>
  class FunctorBase5
  {
  public:
    virtual ~FunctorBase5(){}
    
    virtual void operator()(UpperRule&, const T1&, const T2&, const T3&, const T4&, const T5&) = 0;
  };
  
  template<typename UpperRule, typename T1, typename T2, typename T3, typename T4, typename T5, typename NestedClass>
  class FunctorMem5: public FunctorBase5<UpperRule, T1, T2, T3, T4, T5>
  {
    NestedClass m_Instance;
    void (NestedClass::*m_Func)(UpperRule&, T1 const&, T2 const&, T3 const&, T4 const&, T5 const&);
  public:
    FunctorMem5(const NestedClass Instance,
		void (NestedClass::*Func)(UpperRule&, T1 const&, T2 const&, T3 const&, T4 const&, T5 const&)): m_Instance(Instance),
													       m_Func(Func){}
    virtual ~FunctorMem5(){}
    
    void operator()(UpperRule& U, const T1& Arg1, const T2& Arg2, const T3& Arg3, const T4& Arg4, const T5& Arg5)
    {
      (m_Instance.*m_Func)(U, Arg1, Arg2, Arg3, Arg4, Arg5);
    }
  };

  template<typename UpperRule, typename T1, typename T2, typename T3, typename T4, typename T5>
  class FunctorFree5: public FunctorBase5<UpperRule,T1, T2, T3, T4, T5>
  {
    void (*m_Func)(UpperRule&, const T1&, const T2&, const T3&, const T4&, const T5&);
  public:
    FunctorFree5(void (*Func)(UpperRule&, const T1&, const T2&, const T3&, const T4&, const T5&)):m_Func(Func){}
    virtual ~FunctorFree5(){}

    void operator()(UpperRule& U, const T1& Arg1, const T2& Arg2, const T3& Arg3, const T4& Arg4, const T5& Arg5)
    {
      m_Func(U, Arg1, Arg2, Arg3, Arg4, Arg5);
    }
  };
}

#endif
