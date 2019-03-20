#include "x11utilities.hpp"

X11Display::X11Display():m_Display(XOpenDisplay(NULL))
{
  if(!m_Display)
    throw std::string("");
  m_Root = DefaultRootWindow(m_Display);
}
X11Display::~X11Display()
{
  XCloseDisplay(m_Display);
  m_Display = NULL;
}

OpenGLWindow::OpenGLWindow(unsigned int Width,
			   unsigned int Height,
			   const X11Display& Display): m_Display(Display),
						       m_VisualInfo(NULL)
{
  GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
  m_VisualInfo = glXChooseVisual(Display.GetDisplay(), 0, att);
  if(!m_VisualInfo)
    throw std::string("");
  m_Colormap = XCreateColormap(Display.GetDisplay(),Display.GetRoot(), m_VisualInfo->visual, AllocNone);
  m_WindowAttributes.event_mask = (ExposureMask
				   | KeyPressMask
				   | ButtonPressMask
				   | ButtonReleaseMask
				   | PointerMotionMask
				   | StructureNotifyMask);
  m_WindowAttributes.colormap = m_Colormap;
  m_Window = XCreateWindow(Display.GetDisplay(),
			   Display.GetRoot(),
			   0,0, Width, Height, 0,
			   m_VisualInfo->depth,
			   InputOutput,
			   m_VisualInfo->visual,
			   CWColormap|CWEventMask,
			   &m_WindowAttributes);
  
  m_WmDelete = XInternAtom(Display.GetDisplay(), "WM_DELETE_WINDOW", true);
  XSetWMProtocols(Display.GetDisplay(), m_Window, &m_WmDelete, 1);
  m_Context = glXCreateContext(Display.GetDisplay(), m_VisualInfo, NULL, GL_TRUE);
  glXMakeCurrent(Display.GetDisplay(), m_Window, m_Context);
}
OpenGLWindow::~OpenGLWindow()
{
  glXMakeCurrent(m_Display.GetDisplay(), None, NULL);
  glXDestroyContext(m_Display.GetDisplay(), m_Context);
  XDestroyWindow(m_Display.GetDisplay(), m_Window);
}
