#ifndef GRAPHICALINTERFACE
#define GRAPHICALINTERFACE

class GraphicalInterface
{
public:
  typedef SmartPtr<GraphicalInterface> Ptr;
  
  bool OnMousePressEvent(MousePressEvent& e){}
  bool OnKeyboardEvent(KeyboardEvent& e){}
};

#endif
