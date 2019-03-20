#ifndef CEVENTMANAGER
#define CEVENTMANAGER

#include <utils/smartptr.hpp>
#include <utils/tinymatrix/vector.hpp>
#include "x11utilities.hpp"

typedef Vector<int,2> vec2i;

enum MouseEventType{BTN_PRESSED, BTN_RELEASED};
class Event
{
  bool m_Accepted;
public:
  Event(): m_Accepted(false){}
  virtual ~Event(){}
  void Accept(){m_Accepted = true;}
  bool IsAccepted(){return m_Accepted;}
};

class ScrollEvent: public Event
{
  const double m_pos;
  const double m_delta;
  const vec2i m_mousePos;
public:
  ScrollEvent(const vec2i& mousePos,
	      double Position,
	      double Delta): m_pos(Position),
					   m_delta(Delta),
					   m_mousePos(mousePos)
  {}
  const vec2i GetMousePos(){return m_mousePos;}
  double AbsoluteScroll(){return m_pos;}
  double DeltaScroll(){return m_delta;}
};

class MousePressEvent:public Event
{
  const vec2i m_MousePos;
  const MouseEventType m_Type;
public:
  MousePressEvent(const vec2i& MousePos, MouseEventType Type):m_MousePos(MousePos),
							       m_Type(Type)
  {}
  const vec2i& GetMousePos(){return m_MousePos;}
  MouseEventType Type(){return m_Type;}
};
class MouseMoveEvent:public Event
{
  const vec2i m_Delta;
  const vec2i m_NewMousePos;
public:
  MouseMoveEvent(const vec2i& Delta, const vec2i& NewMousePos): m_Delta(Delta),
								 m_NewMousePos(NewMousePos)
  {}
  const vec2i& GetMousePos(){return m_NewMousePos;}
  const vec2i& GetDelta(){return m_Delta;}
};
class KeyboardEvent: public Event
{
public:
  KeyboardEvent(){}
};
class QuitEvent:public Event
{
public:
  QuitEvent(){}
};


template<typename T> // T est un type dérivé de IEvent
class EventDispatcher
{
public:
  virtual ~EventDispatcher(){}

  virtual void OnEvent(T& Event) = 0;
};

template<typename T>
class FreeFuncDispatcher: public EventDispatcher<T>
{
  void (*m_Func)(T&);
public:
  FreeFuncDispatcher(void (*Func)(T&)): m_Func(Func){}
  virtual ~FreeFuncDispatcher(){}

  virtual void OnEvent(T& Event)
  {
    m_Func(Event);
  }
};


template<typename T, typename U>
class MemFuncDispatcher: public EventDispatcher<T>
{
  U& m_Instance;
  void (U::*m_MemFunc)(T&);
public:
  MemFuncDispatcher(void (U::*MemFunc)(T&), U& Instance):m_Instance(Instance), m_MemFunc(MemFunc) {}
  virtual ~MemFuncDispatcher(){}

  virtual void OnEvent(T& Event)
  {
    (m_Instance.*m_MemFunc)(Event);
  }
};

class EventManager
{
protected:
  SmartPtr<EventDispatcher<MouseMoveEvent> > m_MouseMoveDispatcher;
  SmartPtr<EventDispatcher<MousePressEvent> > m_MousePressDispatcher;
  SmartPtr<EventDispatcher<ScrollEvent> > m_ScrollDispatcher;
  SmartPtr<EventDispatcher<KeyboardEvent> > m_KeyboardDispatcher;
  SmartPtr<EventDispatcher<QuitEvent> > m_QuitDispatcher;
public:
  EventManager(){}
  virtual ~EventManager(){}
  virtual void CheckEvents() = 0;


  //MousePress
  void MousePressEventDispatcher(void (*Func)(MousePressEvent&))
  {
    m_MousePressDispatcher = SmartPtr<EventDispatcher<MousePressEvent> >(new FreeFuncDispatcher<MousePressEvent>(Func));
  }
      
  template<typename U>
  void MousePressEventDispatcher(void (U::*Func)(MousePressEvent&), U& Instance)
  {
    m_MousePressDispatcher = SmartPtr<EventDispatcher<MousePressEvent> >(new MemFuncDispatcher<MousePressEvent, U>(Func, Instance));
  }
      
  //MouseMove
  void MouseMoveDispatcher(void (*Func)(MouseMoveEvent&))
  {
    m_MouseMoveDispatcher = SmartPtr<EventDispatcher<MouseMoveEvent> >(new FreeFuncDispatcher<MouseMoveEvent>(Func));
  }
      
  template<typename U>
  void MouseMoveEventDispatcher(void (U::*Func)(MouseMoveEvent&), U& Instance)
  {
    m_MouseMoveDispatcher = SmartPtr<EventDispatcher<MouseMoveEvent> >(new MemFuncDispatcher<MouseMoveEvent, U>(Func, Instance));
  }

  //Scroll
  void ScrollEventDispatcher(void (*Func)(ScrollEvent&))
  {
    m_ScrollDispatcher = SmartPtr<EventDispatcher<ScrollEvent> >(new FreeFuncDispatcher<ScrollEvent>(Func));
  }
      
  template<typename U>
  void ScrollEventDispatcher(void (U::*Func)(ScrollEvent&), U& Instance)
  {
    m_ScrollDispatcher = SmartPtr<EventDispatcher<ScrollEvent> >(new MemFuncDispatcher<ScrollEvent, U>(Func, Instance));
  }
      
  //KeyboardEvent
  void KeyboardEventDispatcher(void (*Func)(KeyboardEvent&))
  {
    m_KeyboardDispatcher = SmartPtr<EventDispatcher<KeyboardEvent> >(new FreeFuncDispatcher<KeyboardEvent>(Func));
  }

