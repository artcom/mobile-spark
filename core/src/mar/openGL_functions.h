#ifndef _included_mobile_mar_opengl_functions_
#define _included_mobile_mar_opengl_functions_

#include <string>
#include <masl/Exception.h>

#include "GlHeaders.h"

namespace mar {
    DEFINE_EXCEPTION(ShaderCreationException, masl::Exception)

    //////////////////////////////////////////logging
    void printGLString(const char *name, GLenum s);
    void checkGlError(const char* op);
    void printGLInfo();

    ///////////////////////////////////////////////////////////////// Shaders
    GLuint loadShader(GLenum shaderType, const char* pSource);
    GLuint createProgram(const std::string & pVertexSource, const std::string & pFragmentSource);
    GLuint getShaderVariableHandle(GLuint theProgram, std::string theVariableName, bool theUniformFlag);
    GLuint getShaderVariableHandleAttribute(GLuint theProgram, std::string theVariableName);
    GLuint getShaderVariableHandleUniform(GLuint theProgram, std::string theVariableName);

    //////////////////////////////////////////////////////////////// Vertex Buffers
    void createVertexBuffers(GLuint vertexBuffer, GLuint indexBuffer, GLuint theProgram);
};

#endif
