#include "PluginManager.h"
#include <string>
#include "LuaState.h"

PluginManager::PluginManager(){
	

}

PluginManager::~PluginManager(){
	
	std::vector<HMODULE>::iterator it;
	for (it= plugins_.begin(); it!= plugins_.end(); it++)
	{
		::FreeLibrary(*it);
	}
}

void PluginManager::registerCPlugins(){

	char szPluginPath[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH,szPluginPath);
	strcat(szPluginPath,_T("\\Plugin\\*.dll"));
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	BOOL res = 1;
	hFind = ::FindFirstFile(szPluginPath,&FindFileData);
	if(hFind == INVALID_HANDLE_VALUE){
		return;
	}
	memset(szPluginPath,0,sizeof(szPluginPath));
	::GetCurrentDirectory(MAX_PATH,szPluginPath);
	strcat(szPluginPath,_T("\\Plugin\\"));
	std::string Path = szPluginPath;
	while (res){
		std::string fullPath = Path + FindFileData.cFileName;
		HMODULE hModule = ::LoadLibrary(fullPath.c_str());
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

void PluginManager::registerLuaPlugins(){
	
	char szPluginPath[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH,szPluginPath);
	strcat(szPluginPath,_T("\\Plugin\\*.lua"));
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	BOOL res = 1;
	hFind = ::FindFirstFile(szPluginPath,&FindFileData);
	if(hFind == INVALID_HANDLE_VALUE){
		return;
	}
	memset(szPluginPath,0,sizeof(szPluginPath));
	::GetCurrentDirectory(MAX_PATH,szPluginPath);
	strcat(szPluginPath,_T("\\Plugin\\"));
	std::string Path = szPluginPath;
	while (res){
		std::string fullPath = Path + FindFileData.cFileName;
		luaL_dofile(LuaState::instance().L,fullPath.c_str());
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
		//::MessageBox(NULL,"call registerPlugin" ,NULL,MB_OK);
		pfn_Register(&registerPtotocalInfo);  //µ÷ÓÃ×¢²áº¯Êý
	}
}