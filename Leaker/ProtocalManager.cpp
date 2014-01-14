#include "ProtocalManager.h"
#include "ProtocalDissector.h"
#include <queue>
using std::queue;

ProtocalManager::ProtocalManager():m_ProtocalTree(NULL){

	//��һ��Э����ΪMAC��
	Add_New_Node(&m_ProtocalTree,_T("MAC"),MAC,g_MacAnalyse);
	//����MAC�Ӳ�Э������
	Add_New_Node(&m_ProtocalTree->next,_T("IP"),IP,g_IpAnalyse);
	Add_New_Node(&m_ProtocalTree->next,_T("ARP"),ARP,g_ArpAnalyse);
	//����IP�Ӳ�Э������
	Add_New_Node(&m_ProtocalTree->next->next,_T("TCP"),TCP,g_TcpAnalyse);
	Add_New_Node(&m_ProtocalTree->next->next,_T("UDP"),UDP,g_UdpAnalyse);

}

ProtocalManager::~ProtocalManager(){
	
	ProtocalNode *temp1,*temp2,*temp3;
	queue<ProtocalNode *> help_queue;       //��������
	help_queue.push(m_ProtocalTree);
	while (!help_queue.empty())
	{
		temp1 = help_queue.front();
		help_queue.pop();
		if(NULL != temp1->next)
		{
			help_queue.push(temp1->next);
		}
		temp2 = temp1->same;
		delete temp1;
		while (temp2)
		{
			if( NULL !=temp2->next)
			{
				help_queue.push(temp2->next);
			}
			temp3 = temp2->same;
			delete temp2;
			temp2 = temp3;
		}
	}
}



void ProtocalManager::Add_New_Node(ProtocalNode **_head,const char *_name,
						unsigned short _feature,fp_ProtocalAnalyse _func)
{
	ProtocalNode *temp = *_head;
	ProtocalNode *_NewNode = new ProtocalNode;
	_NewNode->featrue = _feature;
	_NewNode->func	  = _func;
	_NewNode->name	  = _name;
	_NewNode->same	  = NULL;
	_NewNode->next    = NULL;
	if(NULL == temp)
	{
		*_head = _NewNode;
	}
	else
	{
		while ( NULL != temp->same)
		{
			temp = temp->same;
		}
		temp->same = _NewNode;
	}
}


ProtocalManager::ProtocalNode * ProtocalManager::Get_by_feature(unsigned short _feature,
								ProtocalNode *_Head)
{
	while ( NULL != _Head )
	{
		if(_Head->featrue == _feature)
		{
			break;
		}
		_Head = _Head->same;
	}
	return _Head;
}

//Э����������ⲿ���ԭʼ���ݿ�ʼ����
void ProtocalManager::StartAnalyse(PacketInfo* RawPacket)
{
	ProtocalNode *temp = m_ProtocalTree;
	temp->func(RawPacket);
	while ( NULL != temp->next )
	{
		temp = Get_by_feature(RawPacket->nCurProt,temp->next);
		if(NULL != temp)
		{
			temp->func(RawPacket);
		}
		else
		{
			break;
		}
	}
}

//��Э�������ע��Э����Ϣ
void ProtocalManager::Register_Ptotocal(const char *prot_name, unsigned int upper_prot, 
						unsigned short feature, fp_ProtocalAnalyse pfn)
{
	//���ݸ�Э�������ֱ���Э�����ҵ����Э����
	ProtocalNode *temp;
	if( m_ProtocalTree->featrue == upper_prot )
	{
		temp = m_ProtocalTree;
	}
	else if(m_ProtocalTree->next->featrue == upper_prot)
	{
		temp = m_ProtocalTree->next;
	}
	else
	{
		temp = Get_by_feature(upper_prot,m_ProtocalTree->next->next);
		
	}
	Add_New_Node(&temp->next,prot_name,feature,pfn);
}