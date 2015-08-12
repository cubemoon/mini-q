#!/usr/bin/env bash

function run_and_check_cmd()
{
    cmd=$1

    echo ""
    echo "--------------------------------------------"
    echo "$cmd"
    echo "--------------------------------------------"
    echo ""

    eval "$cmd"

    if [[ $? != 0 ]]; then
        echo "running:"
        echo "$cmd"
        exit $?
    fi
}

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
APP_ROOT="$DIR/.."
APP_ANDROID_ROOT="$DIR"
COCOS2DX_ROOT=$QMM_ROOT/lib/cocos2d-x

echo "- config:"
echo "  ANDROID_NDK_ROOT    = $ANDROID_NDK_ROOT"
echo "  QMM_ROOT = $QMM_ROOT"
echo "  COCOS2DX_ROOT       = $COCOS2DX_ROOT"
echo "  APP_ROOT            = $APP_ROOT"
echo "  APP_ANDROID_ROOT    = $APP_ANDROID_ROOT"

# if dont use DEBUG, comments out codes below
NDK_DEBUG=1

echo "- cleanup"
find "$APP_ANDROID_ROOT" -type d | xargs chmod 755 $1
if [ -d "$APP_ANDROID_ROOT"/bin ]; then
    rm -rf "$APP_ANDROID_ROOT"/bin/*.apk
fi
mkdir -p "$APP_ANDROID_ROOT"/bin
chmod 755 "$APP_ANDROID_ROOT"/bin

if [ -d "$APP_ANDROID_ROOT"/assets ]; then
    rm -rf "$APP_ANDROID_ROOT"/assets/*
fi
mkdir -p "$APP_ANDROID_ROOT"/assets
chmod 755 "$APP_ANDROID_ROOT"/assets

echo "- copy scripts"
cp -rf "$APP_ROOT"/scripts "$APP_ANDROID_ROOT"/assets/
echo "- copy resources"
cp -rf "$APP_ROOT"/res "$APP_ANDROID_ROOT"/assets/

# build
echo "Using prebuilt externals"
"$ANDROID_NDK_ROOT"/ndk-build -j`sysctl -n hw.logicalcpu_max` $ANDROID_NDK_BUILD_FLAGS \
NDK_DEBUG=$NDK_DEBUG $NDK_BUILD_FLAGS -C "$APP_ANDROID_ROOT" $* \
"NDK_MODULE_PATH=${QMM_ROOT}:${COCOS2DX_ROOT}:${COCOS2DX_ROOT}/cocos2dx/platform/third_party/android/prebuilt"


run_and_check_cmd "android update project -t android-10 -p ../lib/cocos2d-x/cocos2dx/platform/android/java"
run_and_check_cmd "android update project -t android-10 -p `pwd`"
run_and_check_cmd "ant clean debug -f `pwd`/build.xml"
#run_and_check_cmd "adb uninstall \"__PROJECT_PACKAGE_FULL_NAME_L__\""
# adb install /path/to/apk
# adb shell am start -n "__PROJECT_PACKAGE_FULL_NAME_L__/.__PROJECT_PACKAGE_LAST_NAME_UF__"
