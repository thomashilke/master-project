#ifndef BNF
#define BNF

#include "regex.hpp"
#include "ast.hpp"
#include "smartptr.h"

namespace bnf
{
  template<typename Stream,
	   typename TokenType,
	   typename Firt,
	   typename Last>
  class Choice;
  template<typename Stream,
	   typename TokenType>
  class RuleHolderBase;
  template<typename Stream,
	   typename TokenType,
	   typename R>
  class RuleHolder;
  template<typename Stream,
	   typename TokenType>
  class Symbol;
  template<typename Stream,
	   typename TokenType>
  class NonTerminal;
  template<typename Stream,
	   typename TokenType>
  class Terminal;
  template<typename Stream,
	   typename TokenType,
	   typename First,
	   typename Last>
  class Choice;
  template<typename Stream,
	   typename TokenType>
  class SequenceBase;
  template<typename Stream,
	   typename TokenType,
	   typename T1>
  class Sequence1;
  template<typename Stream,
	   typename TokenType,
	   typename T1,
	   typename T2>
  class Sequence2;
  template<typename Stream, typename TokenType,
	   typename T1,
	   typename T2,
	   typename T3>
  class Sequence3;
  template<typename Stream,
	   typename TokenType,
	   typename T>
  class Ref;

  template<typename Stream,
	   typename TokenType>
  class RuleHolderBase
  {
  public:
    virtual ~RuleHolderBase() {}
    virtual ast::SemanticNode<TokenType>* Parse(typename Stream::const_iterator& Start,
						typename Stream::const_iterator const End) const = 0;
  };

  template<typename Stream,
	   typename TokenType,
	   typename R>
  class RuleHolder: public RuleHolderBase<Stream, TokenType>
  {
    R m_Rule;
  public:
    RuleHolder(const R& Rule): m_Rule(Rule)
    {}
    virtual ~RuleHolder(){}
    virtual ast::SemanticNode<TokenType>* Parse(typename Stream::const_iterator& Start,
						typename Stream::const_iterator const End) const
    {
      return m_Rule.Parse(Start, End);
    }
  };

  template<typename Stream,
	   typename TokenType>
  class Symbol
  {
  public:
    virtual ~Symbol(){}

    virtual ast::SemanticNode<TokenType>* Parse(typename Stream::const_iterator& Start,
						typename Stream::const_iterator const End) const = 0;
  };

  template<typename Stream,
	   typename TokenType>
  class NonTerminal: public Symbol<Stream, TokenType>
  {
    RuleHolderBase<Stream, TokenType>* m_Rule;
  public:
    template<typename U>
    NonTerminal(const U& Rule): m_Rule(NULL)
    {
      m_Rule = new RuleHolder<Stream, TokenType, U>(Rule);
    }
    NonTerminal(): m_Rule(NULL)
    {}
    
    template<typename U>
    NonTerminal<Stream, TokenType>& operator=(const U& Rule)
    {
      if(m_Rule)
	delete m_Rule;
      m_Rule = new RuleHolder<Stream, TokenType, U>(Rule);
      return *this;
    }

    virtual ~NonTerminal()
    {
      delete m_Rule;
      m_Rule = NULL;
    }

    virtual ast::SemanticNode<TokenType>* Parse(typename Stream::const_iterator& Start,
						typename Stream::const_iterator const End) const
    {
      typename Stream::const_iterator tmp(Start);
      ast::SemanticNode<TokenType>* Child(NULL);
      if((Child = m_Rule->Parse(tmp, End)))
	{
	  Start = tmp;
	  return Child;
	}
      return NULL;
    }
    template<typename Op>
    Choice<Stream, TokenType, NonTerminal<Stream, TokenType>, Op> operator|(const Op& O) const
    {
      return Choice<Stream, TokenType, NonTerminal<Stream, TokenType>, Op>(*this, O);
    }
    typedef TokenType i_type;
  };
  
  template<typename Stream,
	   typename TokenType>
  class Terminal: public Symbol<Stream, TokenType>
  {
    typedef regex::Char<std::string> c;

    regex::Regex<Stream> m_Regex;
    regex::Regex<std::string> m_WhiteSpaces;

    SmartPtr<func::FunctorBase1<TokenType, Stream> > m_Actionp;
  public:
    Terminal(regex::Regex<Stream> R): m_Regex(R), m_WhiteSpaces(*(c(' ')|c('\n')|c('\r')|c('\t'))){}
    virtual ~Terminal(){}

