#include "renderer.hpp"

GLenum mat[] = {GL_MODELVIEW, GL_PROJECTION, GL_TEXTURE};


BasicRenderer::BasicRenderer(unsigned int width,
			     unsigned int height,
			     bool fullscreen):m_width(width), m_height(height),
					      m_vbo(NULL),
					      m_disp(), m_window(m_width, m_height, m_disp),
					      m_eventManager(NULL)
{
  m_vbo = VertexbufferObjectProc::Instance();
  m_eventManager = new XEventManager(vec2i(m_width, m_height), m_disp, m_window);
  m_window.Show();
  
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_ALPHA_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glClearColor(1.,1.,1.,1.);
}



void BasicRenderer::LoadMatrix(MatrixType mt, const mat4& m)
{
  glMatrixMode(mat[mt]);
  glLoadMatrixd(&m.at(0,0));
}

void BasicRenderer::MultMatrix(MatrixType mt, const mat4& m)
{
  glMatrixMode(mat[mt]);
  glMultMatrixd(&m.at(0,0));
}

void BasicRenderer::PushMatrix(MatrixType mt)
{
  glMatrixMode(mat[mt]);
  glPushMatrix();
}

void BasicRenderer::PopMatrix(MatrixType mt)
{
  glMatrixMode(mat[mt]);
  glPopMatrix();
}

void BasicRenderer::BeginFrame()
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_width, m_height);
}

void BasicRenderer::EndFrame()
{
  glFlush();
  m_window.SwapBuffers();
}

void BasicRenderer::DrawPrimitives(PrimitiveType pt,
				   unsigned int PrimitiveNumber)
{
  switch(pt)
    {
    case PRI_POINTS:	  glDrawArrays(GL_POINTS,     0, PrimitiveNumber); break;
    case PRI_LINES:	  glDrawArrays(GL_LINES,      0, PrimitiveNumber+1); break;
    case PRI_LINELOOP:  glDrawArrays(GL_LINE_STRIP, 0, PrimitiveNumber*2); break;
    case PRI_TRIANGLES: glDrawArrays(GL_TRIANGLES,  0, PrimitiveNumber*3); break;
    }
}


