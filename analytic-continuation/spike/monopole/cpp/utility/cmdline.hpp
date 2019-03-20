#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>

#include "bnf.hpp"

typedef std::map<std::string, std::vector<std::string> > Map;
typedef std::vector<std::string> Vec;

struct Def
{
  std::string m_Name;
  std::vector<std::string> m_Values;
};

void parametre1(Map& r, const Def& d, const Map& s)
{
  r = s;
  r[d.m_Name] = d.m_Values;
}

void parametre2(Map& r, const Def& d)
{
  r[d.m_Name] = d.m_Values;
}

void definition(Def& r, const std::string& s, const char& egal, const std::vector<std::string>& v)
{
  r.m_Name = s;
  r.m_Values = v;
}
void liste0(Vec& l, const Vec& r, const char&, const std::vector<std::string>& v)
{
  l = r;
  l.insert(l.end(), v.begin(), v.end());
}

void liste1(Vec& r, const std::string& s, const char& virgule, const std::vector<std::string>& v)
{
  r.push_back(s);
  r.insert(r.end(), v.begin(), v.end());
}
void liste2(Vec& r, const std::string& s)
{
  r.push_back(s);
}

void liste3(Vec& r, const std::vector<std::string>& s)
{
  r = s;
}

void convert(std::string& r, const std::string& s)
{
  r=s;
}

void range(std::vector<std::string>& r, const std::string& s1, const char&, const std::string& s2, const char&, const std::string& s3)
{
  std::istringstream Iss(s1);
  double a(0.0), b(0.0), i(0.0);
  Iss >> a; Iss.clear(), Iss.str(s2);
  Iss >> i; Iss.clear(), Iss.str(s3);
  Iss >> b;

  double tmp(a);
  std::ostringstream Oss;
  while(tmp <= b)
    {
      Oss.str("");
      Oss << tmp;
      r.push_back(Oss.str());
      tmp += i;
    }
}

class CmdLine
{
  typedef regex::Regex<std::string> Regex;
  typedef regex::Char<std::string> c;
  typedef regex::Num<std::string> n;
  typedef regex::Alpha<std::string> a;
  
  typedef bnf::Terminal<std::string, char> Separator;
  typedef bnf::Terminal<std::string, std::string> Identite;
  typedef bnf::NonTerminal<std::string, Vec> NTVec;
  typedef bnf::NonTerminal<std::string, Def> NTDef;
  typedef bnf::NonTerminal<std::string, Map> NTMap;
  
  Map m_Args;
  
  Regex I;
  Regex V;

  Identite Ident;
  Separator Egal;
  Separator Virgule;
  Separator Colon;
  Identite Value;

  NTMap Parametres;
  NTDef Definition;
  NTVec Liste;
  NTVec Range;

  bool Parse(const std::string& s)
  {
    std::string r;
    
    ast::SemanticNode<Map>* Ast(NULL);
    std::string::const_iterator i(s.begin());
    if(Ast = Parametres.Parse(i, s.end()))
      {
	Ast->Execute(m_Args);
	delete Ast;
	return true;
      }
    return false;
  }
  bool Ask(const std::string& Name)
  {
    std::cerr << "Entrez une liste de valeurs pour le parametre \"" << Name << "\": ";
    std::string list;
    if(!std::cin)
      throw std::string("entree standard fermee.");
    std::getline(std::cin, list);
    if(list.size())
      {
	std::vector<std::string> vec;
	ast::SemanticNode<std::vector<std::string> >* Ast(NULL);
	std::string::const_iterator i(list.begin());
	if(Ast = Liste.Parse(i, list.end()))
	  {
	    if(i == list.end())
	      {
		Ast->Execute(vec);
		delete Ast;
		if(vec.size())
		  {
		    m_Args[Name] = vec;
		    return true;
		  }
	      }
	    else
	      delete Ast;
	  }
      }
    return false;
  }
public:
  CmdLine(int argc, char** argv): I((a()|c('_'))>>*(a()|c('_')|n())),
				  V( (n()|a()|c('.')|c('_')|c('-')) >>*(n()|a()|c('.')|c('_')|c('-')) ),
				  Ident(I),
				  Egal(c('=')),
				  Virgule(c(',')),
				  Colon(c(':')),
				  Value(V)
  {
    Parametres = (bnf::Seq2<Map,NTDef,NTMap>(Definition, Parametres)(parametre1)|
		  bnf::Seq1<Map,NTDef>(Definition)(parametre2));
    Definition = (bnf::Seq3<Def,Identite,Separator,NTVec>(Ident(convert), Egal, Liste)(definition));
		  
    Liste = (bnf::Seq3<Vec, NTVec, Separator, NTVec>(Range, Virgule, Liste)(liste0)|
	     bnf::Seq3<Vec,Identite,Separator,NTVec>(Value(convert), Virgule, Liste)(liste1)|
	     bnf::Seq1<Vec,NTVec>(Range)(liste3)|
	     bnf::Seq1<Vec,Identite>(Value(convert))(liste2));
    
    Range = (bnf::Seq5<Vec,Identite,Separator,Identite,Separator,Identite>(Value(convert),Colon, Value(convert), Colon, Value(convert))(range));

    std::ostringstream Oss;
    for(unsigned int i(1); i<argc; ++i)
      {
	Oss << argv[i] << " ";
      }
    Parse(Oss.str());
  }

  template<typename T>
  std::vector<T> Get(const std::string& Name) 
  {
    Map::const_iterator i(m_Args.find(Name));
    if(i != m_Args.end())
      {
	const std::vector<std::string>& v(i->second);
	std::vector<T> Result(v.size());
	std::istringstream Iss;
	for(unsigned int i(0);i<Result.size();++i)
	  {
	    Iss.str(v[i]);
	    Iss.clear();
	    if(!(Iss >> Result[i]))
	      throw std::string("impossible de convertir une chaine de caractere en ") + typeid(T).name();
	  }
	return Result;
      }
    else
      {
	while(!Ask(Name))
	  {std::cout << "syntaxe invalide, entrez une liste ou un intervalle" << std::endl;}
	i = m_Args.find(Name);
	
	const std::vector<std::string>& v(i->second);
	std::vector<T> Result(v.size());
	std::istringstream Iss;
	for(unsigned int i(0);i<Result.size();++i)
	  {
	    Iss.str(v[i]);
	    Iss.clear();
	    if(!(Iss >> Result[i]))
	      throw std::string("impossible de convertir une chaine de caractere en ") + typeid(T).name();
	  }
	return Result;
      }
  }
};
