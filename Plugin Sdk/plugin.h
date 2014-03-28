#ifndef LEAKER_PLUGIN_H_
#define LEAKER_PLUGIN_H_

#include "protocal.h"
#include "RootedTree.hpp"
/*

	简单的C插件模块
*/

#define _PLUGIN_DLL extern "C" _declspec(dllexport)

typedef void ( *fp_ProtocalAnalyse)(PacketInfo*);
typedef void (*fp_Prot_Register_Protocal)(const char *,unsigned int,unsigned short,fp_ProtocalAnalyse);

//导出函数，向系统注册协议信息
_PLUGIN_DLL void registerPlugin(fp_Prot_Register_Protocal fp);

//自定义解析函数
void ProtocalAnalyse(PacketInfo * info);

#endif