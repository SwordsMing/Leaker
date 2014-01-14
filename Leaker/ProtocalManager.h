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
		std::string name;                 //Э����
		unsigned short featrue;           //Э��������
		fp_ProtocalAnalyse func;          //Э���������
		struct tagProtocalNode * same;    //ָ��ͬ��Э��Э������(��һ���ֵ�)
		struct tagProtocalNode * next;    //ָ���²�Э��Э������(��һ������)
	}ProtocalNode;

	ProtocalNode * m_ProtocalTree;
	void Add_New_Node(ProtocalNode **_head,const char *_name,unsigned short _featrue,\
		fp_ProtocalAnalyse _func);
	ProtocalNode * Get_by_feature(unsigned short _feature,\
		 ProtocalNode *_Head);
};

#endif