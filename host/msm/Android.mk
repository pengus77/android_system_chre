#
# Copyright 2017 The Android Open Source Project
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

ifneq ($(BUILD_WITHOUT_VENDOR),true)
ifeq ($(call is-vendor-board-platform,QCOM),true)
ifeq ($(call is-board-platform-in-list,sdm845 sdm710), true)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := chre
LOCAL_MODULE_TAGS := optional
LOCAL_VENDOR_MODULE := true

LOCAL_CPP_EXTENSION := .cc
LOCAL_CFLAGS += -Wall -Werror -Wextra

LOCAL_SRC_FILES := \
    ../../platform/shared/host_protocol_common.cc \
    ../common/host_protocol_host.cc \
    ../common/socket_server.cc \
    daemon/chre_daemon.cc \
    daemon/generated/chre_slpi_stub.c

LOCAL_C_INCLUDES := \
    system/chre/util/include \
    system/chre/host/common/include \
    system/chre/platform/slpi/include \
    system/chre/platform/shared/include \
    system/chre/platform/slpi/include/chre/platform/slpi \
    system/chre/hexagon/include \
    system/chre/hexagon/include/stddef \
    system/chre/hexagon/remote \
    system/chre/platform/slpi/include \

LOCAL_HEADER_LIBRARIES := chre_flatbuffers

LOCAL_SHARED_LIBRARIES := \
    libbase \
    libutils \
    libcutils \
    liblog \

ifeq ($(call is-board-platform-in-list, sdm710), true)
LOCAL_CFLAGS += -DREMOTE_HANDLE_SPD
LOCAL_SHARED_LIBRARIES += libadsprpc
else
LOCAL_SHARED_LIBRARIES += libsdsprpc
endif

include $(BUILD_EXECUTABLE)

endif
endif
endif
