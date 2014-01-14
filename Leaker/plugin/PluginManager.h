#ifndef LEAKER_PLUGIN_MANAGER_H_
#define LEAKER_PLUGIN_MANAGER_H_

/*

	该文件声明解析插件管理器
	说明：该插件管理器在程序启动时初使化，加载并向系统注册所有协议解码器插件，
	    初使化协议解码器。
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