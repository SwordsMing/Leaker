#include "CaptureCore.h"
#include <boost/bind.hpp>
#include <cassert>
#include "ShowCore.h"
#include "ProtocalManager.h"
#include "ParserCore.h"

CaptureCore::CaptureCore()
	:start_(false),
	openedAdapter_(NULL),
	no_(0),
	captureThread_(NULL),
	showCore_(NULL)
{

}

CaptureCore::~CaptureCore(){
}

void CaptureCore::startCapture(const char * name){
	
	assert(!start_);
	start_ = true;
	char errbuf[PCAP_ERRBUF_SIZE];
	memset(errbuf,0,sizeof(errbuf));
	//打开设备
	openedAdapter_ = pcap_open_live(name,65536,0,1000,errbuf);
	if(openedAdapter_== NULL)
	{
		::MessageBox(NULL,errbuf,NULL,MB_OK);
		std::abort();
	}
	if(!filter_.IsEmpty())
	{
		applyFilter(filter_);
	}
	captureThread_ = new Thread(boost::bind(&CaptureCore::captureFunc,this),_T("CaptureThread"));
	captureThread_->startThread();
	ParserCore::instance().startParser();
	showCore_->startShow();
}

void CaptureCore::startCapture(pcap_t * fp){
	
	assert(!start_);
	start_ = true;
	openedAdapter_ = fp;
	if(openedAdapter_== NULL)
	{
		std::abort();
	}
	captureThread_ = new Thread(boost::bind(&CaptureCore::captureFunc,this),_T("CaptureThread"));
	captureThread_->startThread();
	ParserCore::instance().startParser();
	showCore_->startShow();
}

void CaptureCore::stopCapture(){

	if(start_){
		start_ = false;
		captureThread_->join();
	}
}

void CaptureCore::restartCapture(){

	assert(start_);
}

void CaptureCore::captureFunc(){

	struct pcap_pkthdr *pkt_header =NULL;
	const unsigned char *pkt_data  = NULL;
	int res;
	while (start_)
	{
		res = pcap_next_ex(openedAdapter_,&pkt_header,&pkt_data);
		if(res== 1){
			//ok
			dealCapturedPacket(pkt_header,pkt_data);
		}
		else if(res== 0){
			//timeout
		}
		else if(res== -1){
			//error occurred
		}
		else{
			// offline capture
		}
	}
}


void CaptureCore::dealCapturedPacket(const pcap_pkthdr *pkt_header, const u_char *pkt_data){

	PacketInfo * packetInfo_ = new PacketInfo;
	packetInfo_->pureData_ = new unsigned char[pkt_header->caplen];
	memcpy((void*)packetInfo_->pureData_,pkt_data,pkt_header->caplen);
	packetInfo_->data_ = packetInfo_->pureData_;

	packetInfo_->no_ = no_++;
	packetInfo_->ts_ = pkt_header->ts;
	packetInfo_->leftLength_ = pkt_header->caplen;
	packetInfo_->len_ = pkt_header->len;
	packetInfo_->caplen_ = pkt_header->caplen;
	packetInfo_->header_ = *pkt_header;

	allCapturedPacketBuf_.push_back(packetInfo_);   //存储抓取到的数据包
 
	ParserCore::instance().add(packetInfo_);        //将数据包投递到协议分析器
}

void CaptureCore::setFilter(const CStdString & filter){
	
	filter_ = filter;
}

void CaptureCore::applyFilter(const CStdString & filter){

//	int netmask = 
}

void CaptureCore::setShowCore(ShowCore * showcore){

	showCore_= showcore;
	ParserCore::instance().setShowCore(showcore);
}

void CaptureCore::dumpFile(const char *filename){

	pcap_dumper_t *dumpfile = pcap_dump_open(openedAdapter_,filename);
	if(dumpfile== NULL){

		//err 
		return ;
	}
	std::vector<PacketInfo *>::iterator it_ = allCapturedPacketBuf_.begin();
	for (;it_!= allCapturedPacketBuf_.end(); it_++){

		pcap_dump((unsigned char *)dumpfile,&((*it_)->header_),(*it_)->pureData_);
	}
	pcap_dump_flush(dumpfile);
}