#ifndef AST
#define AST

#include "func.hpp"
#include "smartptr.h"
#include <iomanip>


namespace ast
{
  template<typename SemanticType>
  class SemanticNode
  {
  public:
    SemanticNode(){}
    virtual ~SemanticNode(){}

    virtual bool Execute(SemanticType& UpperRule) = 0;
    virtual void Print(std::ostream& Flux, unsigned int level) = 0;
  };

  template<typename SemanticType,
	   typename IntrinsecType1>
  class Node1: public SemanticNode<SemanticType>
  {
  protected:
	SmartPtr<func::FunctorBase1<SemanticType, IntrinsecType1> > m_Actionp;
    SemanticNode<IntrinsecType1>* m_FirstChild;
  public:
    Node1(SemanticNode<IntrinsecType1>* n1,
	  const SmartPtr<func::FunctorBase1<SemanticType, IntrinsecType1> >& Action): m_Actionp(Action), m_FirstChild(n1){}
    virtual ~Node1()
    {
      delete m_FirstChild;
      m_FirstChild = NULL;
    }
    
    virtual bool Execute(SemanticType& UpperRule)
    {
      IntrinsecType1 T;
      m_FirstChild->Execute(T);
      if(m_Actionp)
		m_Actionp->operator()(UpperRule, T);
      return true;
    }
    virtual void Print(std::ostream& Flux, unsigned int Level)
    {
      Flux << std::string(Level, ' ') << "Node1" << std::endl;
      m_FirstChild->Print(Flux, Level+1);
    }
  };

  template<typename SemanticType>
  class Leaf: public Node1<SemanticType, std::string>
  {    
    const std::string m_Value;
  public:
    Leaf(const std::string& s,
		 const SmartPtr<func::FunctorBase1<SemanticType, std::string> >& Action): Node1<SemanticType, std::string>(NULL, Action), m_Value(s){}
    virtual ~Leaf(){}
    
    virtual bool Execute(SemanticType& UpperRule)
    {
      if(Node1<SemanticType, std::string>::m_Actionp)
		Node1<SemanticType, std::string>::m_Actionp->operator()(UpperRule, m_Value);
      return true;
    }
    virtual void Print(std::ostream& Flux, unsigned int Level)
    {
      Flux << std::string(Level, ' ') << "Leaf: " <<m_Value << std::endl;
    }
  };

  template<typename SemanticType, typename IntrinsecType1, typename IntrinsecType2>
  class Node2: public SemanticNode<SemanticType>
  {
    SmartPtr<func::FunctorBase2<SemanticType, IntrinsecType1, IntrinsecType2> > m_Actionp;
    
    SemanticNode<IntrinsecType1>* m_FirstChild;
    SemanticNode<IntrinsecType2>* m_SecondChild;
  public:
    Node2(SemanticNode<IntrinsecType1>* n1,
		  SemanticNode<IntrinsecType2>* n2,
		  const SmartPtr<func::FunctorBase2<SemanticType, IntrinsecType1, IntrinsecType2> >& Action): m_Actionp(Action), m_FirstChild(n1), m_SecondChild(n2){}
    virtual ~Node2()
    {
      delete m_FirstChild;
      m_FirstChild = NULL;
      delete m_SecondChild;
      m_SecondChild = NULL;
    }
    
    virtual bool Execute(SemanticType& UpperRule)
    {
      IntrinsecType1 T1;
      IntrinsecType2 T2;

      m_FirstChild->Execute(T1);
      m_SecondChild->Execute(T2);
      if(m_Actionp)
	m_Actionp->operator()(UpperRule, T1, T2);
      return true;
    }
    virtual void Print(std::ostream& Flux, unsigned int Level)
    {
      Flux << std::string(Level, ' ') << "Node2" << std::endl;
      m_FirstChild->Print(Flux, Level+1);
      m_SecondChild->Print(Flux, Level+1);
    }
  };

  template<typename SemanticType, typename IntrinsecType1, typename IntrinsecType2, typename IntrinsecType3>
  class Node3: public SemanticNode<SemanticType>
  {
    SmartPtr<func::FunctorBase3<SemanticType, IntrinsecType1, IntrinsecType2, IntrinsecType3> > m_Actionp;

    SemanticNode<IntrinsecType1>* m_FirstChild;
    SemanticNode<IntrinsecType2>* m_SecondChild;
    SemanticNode<IntrinsecType3>* m_ThirdChild;

  public:
    Node3(SemanticNode<IntrinsecType1>* n1, SemanticNode<IntrinsecType2>* n2, SemanticNode<IntrinsecType3>* n3,
		  const SmartPtr<func::FunctorBase3<SemanticType, IntrinsecType1, IntrinsecType2, IntrinsecType3> >& Action): m_Actionp(Action),
													    m_FirstChild(n1),
													    m_SecondChild(n2),
													    m_ThirdChild(n3)
    {}
    virtual ~Node3()
    {
      delete m_FirstChild;
      m_FirstChild = NULL;
      delete m_SecondChild;
      m_SecondChild = NULL;
      delete m_ThirdChild;
      m_ThirdChild = NULL;
    }
    
    virtual bool Execute(SemanticType& UpperRule)
    {
      IntrinsecType1 T1;
      IntrinsecType2 T2;
      IntrinsecType3 T3;

      m_FirstChild->Execute(T1);
      m_SecondChild->Execute(T2);
      m_ThirdChild->Execute(T3);
      if(m_Actionp)
	m_Actionp->operator()(UpperRule, T1, T2, T3);
      return true;
    }
    virtual void Print(std::ostream& Flux, unsigned int Level)
    {
      Flux << std::string(Level, ' ') << "Node3" << std::endl;
      m_FirstChild->Print(Flux, Level+1);
      m_SecondChild->Print(Flux, Level+1);
      m_ThirdChild->Print(Flux, Level+1);
    }
  };

