package com.apex.xplayer;



import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Bundle;
import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ConfigurationInfo;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.Toast;

public class MainActivity extends Activity {

	private GLSurfaceView glSurfaceView;
	private boolean rendererSet;

	private boolean isProbablyEmulator() {
	    return Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH_MR1
	            && (Build.FINGERPRINT.startsWith("generic")
	                    || Build.FINGERPRINT.startsWith("unknown")
	                    || Build.MODEL.contains("google_sdk")
	                    || Build.MODEL.contains("Emulator")
	                    || Build.MODEL.contains("Android SDK built for x86"));
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
		ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();

		final boolean supportsEs2 = configurationInfo.reqGlEsVersion >= 0x20000	|| isProbablyEmulator();

		if (supportsEs2) {
			glSurfaceView = new GLSurfaceView(this);

			if (isProbablyEmulator()) {
				// Avoids crashes on startup with some emulator images.
				glSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
			}

			glSurfaceView.setEGLContextClientVersion(2);
			//draw when requestRender();
			//glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
			glSurfaceView.setRenderer(new RendererWrapper());
			rendererSet = true;
			setContentView(glSurfaceView);
			
		
			FFPlayer player = new FFPlayer();
			player.init();
			player.open("/sdcard/movie/small.flv");
			
    		
		} else {
			// Should never be seen in production, since the manifest filters
			// unsupported devices.
			Toast.makeText(this, "This device does not support OpenGL ES 2.0.",
					Toast.LENGTH_LONG).show();
			return;
		}

		/*setContentView(R.layout.activity_main);
		
		Button btn1 = (Button)findViewById(R.id.button1);
        btn1.setOnClickListener(new OnClickListener(){
        	@Override
        	public void onClick(View v)
        	{
        		FFPlayer player = new FFPlayer();
        		player.init();
        		player.open("/sdcard/movie/small.flv");
        	}
        });*/
        
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
	
		getMenuInflater().inflate(R.menu.main, menu);
		
		return true;
	}

	@Override
	protected void onPause() {
	    super.onPause();
	 
	    /*if (rendererSet) {
	        glSurfaceView.onPause();
	    }*/
	}
	 
	@Override
	protected void onResume() {
	    super.onResume();
	 
	    /*if (rendererSet) {
	        glSurfaceView.onResume();
	    }*/
	}
}