    template<typename Cl>
    Terminal<Stream, TokenType>& operator()(const Cl Instance, void (Cl::*Func)(TokenType&, const Stream&))
    {
      m_Actionp = SmartPtr<func::FunctorMem1<TokenType, Stream, Cl> >(new func::FunctorMem1<TokenType, Stream, Cl>(Instance, Func));
      return *this;
    }
    
    Terminal<Stream, TokenType>& operator()(void (*Action)(TokenType&, const Stream&))
    {
      m_Actionp = SmartPtr<func::FunctorFree1<TokenType, Stream> >(new func::FunctorFree1<TokenType, Stream>(Action));
      return *this;
    }

    virtual ast::SemanticNode<TokenType>* Parse(typename Stream::const_iterator& Start,
						typename Stream::const_iterator const End) const
    {
      m_WhiteSpaces.Match(Start, End);
      typename Stream::const_iterator tmp(Start);
      if(m_Regex.Match(tmp, End))
	{
	  ast::SemanticNode<TokenType>* Node(new ast::Leaf<TokenType>(std::string(Start, tmp), m_Actionp));
	  Start = tmp;
	  return Node;
	}
      return NULL;
    }
    template<typename Op>
    Choice<Stream, TokenType, Terminal<Stream, TokenType>, Op> operator|(const Op& O) const
    {
      return Choice<Stream, TokenType, Terminal<Stream, TokenType>, Op>(*this, O);
    }
    typedef TokenType i_type;
  };

  template<typename Stream,
	   typename TokenType,
	   typename First,
	   typename Last>
  class Choice: public Symbol<Stream, TokenType>
  {
    First m_First;
    Last m_Last;
  public:
    Choice(First f, Last l): m_First(f), m_Last(l)
    {}
    virtual ~Choice(){}

    virtual ast::SemanticNode<TokenType>* Parse(typename Stream::const_iterator& Start,
						typename Stream::const_iterator const End) const
    {
      typename Stream::const_iterator tmp(Start);
      ast::SemanticNode<TokenType>* Children(NULL);
      if((Children = m_First.Parse(tmp, End)))
	{
	  Start = tmp;
	  return Children;
	}
      else if((Children = m_Last.Parse(tmp = Start, End)))
	{
	  Start = tmp;
	  return Children;
	}
      return NULL;
    }

    template<typename Op>
    Choice<Stream, TokenType, Choice<Stream, TokenType, First, Last>, Op> operator|(const Op& O) const
    {
      return Choice<Stream, TokenType, Choice<Stream, TokenType, First, Last>, Op>(*this, O);
    }
  };

  template<typename Stream,
	   typename TokenType>
  class SequenceBase: public Symbol<Stream, TokenType>
  {
  public:
    virtual ~SequenceBase(){}

    virtual ast::SemanticNode<TokenType>* Parse(typename Stream::const_iterator& Start,
						typename Stream::const_iterator const End) const = 0;
  };

  template<typename Stream,
	   typename TokenType,
	   typename T1>
  class Sequence1: public SequenceBase<Stream, TokenType>
  {
    T1 m_s1;
    SmartPtr<func::FunctorBase1<TokenType, typename T1::i_type> > m_Actionp;
  public:
    Sequence1(const T1& s1): m_s1(s1){}
    virtual ~Sequence1(){}

    template<typename Cl>
    Sequence1<Stream, TokenType, T1>& operator()(const Cl Instance, void (Cl::*Func)(TokenType&, typename T1::i_type const&))
    {
      m_Actionp = SmartPtr<func::FunctorMem1<TokenType, typename T1::i_type, Cl> >(new func::FunctorMem1<TokenType, typename T1::i_type, Cl>(Instance, Func));
      return *this;
    }

    Sequence1<Stream, TokenType, T1>& operator()(void (*Action)(TokenType&, typename T1::i_type const&))
    {
      m_Actionp = SmartPtr<func::FunctorFree1<TokenType, typename T1::i_type> >(new func::FunctorFree1<TokenType, typename T1::i_type>(Action));
      return *this;
    }

    virtual ast::SemanticNode<TokenType>* Parse(typename Stream::const_iterator& Start,
						typename Stream::const_iterator const End) const
    {
      typename Stream::const_iterator tmp(Start);
      ast::SemanticNode<typename T1::i_type>* FirstChild(NULL);
      if((FirstChild = m_s1.Parse(tmp, End)))
	{
	  Start = tmp;
	  return new ast::Node1<TokenType, typename T1::i_type>(FirstChild, m_Actionp);
	}
      return NULL;
    }
    
