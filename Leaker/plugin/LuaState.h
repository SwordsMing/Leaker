#ifndef LEAKER_LUASTATE_H_
#define LEAKER_LUASTATE_H_

#include "../utility/Singleton.h"
#include <lua.hpp>

class LuaState: public Singleton<LuaState>
{
public:
	lua_State * L;
	LuaState(){
		L = lua_open();
	}
	
	~LuaState(){
		lua_close(L);
	}
};

#endif