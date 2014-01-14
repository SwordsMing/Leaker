#ifndef LEAKER_PROTOCALMANAGER_H_
#define LEAKER_PROTOCALMANAGER_H_

#include "Protocal.h"
#include "utility/Singleton.h"
#include <string>
#include "ProtocalDissector.h"
#include "plugin/plugin.h"

class ProtocalManager: public Singleton<ProtocalManager>
{
public:
	ProtocalManager();
	~ProtocalManager();
	void Register_Ptotocal(const char * prot_name,unsigned int upper_prot,
							unsigned short feature,fp_ProtocalAnalyse pfn);
	void StartAnalyse(PacketInfo* RawPacket);
private:
	typedef struct tagProtocalNode
	{
		std::string name;                 //协议名
		unsigned short featrue;           //协议特征字
		fp_ProtocalAnalyse func;          //协议解析函数
		struct tagProtocalNode * same;    //指向同层协议协议链表(下一个兄弟)
		struct tagProtocalNode * next;    //指向下层协议协议链表(第一个孩子)
	}ProtocalNode;

	ProtocalNode * m_ProtocalTree;
	void Add_New_Node(ProtocalNode **_head,const char *_name,unsigned short _featrue,\
		fp_ProtocalAnalyse _func);
	ProtocalNode * Get_by_feature(unsigned short _feature,\
		 ProtocalNode *_Head);
};

#endif