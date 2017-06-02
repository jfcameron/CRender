#include "Time.h"

#if defined(_BUILD_WIN32) || defined(_BUILD_MAC) || defined(_BUILD_LINUX)
    //#include <GLFW\glfw3.h>
	#include "../Includes/GLFW/glfw3.h"

#elif defined(_BUILD_ANDROID)
	#include <sys/time.h>

#endif

#include <stdlib.h>

//*****************
// Static variables
//*****************
static unsigned int instanceCount = 0;

//*****************************
// Static function declarations
//*****************************
static float time(void);
static float deltaTime(void);

//******************************
// External function definitions
//******************************
Time* Time_New(void)
{
    Time* output = 0;

    if (instanceCount == 0)
    {
        //Create instance
        output = (Time*)malloc (sizeof (Time));

        //Initialize instance
        output->time      = time   ;
        output->deltaTime = deltaTime   ;

        instanceCount++;

    }

    return output;

}

void Time_Free(const Time* aTime)
{
    //Deallocate members
    free(aTime->time);
    free(aTime->deltaTime);

    //Deallocate object
    free((void*)aTime);

    instanceCount--;

}

//****************************
// static function definitions
//****************************
static float time(void)
{
    float output = 0;
    {
        #if defined(_BUILD_WIN32) || defined(_BUILD_MAC) || defined(_BUILD_LINUX)
            output = (float)glfwGetTime();

		#elif defined(_BUILD_ANDROID)
        {
            //struct timeval tv;
            //gettimeofday(&tv, NULL);
            //return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
            //output = tv.tv_sec * 10000;
            static float temp = 0;
            output = (temp+=0.075);

        }

        #else
            printf("Time.time is undefined for this platform");
    
        #endif


    }

    return output;

}

static float deltaTime(void)
{
    //printf("DeltaTime is not implemented");

}