  template<typename SemanticType, typename IntrinsecType1, typename IntrinsecType2, typename IntrinsecType3, typename IntrinsecType4>
  class Node4: public SemanticNode<SemanticType>
  {
    SmartPtr<func::FunctorBase4<SemanticType, IntrinsecType1, IntrinsecType2, IntrinsecType3, IntrinsecType4> > m_Actionp;

    SemanticNode<IntrinsecType1>* m_FirstChild;
    SemanticNode<IntrinsecType2>* m_SecondChild;
    SemanticNode<IntrinsecType3>* m_ThirdChild;
    SemanticNode<IntrinsecType4>* m_ForthChild;

  public:
    Node4(SemanticNode<IntrinsecType1>* n1,
		  SemanticNode<IntrinsecType2>* n2,
		  SemanticNode<IntrinsecType3>* n3,
		  SemanticNode<IntrinsecType4>* n4,
		  const SmartPtr<func::FunctorBase4<SemanticType, IntrinsecType1, IntrinsecType2, IntrinsecType3, IntrinsecType4> >& Action): m_Actionp(Action),
																																	  m_FirstChild(n1),
																																	  m_SecondChild(n2),
																																	  m_ThirdChild(n3),
																																	  m_ForthChild(n4){}
    virtual ~Node4()
    {
      delete m_FirstChild;
      m_FirstChild = NULL;
      delete m_SecondChild;
      m_SecondChild = NULL;
      delete m_ThirdChild;
      m_ThirdChild = NULL;
      delete m_ForthChild;
      m_ForthChild = NULL;
    }
    
    virtual bool Execute(SemanticType& UpperRule)
    {
      IntrinsecType1 T1;
      IntrinsecType2 T2;
      IntrinsecType3 T3;
      IntrinsecType4 T4;

      m_FirstChild->Execute(T1);
      m_SecondChild->Execute(T2);
      m_ThirdChild->Execute(T3);
      m_ForthChild->Execute(T4);
      if(m_Actionp)
		m_Actionp->operator()(UpperRule, T1, T2, T3, T4);
      return true;
    }
    virtual void Print(std::ostream& Flux, unsigned int Level)
    {
      Flux << std::string(Level, ' ') << "Node4" << std::endl;
      m_FirstChild->Print(Flux, Level+1);
      m_SecondChild->Print(Flux, Level+1);
      m_ThirdChild->Print(Flux, Level+1);
      m_ForthChild->Print(Flux, Level+1);
    }
  };

  template<typename SemanticType, typename IntrinsecType1, typename IntrinsecType2, typename IntrinsecType3, typename IntrinsecType4, typename IntrinsecType5>
  class Node5: public SemanticNode<SemanticType>
  {
    SmartPtr<func::FunctorBase5<SemanticType, IntrinsecType1, IntrinsecType2, IntrinsecType3, IntrinsecType4, IntrinsecType5> > m_Actionp;

    SemanticNode<IntrinsecType1>* m_FirstChild;
    SemanticNode<IntrinsecType2>* m_SecondChild;
    SemanticNode<IntrinsecType3>* m_ThirdChild;
    SemanticNode<IntrinsecType4>* m_ForthChild;
    SemanticNode<IntrinsecType5>* m_FithChild;

  public:
    Node5(SemanticNode<IntrinsecType1>* n1,
	  SemanticNode<IntrinsecType2>* n2,
	  SemanticNode<IntrinsecType3>* n3,
	  SemanticNode<IntrinsecType4>* n4,
	  SemanticNode<IntrinsecType5>* n5,
	  SmartPtr<func::FunctorBase5<SemanticType,
	  IntrinsecType1,
	  IntrinsecType2,
	  IntrinsecType3,
	  IntrinsecType4,
	  IntrinsecType5> > Action): m_Actionp(Action),
					   m_FirstChild(n1),
					   m_SecondChild(n2),
					   m_ThirdChild(n3),
					   m_ForthChild(n4),
					   m_FithChild(n5){}
    virtual ~Node5()
    {
      delete m_FirstChild;
      m_FirstChild = NULL;
      delete m_SecondChild;
      m_SecondChild = NULL;
      delete m_ThirdChild;
      m_ThirdChild = NULL;
      delete m_ForthChild;
      m_ForthChild = NULL;
      delete m_FithChild;
      m_FithChild = NULL;
    }
    
    virtual bool Execute(SemanticType& UpperRule)
    {
      IntrinsecType1 T1;
      IntrinsecType2 T2;
      IntrinsecType3 T3;
      IntrinsecType4 T4;
      IntrinsecType5 T5;

      m_FirstChild->Execute(T1);
      m_SecondChild->Execute(T2);
      m_ThirdChild->Execute(T3);
      m_ForthChild->Execute(T4);
      m_FithChild->Execute(T5);
      if(m_Actionp)
	m_Actionp->operator()(UpperRule, T1, T2, T3, T4, T5);
      return true;
    }
    virtual void Print(std::ostream& Flux, unsigned int Level)
    {
      Flux << std::string(' ', Level) << "Node5" << std::endl;
      m_FirstChild->Print(Flux, Level+1);
      m_SecondChild->Print(Flux, Level+1);
      m_ThirdChild->Print(Flux, Level+1);
      m_ForthChild->Print(Flux, Level+1);
      m_FithChild->Print(Flux, Level+1);
    }
  };
}

#endif
