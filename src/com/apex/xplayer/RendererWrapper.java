package com.apex.xplayer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;


import android.opengl.GLSurfaceView.Renderer;
import static android.opengl.GLES20.*;

public class RendererWrapper implements Renderer {
	//private FFPlayerWrapper player = new FFPlayerWrapper();
	private float fps = 20;
	private long  startTime = System.currentTimeMillis();
	
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
       // glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    	
    	FFPlayerWrapper.on_surface_created();
    }
 
    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        // No-op
    	FFPlayerWrapper.on_surface_changed(width, height);
    }
 
    @Override
    public void onDrawFrame(GL10 gl) {
        //glClear(GL_COLOR_BUFFER_BIT);
    	
    	/*long endTime = System.currentTimeMillis();
    	long interval = (long) (1000/fps); 
    	long passed = endTime - startTime;
    	if(passed < interval){
    		try {
				Thread.sleep(interval-passed);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
    	}
    	startTime = System.currentTimeMillis();*/
    	
    	//decode and show next frame
    	//FFPlayerWrapper.on_draw_frame();
    
			
    }

	
}