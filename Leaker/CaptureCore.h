#ifndef LEAKER_CAPTURECORE_H_
#define LEAKER_CAPTURECORE_H_

#include "stdafx.h"
#include "utility/Singleton.h"
#include "utility/Thread.h"
#include <boost/thread.hpp>
#include <vector>
#include "Protocal.h"

class ShowCore;

class CaptureCore: public Singleton<CaptureCore>
{
public:
	CaptureCore();
	~CaptureCore();
	void startCapture(const char * name);
	void startCapture(pcap_t *fp);
	void stopCapture();
	void restartCapture();
	void captureFunc();
	void dumpFile(const char * filename);
	const PacketInfo * getPkt(int index)const{return allCapturedPacketBuf_[index];}
	void setShowCore(ShowCore * showcore);
	void setFilter(const CStdString & filter);
private:
	bool start_;
	unsigned long no_;
	Thread * captureThread_;
	ShowCore * showCore_;
	pcap_t *openedAdapter_;
	CStdString filter_;
	void applyFilter(const CStdString & filter);
	std::vector<PacketInfo *> allCapturedPacketBuf_;

	void dealCapturedPacket(const pcap_pkthdr *pkt_header, const u_char *pkt_data);
};

#endif