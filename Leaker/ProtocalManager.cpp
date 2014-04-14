#include "ProtocalManager.h"
#include "ProtocalDissector.h"
#include <queue>
#include <utility>
#include <lua.hpp>
#include <string>
#include "plugin/LuaState.h"
using std::queue;

ProtocalManager::ProtocalManager()
	:ProtocalTree_(NULL),
	key_(1)
{

	//��һ��Э����ΪMAC��
	pProtocalNode mac_parent = Add_New_Node(NULL,_T("MAC"),MAC,g_MacAnalyse);
	//����MAC�Ӳ�Э������
	pProtocalNode ip_parent = Add_New_Node(mac_parent,_T("IP"),IP,g_IpAnalyse);
	Add_New_Node(mac_parent,_T("ARP"),ARP,g_ArpAnalyse);
	//����IP�Ӳ�Э������
	Add_New_Node(ip_parent,_T("TCP"),TCP,g_TcpAnalyse);
	Add_New_Node(ip_parent,_T("UDP"),UDP,g_UdpAnalyse);
}

ProtocalManager::~ProtocalManager(){
	
	ProtocalNode *temp1,*temp2,*temp3;
	queue<ProtocalNode *> help_queue;       //��������
	help_queue.push(ProtocalTree_);
	while (!help_queue.empty()){
		temp1 = help_queue.front();
		help_queue.pop();
		if(NULL != temp1->next){
			help_queue.push(temp1->next);
		}
		temp2 = temp1->same;
		delete temp1;
		while (temp2){
			if( NULL !=temp2->next){
				help_queue.push(temp2->next);
			}
			temp3 = temp2->same;
			delete temp2;
			temp2 = temp3;
		}
	}
}

ProtocalManager::ProtocalNode * ProtocalManager::Add_New_Node(ProtocalNode *_parent,const char *_name,
						unsigned short _feature,fp_ProtocalAnalyse _func)
{
	ProtocalNode *_NewNode = new ProtocalNode;
	_NewNode->featrue = _feature;
	_NewNode->func	  = _func;
	_NewNode->name	  = _name;
	_NewNode->key	  =  0;
	_NewNode->same	  = NULL;
	_NewNode->next    = NULL;
	if(NULL == _parent){
		ProtocalTree_ = _NewNode;
	}else{
		ProtocalNode * first = _parent->next;
		if(first!= NULL){
			while (first->same!= NULL){first = first->same;}
			first->same = _NewNode;
		}
		else{
			_parent->next = _NewNode;
		}
	}
	return _NewNode;
}

ProtocalManager::ProtocalNode * ProtocalManager::Add_New_LuaNode(ProtocalNode *_parent,const char * name,unsigned short _feature,\
		const char * _luafun)
{
	ProtocalNode * node = Add_New_Node(_parent,name,_feature,NULL);
	node->key = key_++;
	luaFuncName_.insert(std::pair<unsigned long ,std::string>(node->key,std::string(_luafun)));

	return node;
}

ProtocalManager::ProtocalNode * ProtocalManager::Get_by_feature(unsigned short _feature,
								ProtocalNode *_Head)
{
	while ( NULL != _Head ){
		if(_Head->featrue == _feature){
			break;
		}
		_Head = _Head->same;
	}
	return _Head;
}

//Э����������ⲿ���ԭʼ���ݿ�ʼ����
void ProtocalManager::StartAnalyse(PacketInfo* RawPacket)
{
	ProtocalNode *temp = ProtocalTree_;
	temp->func(RawPacket);
	while ( NULL != temp->next){
		temp = Get_by_feature(RawPacket->nCurProt,temp->next);
		if(NULL != temp){
			if(temp->key== 0){  //C��������
				temp->func(RawPacket);
			}else{              //LUA��������
				doluaFunc(temp->key,RawPacket);
			}
		}else{
			break;
		}
	}
}

void ProtocalManager::doluaFunc(unsigned long key,PacketInfo* RawPacket){

	std::map<unsigned long,std::string>::iterator it = luaFuncName_.find(key);
	std::string name = it->second;
	//����LUA�������Ƚ�LUA��������ջ���ٽ�������ջ������������
	lua_getglobal(LuaState::instance().L,name.c_str());
	lua_pushlightuserdata(LuaState::instance().L,(void*)RawPacket);
	//����ģʽ�����У�����lua_call
	int err = lua_pcall(LuaState::instance().L,1,0,0);
	if(err!= 0){//��������
		const char * errstr = lua_tostring(LuaState::instance().L,-1);
		TRACE("%s",errstr);
		lua_pop(LuaState::instance().L,1);
	}
}

//��Э�������ע��Э����Ϣ
void ProtocalManager::Register_Ptotocal(const char *prot_name, unsigned int upper_prot, 
						unsigned short feature, fp_ProtocalAnalyse pfn){
	pProtocalNode parent = search(upper_prot);
	Add_New_Node(parent,prot_name,feature,pfn);
}

void ProtocalManager::Register_Protocal(const char * prot_name,unsigned int upper_prot,
					   unsigned short feature,const char * luafunc){

	pProtocalNode parent = search(upper_prot);
	Add_New_LuaNode(parent,prot_name,feature,luafunc);
}
ProtocalManager::ProtocalNode * ProtocalManager::search(unsigned short feature){

	std::queue<pProtocalNode> record_;
	pProtocalNode top = NULL;
	record_.push(ProtocalTree_);
	while (!record_.empty()){
			
		top = record_.front();
		record_.pop();
		if(top->featrue== feature){
				return top;
		}
		if(top->next!= NULL){
				record_.push(top->next);
		}
		if(top->same!= NULL){
				record_.push(top->same);
		}
	}
	return NULL;
}

void ProtocalManager::addPtotocalFeature(unsigned short feature,std::string name){

	protocalFeature_.insert(std::pair<unsigned short,std::string>(feature,name));
}

bool ProtocalManager::isExisting(unsigned short feature){

	std::map<unsigned short,std::string>::iterator it_ = protocalFeature_.find(feature);
	return (it_== protocalFeature_.end()) ? false : true;
}

const char * ProtocalManager::getProtocalName(unsigned short feature){

	std::map<unsigned short,std::string>::iterator it_  = protocalFeature_.find(feature);
	if(it_!= protocalFeature_.end()){
		return (it_->second).c_str();
	}
	return NULL;

}
	