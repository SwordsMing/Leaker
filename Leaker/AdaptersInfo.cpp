#include "AdaptersInfo.h"

AdaptersInfo::AdaptersInfo()
	:allDevsInfo_(NULL)
{
	init();
}

AdaptersInfo::~AdaptersInfo(){

	if(allDevsInfo_!= NULL){
		pcap_freealldevs(allDevsInfo_);
	}
}

void AdaptersInfo::refreshInterfaces(){

	pcap_freealldevs(allDevsInfo_);
	init();
}

void AdaptersInfo::init(){

	char errbuf[PCAP_ERRBUF_SIZE+1];
	if (pcap_findalldevs(&allDevsInfo_, errbuf) == -1)
	{	
		std::abort();
	}
}