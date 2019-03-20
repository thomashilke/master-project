#ifndef REGEX
#define REGEX

namespace regex
{
  template<typename Stream>
  class MatchClass
  {
  public:
    virtual ~MatchClass()
    {}
    
    virtual bool Match(typename Stream::const_iterator& Start,
		       typename Stream::const_iterator const End) const = 0;
    virtual MatchClass<Stream>* Clone() = 0;
  };

  template<typename Stream, typename Left, typename Right>
  class Alternation;

  template<typename Stream, typename Left, typename Right>
  class Concatenation;

  template<typename Stream, typename Pattern>
  class KleeneStar;

  template<typename Stream,
	   typename Left,
	   typename Right>
  class Alternation: public MatchClass<Stream>
  {
    Left m_Left;
    Right m_Right;
  public:
    Alternation(const Left& L, const Right& R): m_Left(L),
						m_Right(R)
    {}

    virtual MatchClass<Stream>* Clone()
    {
      return new Alternation<Stream, Left, Right>(m_Left, m_Right);
    }

    virtual bool Match(typename Stream::const_iterator& Start,
		       typename Stream::const_iterator const End) const
    {
      typename Stream::const_iterator tmp(Start);
      if(m_Left.Match(tmp, End))
	{
	  Start = tmp;
	  return true;
	}
      else if(m_Right.Match(tmp = Start, End))
	{
	  Start = tmp;
	  return true;
	}
      else
	return false;
    }
  
    template<typename U>
    Alternation<Stream, Alternation<Stream, Left, Right>, U > operator|(const U& Op)
    {
      return Alternation<Stream, Alternation<Stream, Left, Right>, U >(*this, Op);
    }

    template<typename U>
    Concatenation<Stream, Alternation<Stream, Left, Right>, U > operator>>(const U& Op);

    KleeneStar<Stream, Alternation<Stream, Left, Right> > operator*();
  };

  template<typename Stream,
	   typename Left,
	   typename Right>
  class Concatenation: public MatchClass<Stream>
  {
    Left m_First;
    Right m_Next;
  public:
    Concatenation(const Left& F, const Right& N): m_First(F),
						  m_Next(N)
    {}
  
    virtual MatchClass<Stream>* Clone()
    {
      return new Concatenation<Stream, Left, Right>(m_First, m_Next);
    }

    virtual bool Match(typename Stream::const_iterator& Start,
		       typename Stream::const_iterator const End) const
    {
      typename Stream::const_iterator tmp(Start);
      if(m_First.Match(tmp, End) && m_Next.Match(tmp, End))
	{
	  Start = tmp;
	  return true;
	}
      else
	return false;
    }

    template<typename U>
    Concatenation<Stream, Concatenation<Stream, Left, Right>, U > operator>>(const U& Op)
    {
      return Concatenation<Stream, Concatenation<Stream, Left, Right>, U >(*this, Op);
    }

    template<typename U>
    Alternation<Stream, Concatenation<Stream, Left, Right>, U > operator|(const U& Op)
    {
      return Alternation<Stream, Concatenation<Stream, Left, Right>, U >(*this, Op);
    }

    KleeneStar<Stream, Concatenation<Stream, Left, Right> > operator*();
  };

  template<typename Stream,
	   typename Pattern>
  class KleeneStar: public MatchClass<Stream>
  {
    Pattern m_Pattern;
  public:
    KleeneStar(const Pattern& P): m_Pattern(P)
    {}
  
    virtual MatchClass<Stream>* Clone()
    {
      return new KleeneStar<Stream, Pattern>(m_Pattern);
    }

    virtual bool Match(typename Stream::const_iterator& Start,
		       typename Stream::const_iterator const End) const
    {
      typename Stream::const_iterator tmp(Start);
      while(m_Pattern.Match(tmp, End))
	{
	  Start = tmp;
	}
      return true;
    }

    template<typename U>
    Concatenation<Stream, KleeneStar<Stream, Pattern>, U > operator>>(const U& Op)
    {
      return Concatenation<Stream, KleeneStar<Stream, Pattern>, U >(*this, Op);
    }

    template<typename U>
    Alternation<Stream, KleeneStar<Stream, Pattern>, U > operator|(const U& Op)
    {
      return Alternation<Stream, KleeneStar<Stream, Pattern>, U >(*this, Op);
    }
  };

  template<typename Stream, typename Left, typename Right>
  template<typename U>
  Concatenation<Stream, Alternation<Stream, Left, Right>, U > Alternation<Stream, Left, Right>::operator>>(const U& Op)
  {
    return Concatenation<Stream, Alternation<Stream, Left, Right>, U >(*this, Op);
  }

  template<typename Stream, typename Left, typename Right>
  KleeneStar<Stream, Alternation<Stream, Left, Right> > Alternation<Stream, Left, Right>::operator*()
  {
    return KleeneStar<Stream, Alternation<Stream, Left, Right> >(*this);
  }

