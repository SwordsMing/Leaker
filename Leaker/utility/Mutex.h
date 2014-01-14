#ifndef LEAKER_MUTEX_H_
#define LEAKER_MUTEX_H_

#include <boost/noncopyable.hpp>
#include <windows.h>
#include <cstdio>
#include "CurrentThread.h"
#include <cassert>

class MutexLock: boost::noncopyable
{
	public:
		MutexLock(BOOL initstate_ = FALSE)
			:holder_(0)
		{
			mutex_ = ::CreateMutex(NULL,initstate_,NULL);  //��ʹ��Ϊ����״̬
			assert(NULL != mutex_);
		}

		~MutexLock()
		{
			assert(0 == holder_);
			int ret = ::CloseHandle(mutex_);
			assert(0 != ret);
		}

		void lock()
		{
			DWORD ret = ::WaitForSingleObject(mutex_,INFINITE);
			assert( WAIT_OBJECT_0 == ret);
			holder_ = CurrentThread::getThreadId();
		}
		void release()
		{
			//assert( 0 != holder_);  //���̻߳����£������쳣
			int ret = ::ReleaseMutex(mutex_);
			assert( 0 != ret); //2013-03-10�����̳߳�ʱ�����쳣,1:��mutexͬʱ�������
			holder_ = 0;
		}
		
		bool assertLockedbyOwn()const
		{
			return holder_ == CurrentThread::getThreadId();
		}

		operator HANDLE()const
		{
			return mutex_;
		}
	private:
		HANDLE mutex_;
		unsigned long holder_;
	};

	class MutexWithGuard: boost::noncopyable
	{
	public:
		explicit MutexWithGuard(MutexLock & mutexlock)
			:MutexLock_(mutexlock)
		{                            
			MutexLock_.lock();
		}

		~MutexWithGuard()
		{
			MutexLock_.release();
		}
	private:
		MutexLock & MutexLock_;
};

#endif