    template<typename Op>
    Choice<Stream, TokenType, Sequence1<Stream, TokenType, T1>, Op> operator|(const Op& O) const
    {
      return Choice<Stream, TokenType, Sequence1<Stream, TokenType, T1>, Op>(*this, O);
    }
  };

  template<typename Stream,
	   typename TokenType,
	   typename T1,
	   typename T2>
  class Sequence2: public SequenceBase<Stream, TokenType>
  {
    T1 m_s1;
    T2 m_s2;
    
    SmartPtr<func::FunctorBase2<TokenType, typename T1::i_type, typename T2::i_type> > m_Actionp;
  public:
    Sequence2(const T1& s1, const T2& s2): m_s1(s1), m_s2(s2){}
    virtual ~Sequence2(){}
    
    template<typename Cl>
    Sequence2<Stream, TokenType, T1, T2>& operator()(const Cl Instance, void (Cl::*Func)(TokenType&, typename T1::i_type const&, typename T2::i_type const&))
    {
      m_Actionp = SmartPtr<func::FunctorMem2<TokenType, typename T1::i_type, typename T2::i_type, Cl> >(new func::FunctorMem2<TokenType, typename T1::i_type, typename T2::i_type, Cl>(Instance, Func));
      return *this;
    }

    Sequence2<Stream, TokenType, T1, T2>& operator()(void (*Action)(TokenType&, typename T1::i_type const&, typename T2::i_type const&))
    {
      m_Actionp = SmartPtr<func::FunctorFree2<TokenType, typename T1::i_type, typename T2::i_type> >(new func::FunctorFree2<TokenType, typename T1::i_type, typename T2::i_type>(Action));
      return *this;
    }

    virtual ast::SemanticNode<TokenType>* Parse(typename Stream::const_iterator& Start,
						typename Stream::const_iterator const End) const
    {
      typename Stream::const_iterator tmp(Start);
      ast::SemanticNode<typename T1::i_type>* FirstChild(NULL);
      ast::SemanticNode<typename T2::i_type>* SecondChild(NULL);
      
      if((FirstChild = m_s1.Parse(tmp, End)) &&
	 (SecondChild = m_s2.Parse(tmp, End)))
	{
	  Start = tmp;
	  return new ast::Node2<TokenType, typename T1::i_type,typename T2::i_type>(FirstChild, SecondChild, m_Actionp);
	}
      delete FirstChild;
      delete SecondChild;
      return NULL;
    }
    
    template<typename Op>
    Choice<Stream, TokenType, Sequence2<Stream, TokenType, T1, T2>, Op> operator|(const Op& O) const
    {
      return Choice<Stream, TokenType, Sequence2<Stream, TokenType, T1, T2>, Op>(*this, O);
    }
  };

  template<typename Stream, typename TokenType,
	   typename T1,
	   typename T2,
	   typename T3>
  class Sequence3: public SequenceBase<Stream, TokenType>
  {
    T1 m_s1;
    T2 m_s2;
    T3 m_s3;
    
    SmartPtr<func::FunctorBase3<TokenType, typename T1::i_type,typename T2::i_type,typename T3::i_type> > m_Actionp;
  public:
    Sequence3(const T1& s1,
	      const T2& s2,
	      const T3& s3): m_s1(s1),
			     m_s2(s2),
			     m_s3(s3){}
    virtual ~Sequence3(){}
    
    template<typename Cl>
    Sequence3<Stream, TokenType, T1, T2, T3>& operator()(const Cl Instance, void(Cl::*Func)(TokenType&, typename T1::i_type const&, typename T2::i_type const&, typename T3::i_type const&))
    {
      m_Actionp = SmartPtr<func::FunctorMem3<TokenType, typename T1::i_type, typename T2::i_type, typename T3::i_type, Cl> >(new func::FunctorMem3<TokenType, typename T1::i_type, typename T2::i_type, typename T3::i_type, Cl>(Instance, Func));
      return *this;
    }

    Sequence3<Stream, TokenType, T1, T2, T3>& operator()(void (*Action)(TokenType&, typename T1::i_type const&, typename T2::i_type const&, typename T3::i_type const&))
    {
      m_Actionp = SmartPtr<func::FunctorFree3<TokenType, typename T1::i_type, typename T2::i_type, typename T3::i_type> >(new func::FunctorFree3<TokenType, typename T1::i_type, typename T2::i_type, typename T3::i_type>(Action));
      return *this;
    }