  template<typename Stream, typename Left, typename Right>
  KleeneStar<Stream, Concatenation<Stream, Left, Right> > Concatenation<Stream, Left, Right>::operator*()
  {
    return KleeneStar<Stream, Concatenation<Stream, Left, Right> >(*this);
  }

  
  template<typename Stream>
  class Char: public MatchClass<Stream>
  {
    char m_Char;
  public:
    Char(char c): m_Char(c)
    {}

    virtual MatchClass<Stream>* Clone()
    {
      return new Char<Stream>(m_Char);
    }

    virtual bool Match(typename Stream::const_iterator& Start,
		       typename Stream::const_iterator const End) const 
    {
      if(((Start) != End) && (*(Start) == m_Char))
	{
	  ++Start;
	  return true;
	}
      else
	return false;
    }

    template<typename U>
    Concatenation<Stream, Char<Stream>, U> operator>>(const U& Op)
    {
      return Concatenation<Stream, Char<Stream>, U>(*this, Op);
    }

    template<typename U>
    Alternation<Stream, Char<Stream>, U> operator |(const U& Op)
    {
      return Alternation<Stream, Char<Stream>, U>(*this, Op);
    }

    KleeneStar<Stream, Char<Stream> > operator*()
    {
      return KleeneStar<Stream, Char<Stream> >(*this);
    }
  };

  template<typename Stream>
  class Str: public MatchClass<Stream>
  {
    std::string m_String;
  public:
    Str(const std::string& c): m_String(c)
    {}

    virtual MatchClass<Stream>* Clone()
    {
      return new Str<Stream>(m_String);
    }

    virtual bool Match(typename Stream::const_iterator& Start,
		       typename Stream::const_iterator const End) const 
    {
      typename Stream::const_iterator tmp(Start);
      unsigned int i(0);
      while(i<m_String.size())
	{
	  if((tmp != End) && (*tmp == m_String[i]))
	    {
	      ++i;
	      ++tmp;
	    }
	  else
	    return false;
	}
      Start = tmp;
      return true;
    }

    template<typename U>
    Concatenation<Stream, Str<Stream>, U> operator>>(const U& Op)
    {
      return Concatenation<Stream, Str<Stream>, U>(*this, Op);
    }

    template<typename U>
    Alternation<Stream, Str<Stream>, U> operator |(const U& Op)
    {
      return Alternation<Stream, Str<Stream>, U>(*this, Op);
    }

    KleeneStar<Stream, Str<Stream> > operator*()
    {
      return KleeneStar<Stream, Str<Stream> >(*this);
    }
  };

  template<typename Stream>
  class NotChar: public MatchClass<Stream>
  {
    char m_Char;
  public:
    NotChar(char c): m_Char(c)
    {}

    virtual MatchClass<Stream>* Clone()
    {
      return new NotChar<Stream>(m_Char);
    }

    virtual bool Match(typename Stream::const_iterator& Start,
		       typename Stream::const_iterator const End) const 
    {
      if(((Start) != End) && (*(Start) != m_Char))
	{
	  ++Start;
	  return true;
	}
      else
	return false;
    }

    template<typename U>
    Concatenation<Stream, NotChar<Stream>, U> operator>>(const U& Op)
    {
      return Concatenation<Stream, NotChar<Stream>, U>(*this, Op);
    }

    template<typename U>
    Alternation<Stream, NotChar<Stream>, U> operator |(const U& Op)
    {
      return Alternation<Stream, NotChar<Stream>, U>(*this, Op);
    }

    KleeneStar<Stream, NotChar<Stream> > operator*()
    {
      return KleeneStar<Stream, NotChar<Stream> >(*this);
    }
  };

  template<typename Stream>
  class Alpha: public MatchClass<Stream>
  {
  public:
    virtual ~Alpha(){}

    virtual MatchClass<Stream>* Clone()
    {
      return new Alpha<Stream>;
    }

    virtual bool Match(typename Stream::const_iterator& Start,
		       typename Stream::const_iterator const End) const 
    {
      if(((Start) != End) && (((*Start >= 'a') && (*Start <= 'z'))||((*Start >= 'A') && (*Start <= 'Z'))))
	{
	  ++Start;
	  return true;
	}
      else
	return false;
    }

    template<typename U>
    Concatenation<Stream, Alpha<Stream>, U> operator>>(const U& Op)
    {
      return Concatenation<Stream, Alpha<Stream>, U>(*this, Op);
    }

    template<typename U>
    Alternation<Stream, Alpha<Stream>, U> operator |(const U& Op)
    {
      return Alternation<Stream, Alpha<Stream>, U>(*this, Op);
    }

    KleeneStar<Stream, Alpha<Stream> > operator*()
    {
      return KleeneStar<Stream, Alpha<Stream> >(*this);
    }
  };

  template<typename Stream>
  class Num: public MatchClass<Stream>
  {
  public:
    virtual ~Num(){}

    virtual MatchClass<Stream>* Clone()
    {
      return new Num<Stream>;
    }

