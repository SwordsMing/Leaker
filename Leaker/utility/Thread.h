#ifndef LEAKER_THREAD_H_
#define LEAKER_THREAD_H_

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <string>
#include "CurrentThread.h"


class Thread:public boost::noncopyable
{
public:
	typedef boost::function<void()> functor;

	explicit Thread(const functor & func,const std::string & name=std::string());
	~Thread(){}
	void join();
	void startThread();
	unsigned int getThreadId()const{
		return threadId_;
	}
	operator HANDLE()const{
		return thread_;
	}
private:
	bool started_;
	functor functor_;
	std::string name_;
	HANDLE thread_;
	unsigned int threadId_;
	static unsigned int __stdcall runInThread(void *lParam);

};



#endif