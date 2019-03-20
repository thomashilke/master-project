#ifndef OPENGLEXTS
#define OPENGLEXTS

#include <string>
#include <sstream>

#include <GL/gl.h>
#include <GL/glx.h>
#include "glext.h"

#include <utils/singleton.hpp>

struct OpenGLExtension
{
  template<typename T>
  void Load(T& ptr, const char* Name)
  {
    if(!(ptr = reinterpret_cast<T>(glXGetProcAddress(reinterpret_cast<const GLubyte*>(Name)))))
      throw std::string("failed to load: ") + Name;
  }

  bool CheckExtension(const std::string& Ext)
  {
    std::istringstream Iss(reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS)));
    std::string Tmp;
    while(Iss >> Tmp)
      if(Tmp == Ext)
	return true;
    return false;
  }
};

#define LOADEXT(x) Load(x,#x)

/*struct FramebufferObjectProc: public OpenGLExtension
{
public:
  PFNGLISRENDERBUFFEREXTPROC glIsRenderBufferEXT;
  PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT;
  PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT;
  PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT;
  PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT;
  PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC glGetRenderbufferParameterivEXT;
  PFNGLISFRAMEBUFFEREXTPROC glIsFramebufferEXT;
  PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT;
  PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT;
  PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT;
  PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT;
  PFNGLFRAMEBUFFERTEXTURE1DEXTPROC glFramebufferTexture1DEXT;
  PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT;
  PFNGLFRAMEBUFFERTEXTURE3DEXTPROC glFramebufferTexture3DEXT;
  PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT;
  PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT;
  PFNGLGENERATEMIPMAPEXTPROC glGenerateMipmapEXT;
  PFNGLBLITFRAMEBUFFEREXTPROC glBlitFramebufferEXT;
  PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC glRenderbufferStorageMultisampleEXT;
  PFNGLFRAMEBUFFERTEXTURELAYEREXTPROC glFramebufferTextureLayerEXT;

  FramebufferObjectProc()
  {
    if(!CheckExtension("GL_EXT_framebuffer_object"))
      throw std::string("Extension non supportee: GL_EXT_framebuffer_object");
    
    LOADEXT(glIsRenderBufferEXT);
    LOADEXT(glBindRenderbufferEXT);
    LOADEXT(glDeleteRenderbuffersEXT);
    LOADEXT(glGenRenderbuffersEXT);
    LOADEXT(glRenderbufferStorageEXT);
    LOADEXT(glGetRenderbufferParameterivEXT);
    LOADEXT(glIsFramebufferEXT);
    LOADEXT(glBindFramebufferEXT);
    LOADEXT(glDeleteFramebuffersEXT);
    LOADEXT(glGenFramebuffersEXT);
    LOADEXT(glCheckFramebufferStatusEXT);
    LOADEXT(glFramebufferTexture1DEXT);
    LOADEXT(glFramebufferTexture2DEXT);
    LOADEXT(glFramebufferTexture3DEXT);
    LOADEXT(glFramebufferRenderbufferEXT);
    LOADEXT(glGetFramebufferAttachmentParameterivEXT);
    LOADEXT(glGenerateMipmapEXT);
    LOADEXT(glBlitFramebufferEXT);
    LOADEXT(glRenderbufferStorageMultisampleEXT);
    LOADEXT(glFramebufferTextureLayerEXT);
  }

  bool glExtCheckFramebufferStatus(std::string& Error)
  {
    GLenum status;
    status=glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    switch(status)
      {
      case GL_FRAMEBUFFER_COMPLETE_EXT:
	break;
      case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
	Error = ("Framebuffer object format is unsupported by the video hardware." 
		 "(GL_FRAMEBUFFER_UNSUPPORTED_EXT)(FBO - 820)");
	return false;
      case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
	Error = ("Incomplete attachment. "
		 "(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT)(FBO - 820)");
	return false;
      case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
	Error = ("Incomplete missing attachment. "
		 "(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT)(FBO - 820)");
	return false;
      case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
	Error = ("Incomplete dimensions. "
		 "(GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT)(FBO - 820)");
	return false;
      case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
	Error = ("Incomplete formats. "
		 "(GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT)(FBO - 820)");
	return false;
      case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
	Error = ("Incomplete draw buffer. "
		 "(GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT)(FBO - 820)");
	return false;
      case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
	Error = ("Incomplete read buffer. "
		 "(GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT)(FBO - 820)");
	return false;
      case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT:
	Error = ("Incomplete multisample buffer. "
		 "(GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT)(FBO - 820)");
	return false;
      default:
	Error = ("Some video driver error or programming error occured. "
		 "Framebuffer object status is invalid. (FBO - 823)");
	return false;
      }
    return true;
  }
};*/

struct VertexbufferObjectProc: public OpenGLExtension, public Singleton<VertexbufferObjectProc>
{
  friend class Singleton<VertexbufferObjectProc>;

  VertexbufferObjectProc()
  {
    if(!CheckExtension("GL_ARB_vertex_buffer_object"))
      throw std::string("Extension non supportee: GL_ARB_vertex_buffer_object");

    LOADEXT(glBindBufferARB);
    LOADEXT(glDeleteBuffersARB);
    LOADEXT(glGenBuffersARB);
    LOADEXT(glIsBufferARB);
    LOADEXT(glBufferDataARB);
    LOADEXT(glBufferSubDataARB);
    LOADEXT(glGetBufferSubDataARB);
    LOADEXT(glMapBufferARB);
    LOADEXT(glUnmapBufferARB);
    LOADEXT(glGetBufferParameterivARB);
    LOADEXT(glGetBufferPointervARB);
  }
  ~VertexbufferObjectProc(){}
public:
  PFNGLBINDBUFFERARBPROC glBindBufferARB;
  PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;
  PFNGLGENBUFFERSARBPROC glGenBuffersARB;
  PFNGLISBUFFERARBPROC glIsBufferARB;
  PFNGLBUFFERDATAARBPROC glBufferDataARB;
  PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB;
  PFNGLGETBUFFERSUBDATAARBPROC glGetBufferSubDataARB;
  PFNGLMAPBUFFERARBPROC glMapBufferARB;
  PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB;
  PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB;
  PFNGLGETBUFFERPOINTERVARBPROC glGetBufferPointervARB;
};

