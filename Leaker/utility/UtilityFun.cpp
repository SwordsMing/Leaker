#include "UtilityFun.h"
#include <windows.h>

namespace utility
{
	std::string getResourceDir(){

		char buf[MAX_PATH];
		memset(buf,0,sizeof(buf));
		::GetCurrentDirectory(sizeof(buf),buf);

		return std::string(buf);
	}
}