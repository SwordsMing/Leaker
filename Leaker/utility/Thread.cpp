#include "Thread.h"
#include <process.h>
#include <cassert>

Thread::Thread(const Thread::functor &func, const std::string &name)
		:functor_(func),
		started_(false),
		name_(name),
		thread_(NULL),
		threadId_(0)
{

}

void Thread::startThread(){
	assert(!started_);
	thread_ = (HANDLE)::_beginthreadex(NULL,0,&runInThread,this,0,&threadId_);
	assert(thread_!=NULL);
	started_ = true;
}

unsigned int Thread::runInThread(void *lParam){
	Thread * this_ = static_cast<Thread*>(lParam);
	(this_->functor_)();
	return 0;
}

void Thread::join(){
	DWORD ret = ::WaitForSingleObject(thread_,INFINITE);
	assert( WAIT_OBJECT_0 == ret);
}