    virtual bool Match(typename Stream::const_iterator& Start,
		       typename Stream::const_iterator const End) const 
    {
      if(((Start) != End) && (((*Start >= '0') && (*Start <= '9'))))
	{
	  ++Start;
	  return true;
	}
      else
	return false;
    }

    template<typename U>
    Concatenation<Stream, Num<Stream>, U> operator>>(const U& Op)
    {
      return Concatenation<Stream, Num<Stream>, U>(*this, Op);
    }

    template<typename U>
    Alternation<Stream, Num<Stream>, U> operator |(const U& Op)
    {
      return Alternation<Stream, Num<Stream>, U>(*this, Op);
    }

    KleeneStar<Stream, Num<Stream> > operator*()
    {
      return KleeneStar<Stream, Num<Stream> >(*this);
    }
  };

  template<typename Stream>
  class EndOfFile: public MatchClass<Stream>
  {
  public:
    
    virtual ~EndOfFile(){}

    virtual MatchClass<Stream>* Clone()
    {
      return new EndOfFile<Stream>();
    }
    virtual bool Match(typename Stream::const_iterator& Start,
		       typename Stream::const_iterator const End) const 
    {
      return Start == End;
    }

    template<typename U>
    Concatenation<Stream, EndOfFile<Stream>, U> operator>>(const U& Op)
    {
      return Concatenation<Stream, EndOfFile<Stream>, U>(*this, Op);
    }

    template<typename U>
    Alternation<Stream, EndOfFile<Stream>, U> operator |(const U& Op)
    {
      return Alternation<Stream, EndOfFile<Stream>, U>(*this, Op);
    }

    KleeneStar<Stream, EndOfFile<Stream> > operator*()
    {
      return KleeneStar<Stream, EndOfFile<Stream> >(*this);
    }
  };


  template<typename Stream>
  class Epsilon: public MatchClass<Stream>
  {
  public:
    virtual ~Epsilon(){}

    virtual MatchClass<Stream>* Clone()
    {
      return new Epsilon<Stream>();
    }

    virtual bool Match(typename Stream::const_iterator&,
		       typename Stream::const_iterator const) const 
    {
      return true;
    }

    template<typename U>
    Concatenation<Stream, Epsilon<Stream>, U> operator>>(const U& Op)
    {
      return Concatenation<Stream, Epsilon<Stream>, U>(*this, Op);
    }

    template<typename U>
    Alternation<Stream, Epsilon<Stream>, U> operator |(const U& Op)
    {
      return Alternation<Stream, Epsilon<Stream>, U>(*this, Op);
    }

    KleeneStar<Stream, Epsilon<Stream> > operator*()
    {
      return KleeneStar<Stream, Epsilon<Stream> >(*this);
    }
  };

  template<typename Pattern>
  bool Match(const std::string& s, std::string& r, const Pattern& P)
  {
    std::string::const_iterator i(s.begin());
  
    if(P.Match(i, s.end()))
      {
	r = std::string(s.begin(), i);
	return true;
      }
    else
      return false;
  }

  template<typename Stream, typename Pattern>
  class Holder: public MatchClass<Stream>
  {
    Pattern m_Pattern;
  public:
    virtual ~Holder(){}
    Holder(const Pattern& P): m_Pattern(P)
    {}
  
    virtual MatchClass<Stream>* Clone()
    {
      return new Holder<Stream, Pattern>(m_Pattern);
    }
    virtual bool Match(typename Stream::const_iterator& Start,
		       typename Stream::const_iterator const End) const 
    {
      return m_Pattern.Match(Start, End);
    }
  };

  template<typename Stream>
  class Regex: public MatchClass<Stream>
  {
    MatchClass<Stream> *m_Pattern;
  public:
    template<typename U>
    Regex(const U& P)
    {
      m_Pattern = new Holder<Stream, U>(P);
    }
    Regex(const Regex<Stream>& Op): m_Pattern(NULL)
    {
      m_Pattern = Op.m_Pattern->Clone();
    }
    virtual ~Regex()
    {
      delete m_Pattern;
    }
    Regex<Stream>& operator=(const Regex<Stream>& Op)
    {
      delete m_Pattern;
      m_Pattern = Op.m_Pattern->Clone();
      return *this;
    }
    virtual MatchClass<Stream>* Clone()
    {
      return new Regex<Stream>(*this);
    }
    virtual bool Match(typename Stream::const_iterator& Start,
		       typename Stream::const_iterator const End) const 
    {
      return m_Pattern->Match(Start, End);
    }

    template<typename U>
    Concatenation<Stream, Regex<Stream>, U> operator>>(const U& Op)
    {
      return Concatenation<Stream, Regex<Stream>, U>(*this, Op);
    }

    template<typename U>
    Alternation<Stream, Regex<Stream>, U> operator |(const U& Op)
    {
      return Alternation<Stream, Regex<Stream>, U>(*this, Op);
    }

    KleeneStar<Stream, Regex<Stream> > operator*()
    {
      return KleeneStar<Stream, Regex<Stream> >(*this);
    }
  };
}

#endif
