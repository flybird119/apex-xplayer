LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)


LOCAL_MODULE := main

SDL_PATH := $(LOCAL_PATH)/../SDL

#LOCAL_C_INCLUDES := $(LOCAL_PATH)/../ffmpeg/android/arm/include 
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../ffmpeg/
LOCAL_C_INCLUDES += $(SDL_PATH)/include

#LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := ../SDL/src/main/android/SDL_android_main.cpp  ffplay.c cmdutils.c

#ffplayer_jni.cpp ffplayer.cpp

LOCAL_CFLAGS += -DGL_GLEXT_PROTOTYPES
LOCAL_LDLIBS := -ldl -lGLESv1_CM -lGLESv2 -llog  

LOCAL_SHARED_LIBRARIES := SDL ffmpeg

include $(BUILD_SHARED_LIBRARY)

