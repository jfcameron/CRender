#ifndef _TEXTURE_H
#define _TEXTURE_H
#ifdef __cplusplus
extern "C" {
#endif

//Handle local includes
#ifdef _BUILD_ANDROID
	#include "../Includes/uthash.h"

#else
	//#include <uthash.h>
	#include "../Includes/uthash.h"

#endif

//Get OpenGL header
#if defined(_BUILD_WIN32) || defined(_BUILD_MAC) || defined(_BUILD_LINUX)
    //#include <GL\glew.h>
	#include "../Includes/GL/glew.h"

#elif defined (_BUILD_ANDROID)
    #include <GLES2/gl2.h>

#endif

struct Texture
{
    
    //Data
    GLint handle;
    
    //Hash data
    char* name;        /* Hash key */
    UT_hash_handle hh; /* makes this structure hashable */

};

#ifdef __cplusplus
}
#endif
#endif
