# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

# sandi
include $(CLEAR_VARS)

LOCAL_C_EXTENSION := .c
LOCAL_MODULE    := msandi
LOCAL_SRC_FILES := morse.c component.c
ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_CFLAGS += -ffast-math -mtune=atom -mssse3 -mfpmath=sse
endif

include $(BUILD_SHARED_LIBRARY)

#logger
include $(CLEAR_VARS)

LOCAL_C_EXTENSION := .c
LOCAL_MODULE    := mlogger
LOCAL_SRC_FILES := logger.c component.c
LOCAL_LDLIBS 	:= -llog
ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_CFLAGS += -ffast-math -mtune=atom -mssse3 -mfpmath=sse
endif

include $(BUILD_SHARED_LIBRARY)

#Primitive Data
include $(CLEAR_VARS)

LOCAL_C_EXTENSION := .c
LOCAL_MODULE    := mPDATA
LOCAL_SRC_FILES := com_mylexz_utils_PrimitiveData.c MyLEXZ-Cutils/data_op/PrimitiveData.c component.c
ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_CFLAGS += -ffast-math -mtune=atom -mssse3 -mfpmath=sse
endif

include $(BUILD_SHARED_LIBRARY)

#NDATA
include $(CLEAR_VARS)

LOCAL_C_EXTENSION := .c
LOCAL_MODULE    := mNDATA
LOCAL_LDLIBS 	:= -llog
LOCAL_SRC_FILES := com_mylexz_utils_NodeData.c MyLEXZ-Cutils/data_op/NodeData.c MyLEXZ-Cutils/array/cstring.c component.c
ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_CFLAGS += -ffast-math -mtune=atom -mssse3 -mfpmath=sse
endif

include $(BUILD_SHARED_LIBRARY)
