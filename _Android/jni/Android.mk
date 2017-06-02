LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

#Name of .so
LOCAL_MODULE := game

#Set compiler argument
LOCAL_CFLAGS := -w #suppress warnings

#Android libraries to include
LOCAL_LDLIBS := -lGLESv2
LOCAL_LDLIBS += -llog

#Add to local include directories
#LOCAL_C_INCLUDES := $(LOCAL_PATH)/Includes

#Set C precompiler directives
LOCAL_CFLAGS += -D_BUILD_ANDROID
LOCAL_CPPFLAGS += -D_BUILD_ANDROID

#Source files
LOCAL_SRC_FILES += ../../Common/Source/hello-jni.cpp 
LOCAL_SRC_FILES += ../../Common/Source/Graphics/Graphics.c
LOCAL_SRC_FILES += ../../Common/Source/Helpers/Helpers.c
LOCAL_SRC_FILES += ../../Common/Source/LodePNG/lodepng.c
LOCAL_SRC_FILES += ../../Common/Source/Time/Time.c

include $(BUILD_SHARED_LIBRARY)