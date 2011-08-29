#ifndef _ac_mobile_asl_GlHeaders_h_included_
#define _ac_mobile_asl_GlHeaders_h_included_


#if __APPLE__
    //iOS
    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>
#endif
#ifdef __ANDROID__
    //Android
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
    #include <EGL/egl.h>
    
//    #include <GLES/gl.h>
#endif

#endif
