package com.apex.xplayer;

public class FFPlayer {

	private int native_player= 0;
	
	static {
		System.loadLibrary("ffmpeg");
		System.loadLibrary("SDL2");
        System.loadLibrary("ffplayer");
    }
 
	private  native int ffplayer_create();
    private  native int ffplayer_init(int handle);
    private  native int ffplayer_open(int handle,String url);
    private  native int ffplayer_resume(int handle);
    private  native int ffplayer_pause(int handle);
    private  native int ffplayer_stop(int handle);
    private  native int ffplayer_destroy(int handle);
    
    
    public FFPlayer()
    {
    	native_player = ffplayer_create();
    }
    
    public void close()
    {
    	int ret = ffplayer_destroy(native_player);
    	native_player = 0;
    }
    protected void finalize()
    {
    	close();
    }
    
    public int init()
    {
    	return ffplayer_init(native_player);
    }
    
    public int open(String url)
    {
    	return ffplayer_open(native_player,url);
    }
    
    public int pause(){
    	return ffplayer_pause(native_player);
    }
   
    
    public int resume(){
    	return ffplayer_resume(native_player);
    }
    
    public int stop(){
    	return ffplayer_stop(native_player);
    }
}
