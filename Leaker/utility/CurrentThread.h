#ifndef LEAKER_CURRENTTHREAD_H_
#define LEAKER_CURRENTTHREAD_H_

#include <windows.h>

namespace CurrentThread
{
	inline DWORD getThreadId()
	{
		return ::GetCurrentThreadId();
	}

	inline DWORD getProcessorNum()    //�õ���ǰOS����������
	{
		SYSTEM_INFO systeminfo_;
		::GetSystemInfo(&systeminfo_);
		return systeminfo_.dwNumberOfProcessors;
	}
}

#endif