  template<typename U>
  void KeyboardEventDispatcher(void (U::*Func)(KeyboardEvent&), U& Instance)
  {
    m_KeyboardDispatcher = SmartPtr<EventDispatcher<KeyboardEvent> >(new MemFuncDispatcher<KeyboardEvent, U>(Func, Instance));
  }
  
  //QuitEvent
  void QuitEventDispatcher(void (*Func)(QuitEvent&))
  {
    m_QuitDispatcher = SmartPtr<EventDispatcher<QuitEvent> >(new FreeFuncDispatcher<QuitEvent>(Func));
  }

  template<typename U>
  void QuitEventDispatcher(void (U::*Func)(QuitEvent&), U& Instance)
  {
    m_QuitDispatcher = SmartPtr<EventDispatcher<QuitEvent> >(new MemFuncDispatcher<QuitEvent, U>(Func, Instance));
  }
};
typedef SmartPtr<EventManager> EventManagerPtr;

class XEventManager: public EventManager
{
  X11Display& m_Display;
  OpenGLWindow& m_Window;

  XEvent m_Event;
  XWindowAttributes m_WindowAttributes;
  vec2i m_MousePos;
  vec2i m_ScreenSize;

  int m_scrollPos;
public:
  XEventManager(const vec2i& ScreenSize,
		X11Display& Display,
		OpenGLWindow& Window):m_Display(Display),
				      m_Window(Window),
				      m_ScreenSize(ScreenSize),
				      m_scrollPos(0)
  {}
  void CheckEvents()
  {
    while(XPending(m_Display.GetDisplay()))
      {
	XNextEvent(m_Display.GetDisplay(), &m_Event);
	if(m_Event.type == Expose)
	  {
	    XGetWindowAttributes(m_Display.GetDisplay(),
				 m_Window.GetWindow(),
				 &m_WindowAttributes);
	  }
	else if(m_Event.type == KeyPress)
	  {
	    KeySym Key(XKeycodeToKeysym(m_Display.GetDisplay(),
					m_Event.xkey.keycode,
					m_Event.xkey.state&ShiftMask?1:0));
	    std::string name(XKeysymToString(Key));
	    /*if((((Key >= 0x0000) && (Key <= 0x00ff)) || ((Key >= 0xff08) && (Key <= 0xffff))) && (m_CharDispatcher))
	      {
	      Key &= 0x00ff;
	      std::string c((char*)&Key, 1);
	      CCharacterEvent Event(name + ": " + c);
	      m_CharDispatcher->OnEvent(Event);
	      }*/
	    if(m_KeyboardDispatcher)
	      {
		KeyboardEvent Event;
		m_KeyboardDispatcher->OnEvent(Event);
	      }
	  }
	else if(m_Event.type == KeyRelease)
	  {
	    if(m_KeyboardDispatcher)
	      {
		KeyboardEvent Event;
		m_KeyboardDispatcher->OnEvent(Event);
	      }
	  }
	else if(m_Event.type == MotionNotify)
	  {
	    if(m_MouseMoveDispatcher)
	      {
		vec2i NewMousePos(m_Event.xmotion.x, m_ScreenSize.at(1)-m_Event.xmotion.y);
		MouseMoveEvent Event(NewMousePos-m_MousePos,
				     NewMousePos);
		m_MousePos = NewMousePos;
		m_MouseMoveDispatcher->OnEvent(Event);
	      }
	  }
	else if(m_Event.type == ButtonPress)
	  {
	    if((m_Event.xbutton.button == Button1 ||
		m_Event.xbutton.button == Button2 ||
		m_Event.xbutton.button == Button3)
		&& m_MousePressDispatcher)
	      {
		m_MousePos = vec2i(m_Event.xbutton.x, m_ScreenSize.at(1)-m_Event.xbutton.y);
		MousePressEvent Event(m_MousePos,
				      BTN_PRESSED);
		m_MousePressDispatcher->OnEvent(Event);
	      }
	    else if(m_Event.xbutton.button == Button4
		    && m_MousePressDispatcher)
	      {
		--m_scrollPos;
		m_MousePos = vec2i(m_Event.xbutton.x, m_ScreenSize.at(1)-m_Event.xbutton.y);
		ScrollEvent Event(m_MousePos, m_scrollPos, -1);
		m_ScrollDispatcher->OnEvent(Event);
	      }
	    else if(m_Event.xbutton.button == Button5
		    && m_MousePressDispatcher)
	      {
		++m_scrollPos;
		m_MousePos = vec2i(m_Event.xbutton.x, m_ScreenSize.at(1)-m_Event.xbutton.y);
		ScrollEvent Event(m_MousePos, m_scrollPos, 1);
		m_ScrollDispatcher->OnEvent(Event);
	      }
	  }
	else if(m_Event.type == ButtonRelease)
	  {
	    if(m_MousePressDispatcher)
	      {
		m_MousePos = vec2i(m_Event.xbutton.x, m_ScreenSize.at(1)-m_Event.xbutton.y);
		MousePressEvent Event(m_MousePos,
				      BTN_RELEASED);
		m_MousePressDispatcher->OnEvent(Event);
	      }
	  }
	else if(m_Event.type == ClientMessage)
	  {
	    std::string AtomName(XGetAtomName(m_Display.GetDisplay(),
					      m_Event.xclient.message_type));
	    if(AtomName == "WM_PROTOCOLS")
	      {
		QuitEvent Event;
		if(m_QuitDispatcher)
		  m_QuitDispatcher->OnEvent(Event);		    
	      }
	  }
      }
  }
};
#endif
