package com.joecameron.game;

//
// Wraps C functions through JNI
//
public class JNIWrapper 
{
	//***********************************
    // CPP lib load & function signatures
    //***********************************
    //Loader
    static 
    {
    	System.loadLibrary("game");
           
    }
    
    //Signatures
    static public native String stringFromJNI();
    static public native void programInit();
    static public native void programUpdate();
    
}




