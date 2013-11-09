
#include "ffplayer.h"

extern "C" {

#ifdef __cplusplus
#define __STDC_CONSTANT_MACROS
#ifdef _STDINT_H
#undef _STDINT_H
#endif
//#include
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/version.h"
#include "libswscale/swscale.h"
}

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_Thread.h"

#include "log.h"
#include <cassert>

FFPlayer::FFPlayer() {

	LOGD("player created");
}

FFPlayer::~FFPlayer() {
	LOGD("player destroyed");
}

static SDL_Window *screen = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

int FFPlayer::init() {
	av_register_all();
	LOGD("player init");

	SDL_SetMainReady();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
		fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
		LOGD("Could not initialize SDL - %s\n", SDL_GetError());
		exit(1);
	}

	return 0;
}

void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame) {
	FILE *pFile;
	char szFilename[32];
	int y;

	// Open file
	sprintf(szFilename, "/sdcard/frame%d.ppm", iFrame);
	pFile = fopen(szFilename, "wb");
	if (pFile == NULL) {
		LOGD("can not open file %s to write");
		return;
	}

	// Write header
	fprintf(pFile, "P6\n%d %d\n255\n", width, height);

	// Write pixel data
	for (y = 0; y < height; y++)
		fwrite(pFrame->data[0] + y * pFrame->linesize[0], 1, width * 3, pFile);

	// Close file
	fclose(pFile);
}

int FFPlayer::open(const char *url) {
	LOGD("player open %s", url);
	AVFormatContext *pFormatContext = NULL;

	if (avformat_open_input(&pFormatContext, url, NULL, NULL) != 0) {
		LOGE("can not open file %s", url);
		return -1;
	}

	if (avformat_find_stream_info(pFormatContext, NULL) < 0) {
		LOGE("could not find stream info");
		return -1;
	}

	av_dump_format(pFormatContext, 0, url, 0);

	int i;
	AVCodecContext *pCodecCtx;
	// Find the first video stream
	int videoStream = -1;
	for (i = 0; i < pFormatContext->nb_streams; i++)
		if (pFormatContext->streams[i]->codec->codec_type
				== AVMEDIA_TYPE_VIDEO) {
			videoStream = i;
			break;
		}
	if (videoStream == -1) {
		LOGE("no video stream find");
		return -1; // Didn't find a video stream
	}
	// Get a pointer to the codec context for the video stream
	pCodecCtx = pFormatContext->streams[videoStream]->codec;

	LOGD("ready for decode");
	AVCodec *pCodec = NULL;
	// Find the decoder for the video stream
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if (pCodec == NULL) {
		fprintf(stderr, "Unsupported codec!\n");
		LOGE("Unsupported codec!\n");
		return -1; // Codec not found
			}
		// Open codec
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
		LOGE("error open codec");
		return -1; // Could not open codec
	}

	//
	//screen = SDL_SetVideoMode(pCodecCtx->width, pCodecCtx->height, 0, 0);
	screen = SDL_CreateWindow("My Game Window", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, pCodecCtx->width, pCodecCtx->height,
			SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
	if (!screen) {
		fprintf(stderr, "SDL: could not set video mode - exiting\n");
		exit(1);
	}

	renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_TARGETTEXTURE);
	if (!renderer) {
		fprintf(stderr, "Couldn't setcreate renderer: %s\n", SDL_GetError());
		exit(1);
	}

		//SDL_Overlay     *bmp;
		//bmp = SDL_CreateYUVOverlay(pCodecCtx->width, pCodecCtx->height,SDL_YV12_OVERLAY, screen);

	SDL_Texture *bmp;
	bmp = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV,
			SDL_TEXTUREACCESS_STATIC, pCodecCtx->width, pCodecCtx->height);

	//alloccate video frame
	AVFrame *pFrame = pFrame = avcodec_alloc_frame();
	AVFrame *pFrameRGB = avcodec_alloc_frame();
	if (pFrameRGB == NULL) {
		LOGE("error alloc frame");
		return -1;
	}

	uint8_t *buffer;
	int numBytes;
	// Determine required buffer size and allocate buffer
	numBytes = avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,
			pCodecCtx->height);
	buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
	avpicture_fill((AVPicture *) pFrameRGB, buffer, PIX_FMT_RGB24,
			pCodecCtx->width, pCodecCtx->height);

	int frameFinished;
	AVPacket packet;
	i = 0;
	while (av_read_frame(pFormatContext, &packet) >= 0) {
		// Is this a packet from the video stream?
		if (packet.stream_index == videoStream) {
			// Decode video frame
			avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

			// Did we get a video frame?
			if (frameFinished) {

				LOGD("we got a frame");
				// Convert the image from its native format to RGB
				/*img_convert((AVPicture *)pFrameRGB, PIX_FMT_RGB24,
				 (AVPicture*)pFrame, pCodecCtx->pix_fmt,
				 pCodecCtx->width, pCodecCtx->height);*/

				/*struct SwsContext *img_convert_ctx;
				 img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,  pCodecCtx->width, pCodecCtx->height, PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
				 if(img_convert_ctx == NULL)
				 {
				 fprintf(stderr, "Cannot initialize the conversion context!\n");
				 exit(1);
				 }

				 sws_scale(img_convert_ctx,
				 pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
				 pFrameRGB->data, pFrameRGB->linesize);

				 // Save the frame to disk
				 if(++i<=5)
				 SaveFrame(pFrameRGB, pCodecCtx->width,
				 pCodecCtx->height, i);*/

				//convert to YUV420
				AVPicture pict;
				struct SwsContext *img_convert_ctx;
				img_convert_ctx = sws_getContext(pCodecCtx->width,
						pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width,
						pCodecCtx->height, PIX_FMT_YUV420P, SWS_BILINEAR, NULL,
						NULL, NULL);
				if (img_convert_ctx == NULL) {
					fprintf(stderr, "Cannot initialize the conversion context!\n");
					exit(1);
				}

				sws_scale(img_convert_ctx, pFrame->data, pFrame->linesize, 0,
						pCodecCtx->height, pict.data, pict.linesize);

				SDL_RenderClear(renderer);
				int i = SDL_UpdateTexture(bmp, NULL, pict.data[0],
						pict.linesize[0]);
				LOGE("after SDL_UpdateTexture");
				//SDL_RenderClear( renderer );
				//LOGE("after SDL_RenderClear");
				SDL_RenderCopy(renderer, bmp, NULL, NULL);
				LOGE("after SDL_RenderCopy");
				SDL_RenderPresent(renderer);
				LOGE("after SDL_RenderPresent");

			}
		}

		// Free the packet that was allocated by av_read_frame
		av_free_packet(&packet);
	}

	LOGD("finished");

	avcodec_free_frame(&pFrame);
	avcodec_free_frame(&pFrameRGB);
	av_free(buffer);

	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatContext);

	return 0;
}

int FFPlayer::pause() {
	LOGD("ffplayer pause");
	return 0;
}

int FFPlayer::resume() {
	LOGD("ffplayer resume");
	return 0;
}

int FFPlayer::stop() {
	LOGD("ffplayer stop");
	return 0;
}
