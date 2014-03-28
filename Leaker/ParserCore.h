#ifndef LEAKER_PARSERCORE_H_
#define LEAKER_PARSERCORE_H_

#include "utility/Singleton.h"
#include "utility/BlockingQueue.hpp"
#include "Protocal.h"
#include "utility/Thread.h"

class ShowCore;
class ParserCore: public Singleton<ParserCore>
{
public:
	ParserCore()
		:start_(false),
		parserThread_(NULL),
		showCore_(NULL)
	{}
	void startParser();
	void parserFunc();
	void setShowCore(ShowCore * showcore){showCore_= showcore;}
	void add(PacketInfo *pkt){parserQueue_.put(pkt);}
private:
	bool start_;
	Thread * parserThread_;
	BlockingQueue<PacketInfo *> parserQueue_;
	ShowCore * showCore_;
	void parser(PacketInfo * packet);
};

#endif