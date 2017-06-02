//
//
//
//
#ifdef __cplusplus
extern "C" {
#endif

#if defined(_BUILD_WIN32) || defined(_BUILD_MAC) || defined(_BUILD_LINUX)
    #define GLEW_STATIC 
    #define GLEW_NO_GLU
    
	#include <GL/glew.h>
	//#include "../Common/Source/Includes/GL/glew.h"

    #define GLFW_INCLUDE_ES2 //this doesnt seem to do anything           
    
	#include <GLFW/glfw3.h>
	//#include "../Common/Source/Includes/GLFW/glfw3.h"

#endif

#ifdef _BUILD_WIN32
    #include <Windows.h>

#endif

#include <stdio.h>
#include "Globals\Globals.h"
#include "Graphics\Graphics.h"
#include "Time\Time.h"

#ifdef __cplusplus
}
#endif

//
// Input handler
//
static void key( GLFWwindow* window, int k, int s, int action, int mods )
{

}

void reshape( GLFWwindow* window, int width, int height )
{
    //TODO: resize code

}

/* program entry */
int main(int argc, char *argv[])
{
    GLFWwindow* window = 0;
    int width, height;
    Graphics* graphics;

    //Init GLFW
    {
        printf("Initializing application...\n");

        if( !glfwInit() )
        {
            fprintf( stderr, "Failed to initialize GLFW\n" );
            exit( EXIT_FAILURE );
        }
        
    }

    //Init window & context
    {
        //Create name
        char  data[1] = ""; //TODO: verify this pattern
        char* test    = data;
        strcat(test,_WINDOW_NAME);
        strcat(test,", ");
        strcat(test,_WIN32_NAME);
        strcat(test,". Build: ");
        strcat(test,_BUILDNO);
        printf(test);printf("\n");
        
        printf("Creating the window...\n");

        
        
        glfwDefaultWindowHints();

        //glfwWindowHint(GLFW_DEPTH_BITS, 16);
        //glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        #ifdef _DEBUG
          window = glfwCreateWindow( 500, 500,test, NULL, NULL );
        
        #else
            //window = glfwCreateWindow(1360, 768,test, glfwGetPrimaryMonitor(), NULL);
            window = glfwCreateWindow( 500, 500,test, NULL, NULL );
        #endif

        if (!window)
        {
            fprintf( stderr, "Failed to open GLFW window\n" );
            glfwTerminate();
            exit( EXIT_FAILURE );

        }

        {
            

        }
        

    }

    //Init engine
    {
        printf("Initializing the engine...\n");
        // Set callback functions
        glfwSetFramebufferSizeCallback(window, reshape);
        glfwSetKeyCallback(window, key);

        glfwMakeContextCurrent(window);
        glfwSwapInterval( 1 );

        glfwGetFramebufferSize(window, &width, &height);
        reshape(window, width, height);

        //Init glew
        {
            GLenum err = glewInit();

            if (GLEW_OK != err)
            {
                /* Problem: glewInit failed, something is seriously wrong. */
                fprintf(stderr, "GLEW init failed: %s\n", glewGetErrorString(err));
              
            }
            
            fprintf(stdout, "Using GLEW %s\n", glewGetString(GLEW_VERSION));

        }

        printf("Application initialization complete. Passing control to engine.\n");
        printf("***************************************************************\n");

    }
   
    //Init opengl context
    {
        TIME     = Time_New();
        GRAPHICS = Graphics_New();
        

        graphics = GRAPHICS;
        graphics->init(graphics);

        

    }

    #ifndef _DEBUG_BUILD
        #ifdef _BUILD_WIN32
            while(glfwGetTime() < 1.25f);
            FreeConsole();

        #endif
    
    #endif

    // Main loop
    while( !glfwWindowShouldClose(window) )
    {
        //game->update();   
        graphics->draw(graphics);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    // Terminate GLFW
    glfwTerminate();

    // Exit program
    exit( EXIT_SUCCESS );

}







