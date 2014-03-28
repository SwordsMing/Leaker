#include "ParserCore.h"
#include <assert.h>
#include "ProtocalManager.h"
#include "ShowCore.h"

void ParserCore::startParser(){

	assert(!start_);
	start_ = true;
	parserThread_  = new Thread(boost::bind(&ParserCore::parserFunc,this),_T("ParserThread"));
	parserThread_->startThread();
}

void ParserCore::parserFunc(){

	while (start_)
	{
		PacketInfo *pkt = parserQueue_.take();
		parser(pkt);
		showCore_->add(pkt);
	}
}

void ParserCore::parser(PacketInfo * packet){

	ProtocalManager::instance().StartAnalyse(packet);	
}