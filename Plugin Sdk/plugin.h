#ifndef LEAKER_PLUGIN_H_
#define LEAKER_PLUGIN_H_

#include "protocal.h"
#include "RootedTree.hpp"
/*

	�򵥵�C���ģ��
*/

#define _PLUGIN_DLL extern "C" _declspec(dllexport)

typedef void ( *fp_ProtocalAnalyse)(PacketInfo*);
typedef void (*fp_Prot_Register_Protocal)(const char *,unsigned int,unsigned short,fp_ProtocalAnalyse);

//������������ϵͳע��Э����Ϣ
_PLUGIN_DLL void registerPlugin(fp_Prot_Register_Protocal fp);

//�Զ����������
void ProtocalAnalyse(PacketInfo * info);

#endif