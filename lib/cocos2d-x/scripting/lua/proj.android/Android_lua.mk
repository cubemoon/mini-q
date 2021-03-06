
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := cocos_lua_static
LOCAL_MODULE_FILENAME := liblua

LOCAL_SRC_FILES := ../cocos2dx_support/CCLuaEngine.cpp \
    ../cocos2dx_support/CCLuaStack.cpp \
    ../cocos2dx_support/CCLuaValue.cpp \
    ../cocos2dx_support/Cocos2dxLuaLoader.cpp \
    ../cocos2dx_support/tolua_fix.c \
    ../cocos2dx_support/snapshot.c \
    ../cocos2dx_support/platform/android/CCLuaJavaBridge.cpp \
    ../cocos2dx_support/platform/android/org_cocos2dx_lib_Cocos2dxLuaJavaBridge.cpp \
    ../tolua/tolua_event.c \
    ../tolua/tolua_is.c \
    ../tolua/tolua_map.c \
    ../tolua/tolua_push.c \
    ../tolua/tolua_to.c \
    ../cocos2dx_support/LuaCocos2d.cpp \
    ../cocos2dx_support/Lua_web_socket.cpp \
    ../lua_extensions/cjson/fpconv.c \
    ../lua_extensions/cjson/lua_cjson.c \
    ../lua_extensions/cjson/strbuf.c \
    ../lua_extensions/zlib/lua_zlib.c \
    ../lua_extensions/socket/auxiliar.c \
    ../lua_extensions/socket/luasocket_buffer.c \
    ../lua_extensions/socket/except.c \
    ../lua_extensions/socket/inet.c \
    ../lua_extensions/socket/luasocket_io.c \
    ../lua_extensions/socket/luasocket.c \
    ../lua_extensions/socket/mime.c \
    ../lua_extensions/socket/options.c \
    ../lua_extensions/socket/select.c \
    ../lua_extensions/socket/serial.c \
    ../lua_extensions/socket/socket_scripts.c \
    ../lua_extensions/socket/tcp.c \
    ../lua_extensions/socket/timeout.c \
    ../lua_extensions/socket/udp.c \
    ../lua_extensions/socket/unix.c \
    ../lua_extensions/socket/usocket.c \
    ../lua_extensions/filesystem/lfs.c \
    ../lua_extensions/lpack/lpack.c \
    ../cocos2dx_support/lua_cocos2dx_manual.cpp \
    ../lua_extensions/lua_extensions.c \
    ../lua/lua-5.1.5/src/lapi.c \
    ../lua/lua-5.1.5/src/lauxlib.c \
    ../lua/lua-5.1.5/src/lbaselib.c \
    ../lua/lua-5.1.5/src/lcode.c \
    ../lua/lua-5.1.5/src/ldblib.c \
    ../lua/lua-5.1.5/src/ldebug.c \
    ../lua/lua-5.1.5/src/ldo.c \
    ../lua/lua-5.1.5/src/ldump.c \
    ../lua/lua-5.1.5/src/lfunc.c \
    ../lua/lua-5.1.5/src/lgc.c \
    ../lua/lua-5.1.5/src/linit.c \
    ../lua/lua-5.1.5/src/liolib.c \
    ../lua/lua-5.1.5/src/llex.c \
    ../lua/lua-5.1.5/src/lmathlib.c \
    ../lua/lua-5.1.5/src/lmem.c \
    ../lua/lua-5.1.5/src/loadlib.c \
    ../lua/lua-5.1.5/src/lobject.c \
    ../lua/lua-5.1.5/src/lopcodes.c \
    ../lua/lua-5.1.5/src/loslib.c \
    ../lua/lua-5.1.5/src/lparser.c \
    ../lua/lua-5.1.5/src/lstate.c \
    ../lua/lua-5.1.5/src/lstring.c \
    ../lua/lua-5.1.5/src/lstrlib.c \
    ../lua/lua-5.1.5/src/ltable.c \
    ../lua/lua-5.1.5/src/ltablib.c \
    ../lua/lua-5.1.5/src/ltm.c \
    ../lua/lua-5.1.5/src/lundump.c \
    ../lua/lua-5.1.5/src/lvm.c \
    ../lua/lua-5.1.5/src/lzio.c \
    ../lua/lua-5.1.5/src/print.c

