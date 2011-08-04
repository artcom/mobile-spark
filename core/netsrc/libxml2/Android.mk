LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libxml2
LOCAL_SRC_FILES := libxml2.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/includes
include $(PREBUILT_STATIC_LIBRARY)
