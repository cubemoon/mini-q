#!/bin/bash
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
TOLUA="$QUICK_COCOS2D_MINI/bin/mac/tolua++"

#cd "$DIR/"
#${TOLUA} -L "$DIR/basic.lua" -o "$QUICK_COCOS2D_MINI/lib/cocos2d-x/scripting/lua/cocos2dx_support/LuaCocos2dAssetsManager.cpp" ExtensionsAssetsManager.tolua

cd "$DIR/"
${TOLUA} -L "$DIR/basic.lua" -o "$QUICK_COCOS2D_MINI/lib/cocos2d-x/scripting/lua/cocos2dx_support/LuaCocos2dFilters.cpp" ExtensionsFilters.tolua

cd "$DIR/"
${TOLUA} -L "$DIR/basic.lua" -o "$QUICK_COCOS2D_MINI/lib/cocos2d-x/scripting/lua/cocos2dx_support/LuaCocos2dDragonBones.cpp" ExtensionsDragonBones.tolua
