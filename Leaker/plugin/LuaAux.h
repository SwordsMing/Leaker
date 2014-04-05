#ifndef LEAKER_LUAAUX_H_
#define LEAKER_LUAAUX_H_

/*

    此文件声明注册到LUA环境中的C函数原型，这些函数用于辅助LUA插件的协议解析
	created by zc 2014/4/4 
*/

struct lua_State;

//更改协议列信息中的源地址字段 ColumnInfo.source_
static int c_setSource(lua_State * );

//更改协议列信息中的目的地址字段 ColumnInfo.destination_
static int c_setDestination(lua_State *);

//更改协议列信息中协议字段 ColumnInfo.protocal_
static int c_setProtocal(lua_State *);

//更改协议列信息中的操作码字段 ColumnInfo.opcode_
static int c_setOpcode(lua_State *);

//更改协议列信息中的详细信息字段 ColumnInfo.info_
static int c_setInfo(lua_State *);

//返回包含协议数据的字符串
static int c_getBuffer(lua_State *);

//向详细协议信息树添加一棵子树
//向LUA返回子树的根结点
static int c_addTree(lua_State *);

//向树中添加结点信息
//向LUA返回所添加结点指针
static int c_insert(lua_State *);

//向协议管理器注册新协议信息
static int c_registerProtocal(lua_State *);

//向LUA注册所有的C函数
void registerAllCFun(lua_State * );
#endif