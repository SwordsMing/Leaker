#ifndef LEAKER_CAPTURECORE_H_
#define LEAKER_CAPTURECORE_H_

#include "stdafx.h"
#include "utility/Singleton.h"
#include "utility/Thread.h"
#include <boost/thread.hpp>
#include <vector>
#include <deque>
#include "Protocal.h"

class ShowCore;

class CaptureCore: public Singleton<CaptureCore>
{
public:
	CaptureCore();
	~CaptureCore();
	void startCapture(const char * name);
	void stopCapture();
	void restartCapture();
	void captureFunc();
	void parserFunc();
	const PacketInfo * getPkt(int index)const{return allCapturedPacketBuf_[index];}
	void setShowCore(ShowCore * showcore){showCore_= showcore;}
	void setFilter(const CStdString & filter);
private:
	bool start_;
	unsigned long no_;
	Thread * captureThread_;
	Thread * parserThread_;
	ShowCore * showCore_;
	pcap_t *openedAdapter_;
	CStdString filter_;
	std::vector<PacketInfo *> allCapturedPacketBuf_;
	std::deque<PacketInfo *> parserBuf_;
	boost::mutex parserMutex_;
	boost::condition_variable_any parserCondition_;

	void parser(PacketInfo * packet);
	void dealCapturedPacket(const pcap_pkthdr *pkt_header, const u_char *pkt_data);
};

#endif