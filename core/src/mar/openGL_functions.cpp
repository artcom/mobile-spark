#include "openGL_functions.h"

#include <masl/Logger.h>

namespace mar {

    DEFINE_EXCEPTION(ShaderCreationException, masl::Exception)

    //////////////////////////////////////////logging
    void printGLString(const char *name, GLenum s) {
        const char *v = (const char *) glGetString(s);
        AC_INFO << "GL " << name << " = " << v;
    }

    void checkGlError(const char* op) {
        for (GLint error = glGetError(); error; error
                = glGetError()) {
            AC_ERROR << "after " << op << "() glError (" << error << ")";
        }
    }

    void printGLInfo() {
        printGLString("Version", GL_VERSION);
        printGLString("Vendor", GL_VENDOR);
        printGLString("Renderer", GL_RENDERER);
        printGLString("Extensions", GL_EXTENSIONS);
    }


    ///////////////////////////////////////////////////////////////// Shaders
    GLuint loadShader(GLenum shaderType, const char* pSource) {
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
                    char info[infoLen];
                    glGetShaderInfoLog(shader, infoLen, NULL, info);
                    AC_ERROR << "Could not compile shader " << shaderType << ": \n" << info;
                    glDeleteShader(shader);
                    shader = 0;
                    throw ShaderCreationException(std::string("Could not compile shader '") + pSource + std::string("' ") + info, PLUS_FILE_LINE);
                }
                glDeleteShader(shader);
            }
        }
        return shader;
    }

    GLuint createProgram(const std::string & pVertexSource, const std::string & pFragmentSource) {
        GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource.c_str());
        if (!vertexShader) {
            return 0;
        }

        GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource.c_str());
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
                    char info[bufLength];
                    glGetProgramInfoLog(program, bufLength, NULL, info);
                    AC_ERROR << "Could not link program:\n " << info;
                    glDeleteProgram(program);
                    program = 0;
                    throw ShaderCreationException(std::string("Could not link program ") + info, PLUS_FILE_LINE);
                }
                glDeleteProgram(program);
            }
        }
        return program;
    }


     GLuint getShaderVariableHandle(GLuint theProgram, const std::string & theVariableName, bool theUniformFlag) {
         GLuint myHandle;
         if (theUniformFlag) {
             myHandle = glGetUniformLocation(theProgram, theVariableName.c_str());
         } else {
             myHandle = glGetAttribLocation(theProgram, theVariableName.c_str());
         }
         checkGlError(std::string(theUniformFlag ? "glGetUniformLocation" : "glGetAttribLocation").
                             append(" ").append(theVariableName).c_str());
         AC_DEBUG << (theUniformFlag?"glGetUniformLocation":"glGetAttribLocation") << "(\"" << theVariableName << "\") = " << myHandle;
         return myHandle;
     }

     GLuint getShaderVariableHandleAttribute(GLuint theProgram,
                                                    const std::string & theVariableName) {
         return getShaderVariableHandle(theProgram, theVariableName, false);
     }

     GLuint getShaderVariableHandleUniform(GLuint theProgram,
                                                  const std::string & theVariableName) {
         return getShaderVariableHandle(theProgram, theVariableName, true);
     }
}

