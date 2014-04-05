#ifndef LEAKER_PROTOCALMANAGER_H_
#define LEAKER_PROTOCALMANAGER_H_

#include "Protocal.h"
#include "utility/Singleton.h"
#include <string>
#include <map>
#include "ProtocalDissector.h"
#include "plugin/plugin.h"

struct lua_State;
class ProtocalManager: public Singleton<ProtocalManager>
{
public:
	ProtocalManager();
	~ProtocalManager();
	void Register_Ptotocal(const char * prot_name,unsigned int upper_prot,
							unsigned short feature,fp_ProtocalAnalyse pfn);
	void Register_Protocal(const char * prot_name,unsigned int upper_prot,
							unsigned short feature,const char * luafunc);
	void StartAnalyse(PacketInfo* RawPacket);

	void addPtotocalFeature(unsigned short feature,std::string name);
	bool isExisting(unsigned short feature);
	const char * getProtocalName(unsigned short feature);
private:
	lua_State *L;
	typedef struct tagProtocalNode
	{
		std::string name;                 //协议名
		unsigned short featrue;           //协议特征字
		fp_ProtocalAnalyse func;          //协议解析函数
		struct tagProtocalNode * same;    //指向同层协议协议链表(下一个兄弟)
		struct tagProtocalNode * next;    //指向下层协议协议链表(第一个孩子)
		unsigned long key;                //lua函数的唯一标志符(从1开始，当key==0时，说明是C函数解析
	}ProtocalNode,*pProtocalNode;

	ProtocalNode * ProtocalTree_;
	ProtocalNode * Add_New_Node(ProtocalNode *_parent,const char *_name,unsigned short _featrue,\
		fp_ProtocalAnalyse _func);
	ProtocalNode * Add_New_LuaNode(ProtocalNode *_parent,const char * name,unsigned short _feature,\
		const char * _luafun);
	ProtocalNode * search(unsigned short feature);
	ProtocalNode * Get_by_feature(unsigned short _feature,\
		 ProtocalNode *_Head);

	//管理协议特征字
	//特征字－协议名
	std::map<unsigned short,std::string> protocalFeature_;

	//管理LUA插件的协议解析函数名
	//为了避免函数名重复，转换为用唯一的key来标识
	std::map<unsigned long,std::string> luaFuncName_;

	unsigned long key_;  //唯一标识从1开始，依次增加

	//根据key调用lua函数
	void doluaFunc(unsigned long key,PacketInfo* RawPacket);
};

#endif