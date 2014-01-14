#ifndef LEAKER_PLUGIN_H_
#define LEAKER_PLUGIN_H_

#include "../protocal.h"

/*

	�򵥵�C���ģ��
*/

typedef void ( *fp_ProtocalAnalyse)(PacketInfo*);
typedef void (*fp_Prot_Register_Protocal)(const char *,unsigned int,unsigned short,fp_ProtocalAnalyse);
/*
	���ģ��������Э�������ע��Э����Ϣ
	�ڲ��������ע��������ʱ������ģ�鴫��ú���ָ�빩�����
	������name Э����
		  upper ��Э����ϲ�Э��������
		  feature ��Э��������
		  pfn Э���������ָ��
	����ֵ:��
*/
void registerPtotocalInfo(const char * name,unsigned int upper,
							unsigned short feature,fp_ProtocalAnalyse pfn);

/*

	�������ģ��ʵ�ֵĽӿں������ɲ�����������ز��ʱ����
	������ע��Э���������ָ��
*/
typedef void (*registerPlugin)(fp_Prot_Register_Protocal);

#endif