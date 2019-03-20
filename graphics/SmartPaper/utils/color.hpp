#ifndef COLOR
#define COLOR

#include <string>

enum PixelStructure{PIX_L8 = 0, PIX_L8A8, PIX_R8G8B8, PIX_R8G8B8A8};

template<PixelStructure targetStructure, PixelStructure sourceStructure>
void ConvertPixel(unsigned char* tgt, const unsigned char* src)
{ throw std::string("ConvertPixel() - Pixel conversion not implemented."); }

template<>
inline void ConvertPixel<PIX_L8, PIX_L8>(unsigned char* tgt, const unsigned char* src)
{ *tgt = *src; }

template<>
inline void ConvertPixel<PIX_L8A8, PIX_L8A8>(unsigned char* tgt, const unsigned char* src)
{ *(tgt) = *(src); *(tgt+1) = *(src+1); }

template<>
inline void ConvertPixel<PIX_R8G8B8, PIX_R8G8B8>(unsigned char* tgt, const unsigned char* src)
{ *(tgt) = *(src); *(tgt+1) = *(src+1);*(tgt+2) = *(src+2); }

template<>
inline void ConvertPixel<PIX_R8G8B8A8, PIX_R8G8B8A8>(unsigned char* tgt, const unsigned char* src)
{ *(tgt) = *(src); *(tgt+1) = *(src+1);*(tgt+2) = *(src+2);*(tgt+3) = *(src+3); }
 
template<>
inline void ConvertPixel<PIX_L8, PIX_L8A8>(unsigned char* tgt, const unsigned char* src)
{ *tgt = *src; }

template<>
inline void ConvertPixel<PIX_L8, PIX_R8G8B8>(unsigned char* tgt, const unsigned char* src)
{ *tgt = 0.3f*src[0]+0.59f*src[1]+0.11f*src[2]; }

template<>
inline void ConvertPixel<PIX_L8, PIX_R8G8B8A8>(unsigned char* tgt, const unsigned char* src)
{ *tgt = 0.3f*src[0]+0.59f*src[1]+0.11f*src[2];
}

template<>
inline void ConvertPixel<PIX_L8A8, PIX_L8>(unsigned char* tgt, const unsigned char* src)
{ *tgt = *src; tgt[1] = 255; }

template<>
inline void ConvertPixel<PIX_L8A8, PIX_R8G8B8>(unsigned char* tgt, const unsigned char* src)
{ tgt[1] = 255; *tgt = 0.3f*src[0]+0.59f*src[1]+0.11f*src[2]; }

template<>
inline void ConvertPixel<PIX_L8A8, PIX_R8G8B8A8>(unsigned char* tgt, const unsigned char* src)
{ tgt[1] = src[3]; *tgt = 0.3f*src[0]+0.59f*src[1]+0.11f*src[2]; }

template<>
inline void ConvertPixel<PIX_R8G8B8, PIX_L8A8>(unsigned char* tgt, const unsigned char* src)
{ tgt[0] = src[0]; tgt[1] = src[0]; tgt[2] = src[0]; }

template<>
inline void ConvertPixel<PIX_R8G8B8, PIX_L8>(unsigned char* tgt, const unsigned char* src)
{ tgt[0] = src[0]; tgt[1] = src[0]; tgt[2] = src[0]; }

template<>
inline void ConvertPixel<PIX_R8G8B8, PIX_R8G8B8A8>(unsigned char* tgt, const unsigned char* src)
{ tgt[0] = src[0]; tgt[1] = src[1]; tgt[2] = src[2]; }

template<>
inline void ConvertPixel<PIX_R8G8B8A8, PIX_L8A8>(unsigned char* tgt, const unsigned char* src)
{ tgt[0] = src[0]; tgt[1] = src[0]; tgt[2] = src[0]; tgt[3] = src[1]; }

template<>
inline void ConvertPixel<PIX_R8G8B8A8, PIX_R8G8B8>(unsigned char* tgt, const unsigned char* src)
{ tgt[0] = src[0]; tgt[1] = src[1]; tgt[2] = src[2]; tgt[3]=255; }

template<>
inline void ConvertPixel<PIX_R8G8B8A8, PIX_L8>(unsigned char* tgt, const unsigned char* src)
{ tgt[0] = src[0]; tgt[1] = src[0]; tgt[2] = src[0]; tgt[3] = 255; }

class Color
{
  unsigned char m_array[4];
public:
  Color(unsigned char r,
        unsigned char g,
        unsigned char b,
        unsigned char a)
  {
    m_array[0] = r;
    m_array[1] = g;
    m_array[2] = b;
    m_array[3] = a;
  }

  /*  template<PixelStructure ps>
  Color(const unsigned char* src, const PixelStructure ps)
  { ConvertPixel<PIX_R8G8B8A8, ps>(m_array, src); }*/

  unsigned char& red(){return m_array[0];}
  const unsigned char& red()const{return m_array[0];}

  unsigned char& green(){return m_array[1];}
  const unsigned char& green()const{return m_array[1];}

  unsigned char& blue(){return m_array[2];}
  const unsigned char& blue()const{return m_array[2];}

  unsigned char& alpha(){return m_array[3];}
  const unsigned char& alpha()const{return m_array[3];}

  static Color LinInterp(const Color& c1, const Color& c2, double t)
  {
    return Color(t*c1.red()  +(1.-t)*c2.red(),
	         t*c1.green()+(1.-t)*c2.green(),
		 t*c1.blue() +(1.-t)*c2.blue(),
		 t*c1.alpha()+(1.-t)*c2.alpha());

  }
  unsigned char* Raw(){return m_array;}
  const unsigned char* Raw() const{return m_array;}
};

template<PixelStructure ps>
Color MakeColor(const unsigned char* src)
{ 
  Color c(0,0,0,0);
  ConvertPixel<PIX_R8G8B8A8, ps>(c.Raw(), src);
  return c;
}

#endif