ifeq ($(CC_CCSTUDIO_ENABLED),1)
LOCAL_SRC_FILES += \
    ../cocos2dx_support/lua_cocos2dx_extensions_manual.cpp
endif

ifeq ($(CC_SQLITE_ENABLED),1)
LOCAL_SRC_FILES += \
    ../lua_extensions/lsqlite3/sqlite3.c \
    ../lua_extensions/lsqlite3/lsqlite3.c
endif

ifeq ($(CC_FILTERS_ENABLED),1)
LOCAL_SRC_FILES += \
    ../cocos2dx_support/LuaCocos2dFilters.cpp
endif

ifeq ($(CC_DRAGONBONES_ENABLED),1)
LOCAL_SRC_FILES += \
    ../cocos2dx_support/LuaCocos2dDragonBones.cpp
endif

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../lua/lua-5.1.5/src \
                           $(LOCAL_PATH)/../tolua \
                           $(LOCAL_PATH)/../cocos2dx_support \
                           $(LOCAL_PATH)/../cocos2dx_support/platform/android \
                           $(LOCAL_PATH)/../lua_extensions \
                           $(LOCAL_PATH)/../cocos2d-x/scripting/lua/lua_extensions/cjson \
                           $(LOCAL_PATH)/../cocos2d-x/scripting/lua/lua_extensions/zlib \
                           $(LOCAL_PATH)/../cocos2d-x/scripting/lua/lua_extensions/socket \
                           $(LOCAL_PATH)/../cocos2d-x/scripting/lua/lua_extensions/filesystem \
                           $(LOCAL_PATH)/../cocos2d-x/scripting/lua/lua_extensions/lpack \
                           $(LOCAL_PATH)/../cocos2d-x/scripting/lua/lua_extensions/lsqlite3

LOCAL_C_INCLUDES := $(LOCAL_PATH)/ \
                    $(LOCAL_PATH)/../lua/lua-5.1.5/src \
                    $(LOCAL_PATH)/../tolua \
                    $(LOCAL_PATH)/../cocos2dx_support \
                    $(LOCAL_PATH)/../cocos2dx_support/platform/android \
                    $(LOCAL_PATH)/../lua_extensions \
                    $(LOCAL_PATH)/../cocos2d-x/scripting/lua/lua_extensions/cjson \
                    $(LOCAL_PATH)/../cocos2d-x/scripting/lua/lua_extensions/zlib \
                    $(LOCAL_PATH)/../cocos2d-x/scripting/lua/lua_extensions/socket \
                    $(LOCAL_PATH)/../cocos2d-x/scripting/lua/lua_extensions/filesystem \
                    $(LOCAL_PATH)/../cocos2d-x/scripting/lua/lua_extensions/lpack \
                    $(LOCAL_PATH)/../cocos2d-x/scripting/lua/lua_extensions/lsqlite3 \
                    $(LOCAL_PATH)/../../../cocos2dx \
                    $(LOCAL_PATH)/../../../cocos2dx/include \
                    $(LOCAL_PATH)/../../../cocos2dx/platform \
                    $(LOCAL_PATH)/../../../cocos2dx/platform/android \
                    $(LOCAL_PATH)/../../../cocos2dx/kazmath/include \
                    $(LOCAL_PATH)/../../../CocosDenshion/include \
                    $(LOCAL_PATH)/../../../extensions \
                    $(LOCAL_PATH)/../../../external

LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_external_static

ifeq ($(CC_CURL_ENABLED),1)
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_curl_static
endif

LOCAL_CFLAGS += -Wno-psabi -DCC_LUA_ENGINE_ENABLED=1 $(ANDROID_COCOS2D_BUILD_FLAGS)

include $(BUILD_STATIC_LIBRARY)

$(call import-module,extensions)
$(call import-module,external)
