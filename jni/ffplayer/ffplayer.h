#ifndef __FFPLAYER_H__
#define __FFPLAYER_H__

class FFPlayer {
private:

public:
	FFPlayer();
	~FFPlayer();

//controls
	int init();
	int open(const char *url);
	int pause();
	int resume();
	int stop();

//options

//helpers

};

#endif
