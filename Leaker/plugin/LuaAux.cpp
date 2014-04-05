#include "LuaAux.h"
#include "../Protocal.h"
#include "../ProtocalManager.h"
#include "../RootedTree.hpp"
#include <string>
#include <lua.hpp>

static int c_setSource(lua_State * L){

	PacketInfo * info = (PacketInfo*)lua_touserdata(L,1);
	const char * source = lua_tostring(L,2);
	
	info->columnInfo_ .source_ = std::string(source);

	return 0;
}

static int c_setDestination(lua_State *L){

	PacketInfo * info = (PacketInfo*)lua_touserdata(L,1);
	const char * des = lua_tostring(L,2);
	
	info->columnInfo_.destination_  = std::string(des);

	return 0;
}

static int c_setProtocal(lua_State *L){

	PacketInfo * info = (PacketInfo*)lua_touserdata(L,1);
	const char * protocal = lua_tostring(L,2);
	
	info->columnInfo_.protocal_  = std::string(protocal);

	return 0;
}

static int c_setOpcode(lua_State *L){

	PacketInfo * info = (PacketInfo*)lua_touserdata(L,1);
	const char * opcode = lua_tostring(L,2);
	
	info->columnInfo_.opcode_  = std::string(opcode);

	return 0;
}

static int c_setInfo(lua_State *L){

	PacketInfo * info = (PacketInfo*)lua_touserdata(L,1);
	const char * info_ = lua_tostring(L,2);
	
	info->columnInfo_.info_  = std::string(info_);

	return 0;
}

static int c_getBuffer(lua_State *L){

	PacketInfo * info = (PacketInfo*)lua_touserdata(L,-1);
	const unsigned char * data = info->data_;
	int len = info->leftLength_;

	lua_pushlstring(L,(const char *)data,len);
	return 1;
}

static int c_addTree(lua_State *L){
	
	RootedTree<std::string> *tree_ = new RootedTree<std::string>;
	PacketInfo * info = (PacketInfo*)lua_touserdata(L,-1);
	info->detailInfo_.push_back(tree_);

	lua_pushlightuserdata(L,tree_);
	return 1;
}

static int c_insert(lua_State *L){

	RootedTree<std::string> * tree = (RootedTree<std::string>*)lua_touserdata(L,1);
	HITEM node = (HITEM)lua_touserdata(L,2);
	const char * str = lua_tostring(L,3);

	HITEM node_ = tree->insert(node,std::string(str));

	lua_pushlightuserdata(L,node_);

	return 1;
}
	
static int c_registerProtocal(lua_State *L){

	const char * name = lua_tostring(L,1);   //协议名
	int upper = lua_tointeger(L,2);          //上层协议特征字
	int feature = lua_tointeger(L,3);        //本协议特征字
	const char * func = lua_tostring(L,4);   //解析的LUA函数名

	ProtocalManager::instance().addPtotocalFeature(feature,name);
	ProtocalManager::instance().Register_Protocal(name,upper,feature,func);
	return 0;
}

void registerAllCFun(lua_State * L){
	
	lua_register(L,"setSource",c_setSource);
	lua_register(L,"setDestination",c_setDestination);
	lua_register(L,"setProtocal",c_setProtocal);
	lua_register(L,"setOpcode",c_setOpcode);
	lua_register(L,"setInfo",c_setInfo);
	lua_register(L,"getBuffer",c_getBuffer);
	lua_register(L,"addTree",c_addTree);
	lua_register(L,"insert",c_insert);
	lua_register(L,"registerProtocal",c_registerProtocal);
}