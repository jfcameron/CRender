#ifndef _VERTEX_FORMAT_H
#define _VERTEX_FORMAT_H
#ifdef __cplusplus
extern "C" {
#endif

//Get OpenGL header
#if defined(_BUILD_WIN32) || defined(_BUILD_MAC) || defined(_BUILD_LINUX)
    //#include <GL\glew.h>
	#include "../Includes/GL/glew.h"
  
#elif defined (_BUILD_ANDROID)
    #include <GLES2/gl2.h>
    
#endif

typedef struct
{
    //Modelspace pos
    GLfloat x;
    GLfloat y;
    GLfloat z;

    //Texture coordinate
    GLfloat u;
    GLfloat v;

    //Normal vector
    GLfloat nX;
    GLfloat nY;
    GLfloat nZ;

} VertexFormat;

#ifdef __cplusplus
}
#endif
#endif