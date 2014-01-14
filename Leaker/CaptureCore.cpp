#include "CaptureCore.h"
#include <boost/bind.hpp>
#include <cassert>
#include "ShowCore.h"
#include "ProtocalManager.h"

CaptureCore::CaptureCore()
	:start_(false),
	openedAdapter_(NULL),
	no_(0),
	captureThread_(NULL),
	parserThread_(NULL),
	showCore_(NULL)
{

}

CaptureCore::~CaptureCore(){
}

void CaptureCore::startCapture(const char * name){
	
	assert(!start_);
	start_ = true;
	char errbuf[PCAP_ERRBUF_SIZE];
	//打开设备
	openedAdapter_ = pcap_open_live(name,65536,0,1000,errbuf);
	if(openedAdapter_== NULL)
	{
		std::abort();
	}
	if(!filter_.IsEmpty())
	{
		setFilter(filter_);
	}
	captureThread_ = new Thread(boost::bind(&CaptureCore::captureFunc,this),_T("CaptureThread"));
	parserThread_  = new Thread(boost::bind(&CaptureCore::parserFunc,this),_T("ParserThread"));
	captureThread_->startThread();
	parserThread_->startThread();
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
	//for test CaptureThread exit
	//::MessageBox(NULL,_T("CaptureThread exit"),_T("test"),MB_OK);
}

void CaptureCore::parserFunc(){

	while (start_)
	{
		boost::mutex::scoped_lock lock(parserMutex_);
		while (parserBuf_.empty())
		{
			parserCondition_.wait(lock);
		}
		if(!parserBuf_.empty())
		{
			PacketInfo *pkt = parserBuf_.front();
			parserBuf_.pop_front();
			parser(pkt);
			showCore_->add(pkt);
		}
	}
}

void CaptureCore::parser(PacketInfo * packet){

	ProtocalManager::instance().StartAnalyse(packet);
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

	allCapturedPacketBuf_.push_back(packetInfo_);

	boost::mutex::scoped_lock lock(parserMutex_);
	parserBuf_.push_back(packetInfo_);
	parserCondition_.notify_one();
}

void CaptureCore::setFilter(const CStdString & filter){

	
}