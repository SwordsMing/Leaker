#ifndef LEAKER_LUAAUX_H_
#define LEAKER_LUAAUX_H_

/*

    ���ļ�����ע�ᵽLUA�����е�C����ԭ�ͣ���Щ�������ڸ���LUA�����Э�����
	created by zc 2014/4/4 
*/

struct lua_State;

//����Э������Ϣ�е�Դ��ַ�ֶ� ColumnInfo.source_
static int c_setSource(lua_State * );

//����Э������Ϣ�е�Ŀ�ĵ�ַ�ֶ� ColumnInfo.destination_
static int c_setDestination(lua_State *);

//����Э������Ϣ��Э���ֶ� ColumnInfo.protocal_
static int c_setProtocal(lua_State *);

//����Э������Ϣ�еĲ������ֶ� ColumnInfo.opcode_
static int c_setOpcode(lua_State *);

//����Э������Ϣ�е���ϸ��Ϣ�ֶ� ColumnInfo.info_
static int c_setInfo(lua_State *);

//���ذ���Э�����ݵ��ַ���
static int c_getBuffer(lua_State *);

//����ϸЭ����Ϣ�����һ������
//��LUA���������ĸ����
static int c_addTree(lua_State *);

//��������ӽ����Ϣ
//��LUA��������ӽ��ָ��
static int c_insert(lua_State *);

//��Э�������ע����Э����Ϣ
static int c_registerProtocal(lua_State *);

//��LUAע�����е�C����
void registerAllCFun(lua_State * );
#endif