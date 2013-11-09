package com.apex.xplayer;

public class FFPlayerWrapper {

	static {
		System.loadLibrary("ffmpeg");
		System.loadLibrary("SDL2");
        System.loadLibrary("ffplayer");
    }
 
    public static native void on_surface_created();
 
    public static native void on_surface_changed(int width, int height);
 
    public static native void on_draw_frame();

}
