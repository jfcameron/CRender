//
// Hashtable entry contains a reference to a compiled shaderprogram object
// Uses uthash header https://troydhanson.github.io/uthash/
// key is the program's name, determined at shader compilation
//
#ifndef _SHADER_PROGRAM_H
#define _SHADER_PROGRAM_H
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

struct ShaderProgram 
{
    char* name; //Key
    GLint handle;
                 
    UT_hash_handle hh; /* makes this structure hashable */

};

#ifdef __cplusplus
}
#endif

#endif




