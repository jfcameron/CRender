#include <string.h>
#include <jni.h>
#include "Graphics/Graphics.h"
#include "Globals/Globals.h"

extern "C"
{
    //
    // Engine components
    //
    Graphics* graphics = 0; //TODO fix

    
    //
    // Test func
    //
    JNIEXPORT jstring JNICALL
    Java_com_joecameron_game_JNIWrapper_stringFromJNI
    (JNIEnv *env, jobject obj)
    {
          return env->NewStringUTF("Hello from C++ over JNI!");
    
    }
    
    //
    // Init
    //
    JNIEXPORT void JNICALL
    Java_com_joecameron_game_JNIWrapper_programInit
    (JNIEnv *env, jobject obj)
    {
        graphics = Graphics_New();
        graphics->init(*graphics);
        
        TIME = Time_New();

    }
    
    //
    // OnUpdate
    //
    JNIEXPORT void JNICALL
    Java_com_joecameron_game_JNIWrapper_programUpdate
    (JNIEnv *env, jobject obj)
    {
        graphics->draw(*graphics);
        
    }

}
