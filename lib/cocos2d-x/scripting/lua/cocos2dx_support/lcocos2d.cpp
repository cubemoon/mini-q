//
//  lcocos2d.cpp
//  quickcocos2dx
//

#include "lua.hpp"
#include "base_nodes/CCNode.h"

USING_NS_CC;
/**
 * CCNode
 */

#define CCNodeSelf(L) (CCNode*) luaL_checkudata(L, 1, "CCNode")

static int
lCCNode_gc(lua_State *L) {
    CCNode *node = CCNodeSelf(L);
    node->release();
    return 0;
}

static int lCCNode_setPosition(lua_State *L)
{
    CCNode *node = CCNodeSelf(L);
    float x = luaL_checknumber(L, 2);
    float y = luaL_checknumber(L, 3);
    node->setPosition(x, y);

    return 0;
}

static int lCCNode_getPosition(lua_State *L)
{
    CCNode *node = CCNodeSelf(L);
    lua_pushnumber(L, node->getPositionX());
    lua_pushnumber(L, node->getPositionY());
    return 2;
}

static int lCCNode_retain(lua_State *L)
{
    CCNode *node = CCNodeSelf(L);
    node->retain();
    return 0;
}

static int lCCNode_create(lua_State *L)
{
    /* 相关函数解释
     * http://cloudwu.github.io/lua53doc/manual.html#luaL_newmetatable
     * http://cloudwu.github.io/lua53doc/manual.html#lua_setfield
     * http://cloudwu.github.io/lua53doc/manual.html#lua_setmetatable
     */

    CCNode *node = CCNode::create();            /* object's lifetime is not managed by Lua, so without gc */
    lua_pushlightuserdata(L, node);             /* -1 */

    node->init();

    if (luaL_newmetatable(L, "CCNode")) {       /* -2 */
        luaL_Reg l[] = {
            { "setPosition", lCCNode_setPosition },
            { "getPosition", lCCNode_getPosition },
            { "retain", lCCNode_retain },
            {NULL,NULL},
        };
        luaL_newlib(L, l);
        lua_setfield(L, -2, "__index");
        lua_pushcfunction(L, lCCNode_gc);
        lua_setfield(L, -2, "__gc");    /* never call __gc with lightuserdata */
    }
    lua_setmetatable(L, -2);
    return 1;
}

static const struct luaL_Reg lib[] =
{
    {"create", lCCNode_create},
    {NULL, NULL}
};

static int lCCNode_reg(lua_State* L)
{
    luaL_newlib(L, lib);
    return 1;
}

void lcocos2d_reg(lua_State* L)
{
    luaL_requiref(L, "CCNode", lCCNode_reg, 1), lua_pop(L, 1);
}

