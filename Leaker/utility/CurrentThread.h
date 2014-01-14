#ifndef LEAKER_CURRENTTHREAD_H_
#define LEAKER_CURRENTTHREAD_H_

#include <windows.h>

namespace CurrentThread
{
	inline DWORD getThreadId()
	{
		return ::GetCurrentThreadId();
	}

	inline DWORD getProcessorNum()    //得到当前OS处理器个数
	{
		SYSTEM_INFO systeminfo_;
		::GetSystemInfo(&systeminfo_);
		return systeminfo_.dwNumberOfProcessors;
	}
}

#endif