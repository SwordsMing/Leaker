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
		std::string name;                 //Э����
		unsigned short featrue;           //Э��������
		fp_ProtocalAnalyse func;          //Э���������
		struct tagProtocalNode * same;    //ָ��ͬ��Э��Э������(��һ���ֵ�)
		struct tagProtocalNode * next;    //ָ���²�Э��Э������(��һ������)
		unsigned long key;                //lua������Ψһ��־��(��1��ʼ����key==0ʱ��˵����C��������
	}ProtocalNode,*pProtocalNode;

	ProtocalNode * ProtocalTree_;
	ProtocalNode * Add_New_Node(ProtocalNode *_parent,const char *_name,unsigned short _featrue,\
		fp_ProtocalAnalyse _func);
	ProtocalNode * Add_New_LuaNode(ProtocalNode *_parent,const char * name,unsigned short _feature,\
		const char * _luafun);
	ProtocalNode * search(unsigned short feature);
	ProtocalNode * Get_by_feature(unsigned short _feature,\
		 ProtocalNode *_Head);

	//����Э��������
	//�����֣�Э����
	std::map<unsigned short,std::string> protocalFeature_;

	//����LUA�����Э�����������
	//Ϊ�˱��⺯�����ظ���ת��Ϊ��Ψһ��key����ʶ
	std::map<unsigned long,std::string> luaFuncName_;

	unsigned long key_;  //Ψһ��ʶ��1��ʼ����������

	//����key����lua����
	void doluaFunc(unsigned long key,PacketInfo* RawPacket);
};

#endif