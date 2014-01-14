#ifndef LEAKER_PLUGIN_H_
#define LEAKER_PLUGIN_H_

#include "../protocal.h"

/*

	简单的C插件模块
*/

typedef void ( *fp_ProtocalAnalyse)(PacketInfo*);
typedef void (*fp_Prot_Register_Protocal)(const char *,unsigned int,unsigned short,fp_ProtocalAnalyse);
/*
	插件模块用于向协议管理器注册协议信息
	在插件管理器注册解析插件时会向插件模块传入该函数指针供其调用
	参数：name 协议名
		  upper 该协议的上层协议特征字
		  feature 该协议特征字
		  pfn 协议解析函数指针
	返回值:无
*/
void registerPtotocalInfo(const char * name,unsigned int upper,
							unsigned short feature,fp_ProtocalAnalyse pfn);

/*

	解析插件模块实现的接口函数，由插件管理器加载插件时调用
	参数：注册协议解析函数指针
*/
typedef void (*registerPlugin)(fp_Prot_Register_Protocal);

#endif