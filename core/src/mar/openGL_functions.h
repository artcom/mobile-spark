#ifndef _included_mobile_mar_opengl_functions_
#define _included_mobile_mar_opengl_functions_

#include <string>
#include <masl/Exception.h>

#include "GlHeaders.h"

namespace mar {
#define DEBUG_GL
#ifdef DEBUG_GL
    #define ASSERT_GL(what, where) \
    checkGlError(what, where)
#elif DEBUG
    #define ASSERT_GL(what, where) \
    checkGlError(what, where)
#else
    #define ASSERT_GL(what, where)
#endif

    DECLARE_EXCEPTION(ShaderCreationException, masl::Exception)
    DECLARE_EXCEPTION(GLRenderException, masl::Exception)

    //////////////////////////////////////////logging
    void printGLString(const char *name, GLenum s);
    void checkGlError(const std::string & op, const std::string & where = "");
    void printGLInfo();

    ///////////////////////////////////////////////////////////////// Shaders
    GLuint loadShader(GLenum shaderType, const char* pSource);
    GLuint createProgram(const std::string & pVertexSource, const std::string & pFragmentSource);
    GLuint getShaderVariableHandle(GLuint theProgram, const std::string & theVariableName, bool theUniformFlag);
    GLuint getShaderVariableHandleAttribute(GLuint theProgram, const std::string & theVariableName);
    GLuint getShaderVariableHandleUniform(GLuint theProgram, const std::string & theVariableName);

    //////////////////////////////////////////////////////////////// Vertex Buffers
    void createVertexBuffers(GLuint vertexBuffer, GLuint indexBuffer, GLuint theProgram);
};

#endif
