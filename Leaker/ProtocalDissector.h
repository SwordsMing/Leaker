#ifndef LEAKER_PROTOCALDISSECTOR_H_
#define LEAKER_PROTOCALDISSECTOR_H_

/*
	TCP/IP��ϵ�и���Э�������
	created by zc in 2014/1/1
*/
#include "Protocal.h"

//TCP/IP��ϵ������Э���������ָ������
void g_MacAnalyse(PacketInfo* AnalysePacket);
void g_ArpAnalyse(PacketInfo* AnalysePacket);
void g_IpAnalyse(PacketInfo* AnalysePacket);
void g_TcpAnalyse(PacketInfo* AnalysePacket);
void g_UdpAnalyse(PacketInfo* AnalysePacket);

//�ж�һ����ַ�Ƿ�Ϊ�㲥��
bool g_isBroadcast(const unsigned char * mac);

#endif