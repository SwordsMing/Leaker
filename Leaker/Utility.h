/*

   此文件声明一些辅助性函数，用于协议解析
   by zhucong 2014/03/15

*/

#ifndef LEAKER_UTILITY_H_
#define LEKAER_UTILITY_H_

#include <string>

namespace Utility{

	//Tcp首部中的码元比特
	enum TcpBit
	{
		URG = 0,
		ACK,
		PSH,
		RST,
		SYN,
		FIN
	};

	//判断一个ＭＡＣ地址是否为广播包
	bool g_isBroadcast(const unsigned char * mac);

	//用于ＩＰ协议解析中的DSCP
	//取得字符串形如："＊＊＊＊　＊＊．．"的形式
	std::string g_getCodePoint(unsigned char dscp);


	//判断一个ＩＰ包是否可以进行分片
	//参数：ＩＰ首部的"标志－偏移"字段
	//返回值：true:不可以分片(ＤＦ)，false:能分片
	bool g_isFragment(unsigned short flag_offset);

	//计算ＩＰ首部检验和
	unsigned short getIPCheckSum(unsigned char * header);

	//判断ＴＣＰ首部中的码元比特置位情况
	//输入参数：flag,后６位为测试位
	//返回：true:被置位，false:未
	bool g_testTcpBit(unsigned short flag,TcpBit bit);

	//用诸如".... ..0. .... "的形式描述ＴＣＰ首部的置位情况
	std::string g_getTcpBitStr(unsigned short flag,TcpBit bit);

	//该函数判断MAC帧类型 802.3 or DIX Ethernet V2
	//返回值：true:Ethernet II,false:IEEE 802.3
	bool g_getMacType(unsigned short flag);

}



#endif