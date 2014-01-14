#include "plugin.h"
#include "../ProtocalManager.h"

void registerPtotocalInfo(const char * name,unsigned int upper,
							unsigned short feature,fp_ProtocalAnalyse pfn)
{
	ProtocalManager::instance().Register_Ptotocal(name,upper,feature,pfn);
}