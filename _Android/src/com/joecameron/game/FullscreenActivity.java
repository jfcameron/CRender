package com.joecameron.game;

import com.joecameron.game.util.SystemUiHider;

import android.annotation.TargetApi;
import android.app.Activity;
import android.app.ActivityManager;
import android.content.pm.ConfigurationInfo;
import android.content.Context;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.TextView;
import android.opengl.GLSurfaceView;

public class FullscreenActivity extends Activity 
{
    //*************
    // Data members
    //*************
    private GLSurfaceView glSurfaceView;
    private boolean rendererSet;
    
    //***************************
    // Android Activity interface
    //***************************
	@Override
    protected void onCreate(Bundle savedInstanceState) 
    {
        super.onCreate(savedInstanceState);
        
        //Check for GLES 2.0 support
        ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
        final boolean supportsEs2 = configurationInfo.reqGlEsVersion >= 0x20000 || isProbablyEmulator();
        
        
        
        if (supportsEs2) 
        {
			glSurfaceView = new GLSurfaceView(this);

			if (isProbablyEmulator()) 
            {
				// Avoids crashes on startup with some emulator images.
				glSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
			
            }

			glSurfaceView.setEGLContextClientVersion(2);
			glSurfaceView.setRenderer(new RendererWrapper());
			rendererSet = true;
			setContentView(glSurfaceView);
			getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
            
		} 
        
        else 
        {
			// Should never be seen in production, since the manifest filters
			// unsupported devices.
			//Toast.makeText(this, "This device does not support OpenGL ES 2.0.", Toast.LENGTH_LONG).show();
			return;
            
		}
        
        
        /*    
        TextView tv = new TextView(this);
        tv.setText("Hello from Java!");
        tv.setText(stringFromJNI());
        tv.setY(200);
        
        setContentView(tv);
        */
	
    }
    
    //A thing that might not be needed
    private boolean isProbablyEmulator() 
    {
		return Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH_MR1
				&& (Build.FINGERPRINT.startsWith("generic")
						|| Build.FINGERPRINT.startsWith("unknown")
						|| Build.MODEL.contains("google_sdk")
						|| Build.MODEL.contains("Emulator")
						|| Build.MODEL.contains("Android SDK built for x86"));
                        
	}
    
}
