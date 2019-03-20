#ifndef DOCUMENT
#define DOCUMENT

#include <renderer/renderer.hpp>
#include <utils/smartptr.hpp>
#include <utils/tuple.hpp>


#include "graphicalinterface.hpp"
#include "canevas.hpp"
#include "tool.hpp"

class Document
{
  BasicRenderer m_rend;
  bool m_shouldQuit;
  
  Tool::Ptr m_activeTool;
  GraphicalInterface::Ptr m_gui;
public:
  Document(): m_rend(500,500,false), m_shouldQuit(false)
  {
    m_rend.getEventManager()
      .MousePressEventDispatcher(&Document::OnMousePressEvent, *this);
    m_rend.getEventManager()
      .MouseMoveEventDispatcher(&Document::OnMouseMoveEvent, *this);
    m_rend.getEventManager()
      .KeyboardEventDispatcher(&Document::OnKeyboardEvent, *this);
    m_rend.getEventManager()
      .QuitEventDispatcher(&Document::OnQuitEvent, *this);
  }
  ~Document(){}
  
  void Run()
    {
      while(!m_shouldQuit)
	{
	  m_rend.getEventManager().CheckEvents();
	  m_rend.BeginFrame();


	  m_rend.EndFrame();
	}
    }
  void OnMouseMoveEvent(MouseMoveEvent& e)
  { if(m_activeTool) m_activeTool->OnMouseMoveEvent(e); }
  void OnMousePressEvent(MousePressEvent& e)
  { 
    std::cout << "mouse press event" << std::endl;
    if(m_gui && !m_gui->OnMousePressEvent(e))
      if(m_activeTool)
	m_activeTool->OnMousePressEvent(e);
  }
  void OnKeyboardEvent(KeyboardEvent& e)
  {
    std::cout << "key event" << std::endl;
    if(m_gui)
      m_gui->OnKeyboardEvent(e);
  }
  void OnQuitEvent(QuitEvent& e)
  {
    std::cout << "quit event" << std::endl;    
    m_shouldQuit = true;
  }  
};

#endif
