#ifndef LEAKER_PLUGIN_MANAGER_H_
#define LEAKER_PLUGIN_MANAGER_H_

/*

	���ļ������������������
	˵�����ò���������ڳ�������ʱ��ʹ�������ز���ϵͳע������Э������������
	    ��ʹ��Э���������
	created by zhucong in 2012/08/01
	fixed in 2014/1/11
*/
#include "plugin.h"
#include "../utility/Singleton.h"
#include <vector>
class PluginManager: public Singleton<PluginManager>
{
public:
	PluginManager();
	~PluginManager();
	void registerPlugins();
private:
	std::vector<HMODULE> plugins_;
	void registerModule(HMODULE);
};
#endif