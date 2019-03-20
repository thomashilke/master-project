#ifndef TOOL
#define TOOL

class Tool
{
public:
  virtual ~Tool(){}
  typedef SmartPtr<Tool> Ptr;
  
  virtual bool OnMousePressEvent(MousePressEvent& e) = 0;
  virtual bool OnMouseMoveEvent(MouseMoveEvent& e) = 0;
};


class DrawTool: public Tool
{};
class MoveTool: public Tool
{};
class NavigateTool: public Tool
{};

#endif