    virtual ast::SemanticNode<TokenType>* Parse(typename Stream::const_iterator& Start,
						typename Stream::const_iterator const End) const
    {
      typename Stream::const_iterator tmp(Start);
      ast::SemanticNode<typename T1::i_type>* FirstChild(NULL);
      ast::SemanticNode<typename T2::i_type>* SecondChild(NULL);
      ast::SemanticNode<typename T3::i_type>* ThirdChild(NULL);
      if((FirstChild = m_s1.Parse(tmp, End)) &&
	 (SecondChild = m_s2.Parse(tmp, End)) &&
	 (ThirdChild = m_s3.Parse(tmp, End)))
	{
	  Start = tmp;
	  return new ast::Node3<TokenType,typename  T1::i_type,typename  T2::i_type, typename T3::i_type>(FirstChild, SecondChild, ThirdChild, m_Actionp);
	}
      delete FirstChild;
      delete SecondChild;
      delete ThirdChild;
      return NULL;
    }
    
    template<typename Op>
    Choice<Stream,TokenType, Sequence3<Stream, TokenType, T1, T2, T3>, Op> operator|(const Op& O) const
    {
      return Choice<Stream, TokenType,Sequence3<Stream, TokenType, T1, T2, T3>, Op>(*this, O);
    }
  };

  template<typename Stream, typename TokenType,
	   typename T1,
	   typename T2,
	   typename T3,
	   typename T4>
  class Sequence4: public SequenceBase<Stream, TokenType>
  {
    T1 m_s1;
    T2 m_s2;
    T3 m_s3;
    T4 m_s4;
    
    SmartPtr<func::FunctorBase4<TokenType, typename T1::i_type, typename T2::i_type, typename T3::i_type, typename T4::i_type> > m_Actionp;
  public:
    Sequence4(const T1& s1,
	      const T2& s2,
	      const T3& s3, const T4& s4): m_s1(s1),
					   m_s2(s2),
					   m_s3(s3),
					   m_s4(s4){}
    virtual ~Sequence4(){}
    
    template<typename Cl>
    Sequence4<Stream, TokenType, T1, T2, T3, T4>& operator()(const Cl Instance, void (Cl::*Func)(TokenType&, typename T1::i_type const&, typename T2::i_type const&, typename T3::i_type const&, typename T4::i_type const&))
    {
      m_Actionp = SmartPtr<func::FunctorMem4<TokenType, typename T1::i_type, typename T2::i_type, typename T3::i_type, typename T4::i_type, Cl> >(new func::FunctorMem4<TokenType, typename T1::i_type, typename T2::i_type, typename T3::i_type, typename T4::i_type, Cl>(Instance, Func));
      return *this;
    }

    Sequence4<Stream, TokenType, T1, T2, T3, T4>& operator()(void (*Action)(TokenType&, typename T1::i_type const&, typename T2::i_type const&, typename T3::i_type const&, typename T4::i_type const&))
    {
      m_Actionp = SmartPtr<func::FunctorFree4<TokenType, typename T1::i_type, typename T2::i_type, typename T3::i_type, typename T4::i_type> >(new func::FunctorFree4<TokenType, typename T1::i_type, typename T2::i_type, typename T3::i_type, typename T4::i_type>(Action));
      return *this;
    }

    virtual ast::SemanticNode<TokenType>* Parse(typename Stream::const_iterator& Start,
						typename Stream::const_iterator const End) const
    {
      typename Stream::const_iterator tmp(Start);
      ast::SemanticNode<typename T1::i_type>* FirstChild(NULL);
      ast::SemanticNode<typename T2::i_type>* SecondChild(NULL);
      ast::SemanticNode<typename T3::i_type>* ThirdChild(NULL);
      ast::SemanticNode<typename T4::i_type>* ForthChild(NULL);
      if((FirstChild = m_s1.Parse(tmp, End)) &&
	 (SecondChild = m_s2.Parse(tmp, End)) &&
	 (ThirdChild = m_s3.Parse(tmp, End)) &&
	 (ForthChild = m_s4.Parse(tmp, End)))
	{
	  Start = tmp;
	  return new ast::Node4<TokenType,typename  T1::i_type,typename  T2::i_type, typename T3::i_type, typename T4::i_type>(FirstChild, SecondChild, ThirdChild,ForthChild, m_Actionp);
	}
      delete FirstChild;
      delete SecondChild;
      delete ThirdChild;
      delete ForthChild;
      return NULL;
    }
    
