#ifndef _GLOBALS_H
#define _GLOBALS_H
#ifdef __cplusplus
extern "C" {
#endif

    //PRECOMPILER CONSTANTS
    #define _WINDOW_NAME "Game Engine - C, OpenGL ES 2.0"
    #define _WIN32_NAME "Win32"
    #define _BUILDNO __DATE__
    //#define _DEBUG_BUILD _DEBUG

    //TYPEDEFS
    //#ifndef uint
        typedef unsigned int uint;
    
    //#endif

    //RENDERER CONSTANTS
	#if defined(_BUILD_WIN32) || defined(_BUILD_MAC) || defined(_BUILD_LINUX)
    	#define _SHADER_DIR "Shaders/"
		#define _TEXTURE_DIR ""

	#elif defined (_BUILD_ANDROID)
        #define _SHADER_DIR "/sdcard/Shaders/"
		#define _TEXTURE_DIR "/sdcard/"

	#endif

    //ENGINE COMPONENT POINTERS
    #include "../Graphics/Graphics.h"
    struct Graphics* GRAPHICS;

    #include "../Time/Time.h"
    Time* TIME;

#ifdef __cplusplus
}
#endif

#endif
