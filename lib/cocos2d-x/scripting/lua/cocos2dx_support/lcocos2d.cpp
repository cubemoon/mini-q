//
//  lcocos2d.cpp
//  quickcocos2dx
//
//  Copyright (c) 2015 qeeplay.com. All rights reserved.
//

#include "fflua/fflua.h"
#include "base_nodes/CCNode.h"

using namespace ff;
USING_NS_CC;

void lcocos2d_reg(lua_State* L)
{
    fflua_register_t<CCNode, ctor()>(L, "CCNode")
    .def(&CCNode::create, "create")
    .def(&CCNode::setZOrder, "setZOrder")
    .def(&CCNode::getZOrder, "getZOrder");
//    .def(&CCNode::setPosition, "setPosition");
}