    template<typename Op>
    Choice<Stream,TokenType, Sequence4<Stream, TokenType, T1, T2, T3, T4>, Op> operator|(const Op& O) const
    {
      return Choice<Stream, TokenType,Sequence4<Stream, TokenType, T1, T2, T3, T4>, Op>(*this, O);
    }
  };

  template<typename Stream, typename TokenType,
	   typename T1,
	   typename T2,
	   typename T3,
	   typename T4,
	   typename T5>
  class Sequence5: public SequenceBase<Stream, TokenType>
  {
    T1 m_s1;
    T2 m_s2;
    T3 m_s3;
    T4 m_s4;
    T5 m_s5;
    
    SmartPtr<func::FunctorBase5<TokenType,
								typename T1::i_type,
								typename T2::i_type,
								typename T3::i_type,
								typename T4::i_type,
								typename T5::i_type> > m_Actionp;
  public:
    Sequence5(const T1& s1,
	      const T2& s2,
	      const T3& s3,
	      const T4& s4,
	      const T5& s5): m_s1(s1),
			     m_s2(s2),
			     m_s3(s3),
			     m_s4(s4),
			     m_s5(s5){}
    virtual ~Sequence5(){}
    
    template<typename Cl>
    Sequence5<Stream, TokenType, T1, T2, T3, T4, T5>& operator()(const Cl Instance, void (Cl::*Func)(TokenType&,
												     typename T1::i_type const&,
												     typename T2::i_type const&,
												     typename T3::i_type const&,
												     typename T4::i_type const&,
												     typename T5::i_type const&))
    {
      m_Actionp = SmartPtr<func::FunctorMem5<TokenType,
		typename T1::i_type,
		typename T2::i_type,
		typename T3::i_type,
		typename T4::i_type,
		typename T5::i_type, Cl> >(new func::FunctorMem5<TokenType,
								   typename T1::i_type,
								   typename T2::i_type,
								   typename T3::i_type,
								   typename T4::i_type,
								   typename T5::i_type, Cl>(Instance, Func));
      return *this;
    }

    Sequence5<Stream, TokenType, T1, T2, T3, T4, T5>& operator()(void (*Action)(TokenType&,
										typename T1::i_type const&,
										typename T2::i_type const&,
										typename T3::i_type const&,
										typename T4::i_type const&,
										typename T5::i_type const&))
    {
      m_Actionp = SmartPtr<func::FunctorFree5<TokenType,
		typename T1::i_type,
		typename T2::i_type,
		typename T3::i_type,
		typename T4::i_type,
		typename T5::i_type> >(new func::FunctorFree5<TokenType,
							   typename T1::i_type,
							   typename T2::i_type,
							   typename T3::i_type,
							   typename T4::i_type,
							   typename T5::i_type>(Action));
      return *this;
    }

    virtual ast::SemanticNode<TokenType>* Parse(typename Stream::const_iterator& Start,
						typename Stream::const_iterator const End) const
    {
      typename Stream::const_iterator tmp(Start);
      ast::SemanticNode<typename T1::i_type>* FirstChild(NULL);
      ast::SemanticNode<typename T2::i_type>* SecondChild(NULL);
      ast::SemanticNode<typename T3::i_type>* ThirdChild(NULL);
      ast::SemanticNode<typename T4::i_type>* ForthChild(NULL);
      ast::SemanticNode<typename T5::i_type>* FithChild(NULL);
      if((FirstChild = m_s1.Parse(tmp, End)) &&
		 (SecondChild = m_s2.Parse(tmp, End)) &&
		 (ThirdChild = m_s3.Parse(tmp, End)) &&
		 (ForthChild = m_s4.Parse(tmp, End)) &&
		 (FithChild = m_s5.Parse(tmp, End)))
		{
		  Start = tmp;
		  return new ast::Node5<TokenType,
			typename T1::i_type,
			typename T2::i_type,
			typename T3::i_type,
			typename T4::i_type,
			typename T5::i_type>(FirstChild, SecondChild, ThirdChild,ForthChild, FithChild, m_Actionp);
		}
      delete FirstChild;
      delete SecondChild;
      delete ThirdChild;
      delete ForthChild;
      delete FithChild;
      return NULL;
    }
    
    template<typename Op>
    Choice<Stream,TokenType, Sequence5<Stream, TokenType, T1, T2, T3, T4, T5>, Op> operator|(const Op& O) const
    {
      return Choice<Stream, TokenType,Sequence5<Stream, TokenType, T1, T2, T3, T4, T5>, Op>(*this, O);
    }
  };
  