/*struct ShaderObjectProc: public OpenGLExtension
{
public:
  PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
  PFNGLGETHANDLEARBPROC glGetHandleARB;
  PFNGLDETACHOBJECTARBPROC glDetachObjectARB;
  PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
  PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
  PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
  PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
  PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
  PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
  PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
  PFNGLVALIDATEPROGRAMARBPROC glValidateProgramARB;
  PFNGLUNIFORM1FARBPROC glUniform1fARB;
  PFNGLUNIFORM2FARBPROC glUniform2fARB;
  PFNGLUNIFORM3FARBPROC glUniform3fARB;
  PFNGLUNIFORM4FARBPROC glUniform4fARB;
  PFNGLUNIFORM1IARBPROC glUniform1iARB;
  PFNGLUNIFORM2IARBPROC glUniform2iARB;
  PFNGLUNIFORM3IARBPROC glUniform3iARB;
  PFNGLUNIFORM4IARBPROC glUniform4iARB;
  PFNGLUNIFORM1FVARBPROC glUniform1fvARB;
  PFNGLUNIFORM2FVARBPROC glUniform2fvARB;
  PFNGLUNIFORM3FVARBPROC glUniform3fvARB;
  PFNGLUNIFORM4FVARBPROC glUniform4fvARB;
  PFNGLUNIFORM1IVARBPROC glUniform1ivARB;
  PFNGLUNIFORM2IVARBPROC glUniform2ivARB;
  PFNGLUNIFORM3IVARBPROC glUniform3ivARB;
  PFNGLUNIFORM4IVARBPROC glUniform4ivARB;
  PFNGLUNIFORMMATRIX2FVARBPROC glUniformMatrix2fvARB; 
  PFNGLUNIFORMMATRIX3FVARBPROC glUniformMatrix3fvARB;
  PFNGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fvARB;
  PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfvARB;
  PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
  PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
  PFNGLGETATTACHEDOBJECTSARBPROC glGetAttachedObjectsARB;
  PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;
  PFNGLGETACTIVEUNIFORMARBPROC glGetActiveUniformARB;
  PFNGLGETUNIFORMFVARBPROC glGetUniformfvARB;
  PFNGLGETUNIFORMIVARBPROC glGetUniformivARB;
  PFNGLGETSHADERSOURCEARBPROC glGetShaderSourceARB;

  PFNGLBINDATTRIBLOCATIONARBPROC glBindAttribLocationARB;
  PFNGLGETACTIVEATTRIBARBPROC glGetActiveAttribARB;
  PFNGLGETATTRIBLOCATIONARBPROC glGetAttribLocationARB;


  ShaderObjectProc()
  {
    if(!CheckExtension("GL_ARB_shader_objects"))
      throw std::string("Extension non supportee: GL_ARB_shader_objects");
    
    LOADEXT(glDeleteObjectARB);
    LOADEXT(glGetHandleARB);
    LOADEXT(glDetachObjectARB);
    LOADEXT(glCreateShaderObjectARB);
    LOADEXT(glShaderSourceARB);
    LOADEXT(glCompileShaderARB);
    LOADEXT(glCreateProgramObjectARB);
    LOADEXT(glAttachObjectARB);
    LOADEXT(glLinkProgramARB);
    LOADEXT(glUseProgramObjectARB);
    LOADEXT(glValidateProgramARB);
    LOADEXT(glUniform1fARB);
    LOADEXT(glUniform2fARB);
    LOADEXT(glUniform3fARB);
    LOADEXT(glUniform4fARB);
    LOADEXT(glUniform1iARB);
    LOADEXT(glUniform2iARB);
    LOADEXT(glUniform3iARB);
    LOADEXT(glUniform4iARB);
    LOADEXT(glUniform1fvARB);
    LOADEXT(glUniform2fvARB);
    LOADEXT(glUniform3fvARB);
    LOADEXT(glUniform4fvARB);
    LOADEXT(glUniform1ivARB);
    LOADEXT(glUniform2ivARB);
    LOADEXT(glUniform3ivARB);
    LOADEXT(glUniform4ivARB);
    LOADEXT(glUniformMatrix2fvARB);
    LOADEXT(glUniformMatrix3fvARB);
    LOADEXT(glUniformMatrix4fvARB);
    LOADEXT(glGetObjectParameterfvARB);
    LOADEXT(glGetObjectParameterivARB);
    LOADEXT(glGetInfoLogARB);
    LOADEXT(glGetAttachedObjectsARB);
    LOADEXT(glGetUniformLocationARB);
    LOADEXT(glGetActiveUniformARB);
    LOADEXT(glGetUniformfvARB);
    LOADEXT(glGetUniformivARB);
    LOADEXT(glGetShaderSourceARB);

    LOADEXT(glBindAttribLocationARB);
    LOADEXT(glGetAttribLocationARB);
    LOADEXT(glGetActiveAttribARB);
  }
  };*/

#undef LOADEXT

#endif
