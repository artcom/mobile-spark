#ifndef _included_mobile_mar_opengl_functions_
#define _included_mobile_mar_opengl_functions_

#include "GlHeaders.h"

#include <masl/Logger.h>

namespace mar {
    //////////////////////////////////////////logging
    static void printGLString(const char *name, GLenum s) {
        const char *v = (const char *) glGetString(s);
        AC_PRINT << "GL " << name << " = " << v;
    }

    static void checkGlError(const char* op) {
        for (GLint error = glGetError(); error; error
                = glGetError()) {
            AC_PRINT << "after " << op << "() glError (0x" << error << ")";
        }
    }

    static void printGLInfo() {
        printGLString("Version", GL_VERSION);
        printGLString("Vendor", GL_VENDOR);
        printGLString("Renderer", GL_RENDERER);
        printGLString("Extensions", GL_EXTENSIONS);
    }


    ///////////////////////////////////////////////////////////////// Shaders
    static GLuint loadShader(GLenum shaderType, const char* pSource) {
        GLuint shader = glCreateShader(shaderType);
        if (shader) {
            glShaderSource(shader, 1, &pSource, NULL);
            glCompileShader(shader);
            GLint compiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
            if (!compiled) {
                GLint infoLen = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
                if (infoLen) {
                    char* buf = (char*) malloc(infoLen);
                    if (buf) {
                        glGetShaderInfoLog(shader, infoLen, NULL, buf);
                        AC_ERROR << "Could not compile shader " << shaderType << ": \n" << buf;
                        free(buf);
                    }
                    glDeleteShader(shader);
                    shader = 0;
                }
            }
        }
        return shader;
    }

    static GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
        GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
        if (!vertexShader) {
            return 0;
        }

        GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
        if (!pixelShader) {
            return 0;
        }

        GLuint program = glCreateProgram();
        if (program) {
            glAttachShader(program, vertexShader);
            checkGlError("glAttachShader");
            glAttachShader(program, pixelShader);
            checkGlError("glAttachShader");
            glLinkProgram(program);
            GLint linkStatus = GL_FALSE;
            glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
            if (linkStatus != GL_TRUE) {
                GLint bufLength = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
                if (bufLength) {
                    char* buf = (char*) malloc(bufLength);
                    if (buf) {
                        glGetProgramInfoLog(program, bufLength, NULL, buf);
                        AC_ERROR << "Could not link program:\n " << buf;
                        free(buf);
                    }
                }
                glDeleteProgram(program);
                program = 0;
            }
        }
        return program;
    }


     static GLuint getShaderVariableHandle(GLuint theProgram, std::string theVariableName, bool theUniformFlag) {
         GLuint myHandle;
         if (theUniformFlag) {
             myHandle = glGetUniformLocation(theProgram, theVariableName.c_str());
         } else {
             myHandle = glGetAttribLocation(theProgram, theVariableName.c_str());
         }
         checkGlError(std::string(theUniformFlag ? "glGetUniformLocation" : "glGetAttribLocation").
                             append(" ").append(theVariableName).c_str());
         AC_PRINT << (theUniformFlag?"glGetUniformLocation":"glGetAttribLocation") << "(\"" << theVariableName << "\") = " << myHandle;
         return myHandle;
     }
 
     static GLuint getShaderVariableHandleAttribute(GLuint theProgram, 
                                                    std::string theVariableName) {
         return getShaderVariableHandle(theProgram, theVariableName, false);
     }
 
     static GLuint getShaderVariableHandleUniform(GLuint theProgram, 
                                                  std::string theVariableName) {
         return getShaderVariableHandle(theProgram, theVariableName, true);
     }

};

#endif 
