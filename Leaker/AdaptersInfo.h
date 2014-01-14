/*
    主机网络适配器信息类
	created by zc in 2013/12/27 
*/

#ifndef LEAKER_ADAPTERSINFO_H_
#define LEAKER_ADAPTERSINFO_H_

#include "stdafx.h"
#include "utility/Singleton.h"
#include <vector>

class AdaptersInfo:public Singleton<AdaptersInfo>
{
public:
	AdaptersInfo();
	~AdaptersInfo();
	void refreshInterfaces();
	const pcap_if_t * getAllDevsInfo()const{return allDevsInfo_;}
private:
	pcap_if_t * allDevsInfo_;
	void init();
};

#endif 