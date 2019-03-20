#ifndef X11UTILITIES
#define X11UTILITIES

#include <string>

#include<X11/X.h>
#include<X11/Xlib.h>
#include<X11/Xatom.h>

#include<GL/gl.h>
#include<GL/glx.h>

class X11Display
{
  Display* m_Display;
  Window m_Root;
  
  X11Display(const X11Display&);
  X11Display& operator=(const X11Display&);

public:
  X11Display();
  ~X11Display();

  Display*
  GetDisplay() const;

  Window
  GetRoot() const;
};

inline Display* X11Display::GetDisplay() const {return m_Display;}
inline Window X11Display::GetRoot() const {return m_Root;}

class OpenGLWindow
{
  const X11Display& m_Display;

  XVisualInfo* m_VisualInfo;
  Colormap m_Colormap;
  XSetWindowAttributes m_WindowAttributes;
  Window m_Window;
  Atom m_WmDelete;

  GLXContext m_Context;

  OpenGLWindow(const OpenGLWindow&);
  OpenGLWindow& operator=(const OpenGLWindow&);
public:
  OpenGLWindow(unsigned int Width,
	       unsigned int Height,
	       const X11Display& Display);
  ~OpenGLWindow();
  
  void Show();
  void Hide();
  void SetTitle(const std::string& Title);
  void SwapBuffers();
  Window GetWindow() const;
};

inline void OpenGLWindow::Show()
{XMapWindow(m_Display.GetDisplay(), m_Window); XFlush(m_Display.GetDisplay());}

inline void OpenGLWindow::Hide()
{XUnmapWindow(m_Display.GetDisplay(), m_Window);}

inline void OpenGLWindow::SetTitle(const std::string& Title)
{XStoreName(m_Display.GetDisplay(), m_Window, Title.c_str());}

inline void OpenGLWindow::SwapBuffers()
{glXSwapBuffers(m_Display.GetDisplay(), m_Window);}

inline Window OpenGLWindow::GetWindow() const
{return m_Window;}

#endif
