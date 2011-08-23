#ifndef _included_mobile_mar_opengl_functions_
#define _included_mobile_mar_opengl_functions_

#include "GlHeaders.h"
#include <string>

namespace mar {
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
};

#endif 
