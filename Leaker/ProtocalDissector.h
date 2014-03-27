#ifndef LEAKER_PROTOCALDISSECTOR_H_
#define LEAKER_PROTOCALDISSECTOR_H_

/*
	TCP/IP体系中各层协议解析器
	created by zc in 2014/1/1
*/
#include "Protocal.h"

//TCP/IP体系下三层协议解析函数指针类型
void g_MacAnalyse(PacketInfo* AnalysePacket);
void g_ArpAnalyse(PacketInfo* AnalysePacket);
void g_IpAnalyse(PacketInfo* AnalysePacket);
void g_TcpAnalyse(PacketInfo* AnalysePacket);
void g_UdpAnalyse(PacketInfo* AnalysePacket);


#endif