  template<typename Stream,
	   typename TokenType,
	   typename T>
  class Ref: public Symbol<Stream, TokenType>
  {
    const T& m_Op;
  public:
    Ref(const T& Op):m_Op(Op){}
    virtual ~Ref(){};
    virtual ast::SemanticNode<TokenType>* Parse(typename Stream::const_iterator& Start,
						typename Stream::const_iterator const End) const
    {
      return m_Op.Parse(Start, End);
    }
    typedef typename T::i_type i_type;
  };

  template<typename TokenType, typename T1,typename T2, typename T3, typename T4, typename T5>
  Sequence5<std::string, TokenType,
	    Ref<std::string,typename T1::i_type, T1>,
	    Ref<std::string,typename T2::i_type, T2>,
	    Ref<std::string,typename T3::i_type, T3>,
	    Ref<std::string,typename T4::i_type, T4>,
	    Ref<std::string,typename T5::i_type, T5> > Seq5(const T1& s1, const T2& s2, const T3& s3, const T4& s4, const T5& s5)
	    {
	      return Sequence5<std::string,TokenType,
		Ref<std::string,typename T1::i_type, T1>,
		Ref<std::string,typename T2::i_type, T2>,
		Ref<std::string,typename T3::i_type, T3>,
		Ref<std::string,typename T4::i_type, T4>,
		Ref<std::string,typename T5::i_type, T5> >(Ref<std::string,typename T1::i_type, T1>(s1),
							   Ref<std::string,typename T2::i_type, T2>(s2),
							   Ref<std::string,typename T3::i_type, T3>(s3),
							   Ref<std::string,typename T4::i_type, T4>(s4),
							   Ref<std::string,typename T5::i_type, T5>(s5));
}
 
  template<typename TokenType, typename T1,typename T2, typename T3, typename T4>
  Sequence4<std::string, TokenType,
	    Ref<std::string,typename T1::i_type, T1>,
	    Ref<std::string,typename T2::i_type, T2>,
	    Ref<std::string,typename T3::i_type, T3>,
	    Ref<std::string,typename T4::i_type, T4> > Seq4(const T1& s1, const T2& s2, const T3& s3, const T4& s4)
	    {
	      return Sequence4<std::string,TokenType,
		Ref<std::string,typename T1::i_type, T1>,
		Ref<std::string,typename T2::i_type, T2>,
		Ref<std::string,typename T3::i_type, T3>,
		Ref<std::string,typename T4::i_type, T4> >(Ref<std::string,typename T1::i_type, T1>(s1),
							   Ref<std::string,typename T2::i_type, T2>(s2),
							   Ref<std::string,typename T3::i_type, T3>(s3),
							   Ref<std::string,typename T4::i_type, T4>(s4));
}

 
  template<typename TokenType, typename T1,typename T2, typename T3>
  Sequence3<std::string, TokenType,
	    Ref<std::string,typename T1::i_type, T1>,
	    Ref<std::string,typename T2::i_type, T2>,
	    Ref<std::string,typename T3::i_type, T3> > Seq3(const T1& s1, const T2& s2, const T3& s3)
	    {
	      return Sequence3<std::string,TokenType,
		Ref<std::string,typename T1::i_type, T1>,
		Ref<std::string,typename T2::i_type, T2>,
		Ref<std::string,typename T3::i_type, T3> >(Ref<std::string,typename T1::i_type, T1>(s1),
						 Ref<std::string,typename T2::i_type, T2>(s2),
						 Ref<std::string,typename T3::i_type, T3>(s3));
}
  
template<typename TokenType,
	 typename T1,
	 typename T2>
Sequence2<std::string, TokenType,
	  Ref<std::string,typename T1::i_type, T1>,
	  Ref<std::string,typename T2::i_type, T2> > Seq2(const T1& s1, const T2& s2)
{
  return Sequence2<std::string,TokenType,
    Ref<std::string,typename T1::i_type, T1>,
    Ref<std::string,typename T2::i_type, T2> >(Ref<std::string,typename T1::i_type, T1>(s1),
					       Ref<std::string,typename T2::i_type, T2>(s2));
}
 
  template<typename TokenType, typename T1>
  Sequence1<std::string,TokenType, Ref<std::string,typename T1::i_type, T1> > Seq1(const T1& s1)
  {
    return Sequence1<std::string,TokenType, Ref<std::string,typename T1::i_type,  T1> >(Ref<std::string,typename T1::i_type, T1>(s1));
  }
}
  
#endif
