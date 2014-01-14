#include "PluginManager.h"

PluginManager::PluginManager(){
	

}

PluginManager::~PluginManager(){
	
	std::vector<HMODULE>::iterator it;
	for (it= plugins_.begin(); it!= plugins_.end(); it++)
	{
		::FreeLibrary(*it);
	}
}

void PluginManager::registerPlugins(){

	char szPluginPath[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH,szPluginPath);
	strcat(szPluginPath,_T("\\plugin\\*.dll"));
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	BOOL res = 1;
	hFind = ::FindFirstFile(szPluginPath,&FindFileData);
	if(hFind == INVALID_HANDLE_VALUE){
		return;
	}
	while (res){
		HMODULE hModule = ::LoadLibrary(FindFileData.cFileName);
		if(hModule==NULL){
			return ;
		}
		plugins_.push_back(hModule);
		registerModule(hModule);
		res = ::FindNextFile(hFind,&FindFileData);
	}
	if(hFind!= INVALID_HANDLE_VALUE){
		::FindClose(hFind);
	}
}

void PluginManager::registerModule(HMODULE hModule){

	registerPlugin pfn_Register;

	pfn_Register = (registerPlugin)::GetProcAddress(hModule,_T("registerPlugin"));
	if( NULL != pfn_Register)
	{
		pfn_Register(&registerPtotocalInfo);  //µ÷ÓÃ×¢²áº¯Êý
	}
}