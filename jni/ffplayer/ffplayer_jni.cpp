
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <jni.h>
#include <assert.h>

//#include "ffmpeg.h"
//#include "glwrapper.h"

#include <GLES2/gl2.h>
#include "ffplayer.h"

void on_surface_created(JNIEnv *env, jobject clazz) {
    glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
}

void on_surface_changed(JNIEnv *env, jobject clazz,jint width,jint height) {
    // No-op
}

void on_draw_frame(JNIEnv *env, jobject clazz) {
    glClear(GL_COLOR_BUFFER_BIT);
}

jint ffplayer_create(JNIEnv *env, jobject clazz)
{
	FFPlayer *player = new FFPlayer();
	return (jint)player;
}

jint ffplayer_init(JNIEnv *env,jobject clazz,jint handle)
{
	FFPlayer *player = (FFPlayer*)handle;
	return player->init();
}

jint ffplayer_open(JNIEnv *env,jobject clazz,jint handle,jstring jurl)
{
	FFPlayer *player = (FFPlayer*)handle;
	const char *url = env->GetStringUTFChars(jurl,0);
	return player->open(url);
}


jint ffplayer_pause(JNIEnv *env,jobject clazz,jint handle)
{
	FFPlayer *player = (FFPlayer*)handle;
	return player->pause();
}

jint ffplayer_resume(JNIEnv *env,jobject clazz,jint handle)
{
	FFPlayer *player = (FFPlayer*)handle;
	return player->resume();
}

jint ffplayer_stop(JNIEnv *env,jobject clazz,jint handle)
{
	FFPlayer *player = (FFPlayer*)handle;
	return player->stop();
}

jint ffplayer_destroy(JNIEnv *env,jobject clazz,jint handle)
{
	FFPlayer *player = (FFPlayer*)handle;
	delete player;
	return 0;
}


#define JNIREG_CLASS "com/apex/xplayer/FFPlayerWrapper"
static JNINativeMethod gMethods[] = {
	{ "on_surface_created", "()V", (void*)on_surface_created },
	{ "on_surface_changed", "(II)V", (void*)on_surface_changed },
	{ "on_draw_frame", "()V", (void*)on_draw_frame },
};

#define JNI_PLAYER_CLASS "com/apex/xplayer/FFPlayer"
static JNINativeMethod gPlayerMethods[] = {
		{"ffplayer_create","()I",(void*)ffplayer_create},
		{"ffplayer_destroy","(I)I",(void*)ffplayer_destroy},
		{"ffplayer_init","(I)I",(void*)ffplayer_init},
		{"ffplayer_open","(ILjava/lang/String;)I",(void*)ffplayer_open},
		{"ffplayer_resume","(I)I",(void*)ffplayer_resume},
		{"ffplayer_stop","(I)I",(void*)ffplayer_stop},
		{"ffplayer_pause","(I)I",(void*)ffplayer_pause},
};

static int registerNativeMethods(JNIEnv* env, const char* className,
        JNINativeMethod* gMethods, int numMethods)
{
	jclass clazz;
	clazz = env->FindClass(className);
	if (clazz == NULL) {
		return JNI_FALSE;
	}
	if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
		return JNI_FALSE;
	}

	return JNI_TRUE;
}



static int registerNatives(JNIEnv* env)
{
	if (!registerNativeMethods(env, JNIREG_CLASS, gMethods,
                                 sizeof(gMethods) / sizeof(gMethods[0])))
		return JNI_FALSE;

	if(!registerNativeMethods(env,JNI_PLAYER_CLASS, gPlayerMethods, sizeof(gPlayerMethods) / sizeof(gPlayerMethods[0])))
		return JNI_FALSE;

	return JNI_TRUE;
}


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv* env = NULL;
	jint result = -1;

	if (vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
		return -1;
	}
	assert(env != NULL);

	if (!registerNatives(env)) {
		return -1;
	}
	/* success -- return valid version number */
	result = JNI_VERSION_1_4;
	return result;
}



