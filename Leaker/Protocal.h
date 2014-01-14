#ifndef LEAKER_PROTOCAL_H_
#define LEAKER_PROTOCAL_H_

#include "stdafx.h"
#include <vector>
#include <string>
#include "RootedTree.hpp"

#pragma pack(push,1)

//该函数判断MAC帧类型 802.3 or DIX Ethernet V2
inline void getMacType(unsigned short rhs){
	
}
//以下为TCP/IP协议体系中各层协议的特征字

#define MAC		  0X0001

//封装在MAC帧中的上层协议-基于MAC头的“类型”字段
#define IP		  0X0800
//#define IPV6      0X08dd
#define ARP       0X0806
#define RARP      0X0835
#define NOVEL_IPX 0X8137

//封装在IP帧中的上层协议-基于IP头中的“协议”字段
#define ICMP 1
#define IGMP 2
#define TCP	 6
#define UDP  17
#define EGP  8
#define IGP  9
#define OSPF 89
#define IPV6 41

//封装在TCP帧中的应用层协议－基于TCP头中的端口号
#define HTTP 80
#define FTP  21

//封装在UDP帧中的应用层协议－基于UDP头中的端口号
#define TFTP 69


/*四种格式的以太网帧头结构*/
//DIX Ethernet V2标准
//sizeof() = 14
typedef struct tagDLC_Hdr
{
	unsigned char	DestMAC[6];
	unsigned char 	SrcMAC[6];
	unsigned short 	Ethertype;
}DLC_Hdr,*PDLC_Hdr;

//Novel Ethernet标准
typedef struct tagNovel_Ethernet
{
	unsigned char	DestMAC[6];
	unsigned char 	SrcMAC[6];
	unsigned short  Length;             //长度
	unsigned short  flag;               //为0XFFFFFFFF标志为Novel Ethernet
}Novel_Ethernet;

//IEEE802.3/802.2 SAP标准
typedef struct tagIEEE802_3_SAP
{
	unsigned char	DestMAC[6];
	unsigned char 	SrcMAC[6];
	unsigned short  Length;             //长度
	unsigned char   DSAP;               //目的服务访问点
	unsigned char   SSAP;               //源服务访问点
	unsigned char   Cntl;               //控制
}IEEE802_3_SAP;

//Ethernet SNAP
typedef struct tagEthe_SNAP
{
	unsigned char	DestMAC[6];
	unsigned char 	SrcMAC[6];
	unsigned short  Length;             //长度
	unsigned char   DSAP;               //目的服务访问点
	unsigned char   SSAP;               //源服务访问点
	unsigned char   Cntl;               //控制
	unsigned char   Ogt_Code[3];        //一般设为全0
	unsigned short  Type;               //类型
}Ethe_SNAP;

/**************************/


//ARP帧封装
 typedef struct tagARPFrame
 {
	unsigned short	HW_Type;
	unsigned short 	prot_Type;
	unsigned char	HW_Addr_Len;
	unsigned char	Prot_Addr_Len;
	unsigned short	Opcode;              //1请求，2响应
	
	unsigned char	Send_HW_Addr[6];
	unsigned char	Send_Prot_Addr[4];
	unsigned char	Targ_HW_Addr[6];
	unsigned char	Tart_Prot_Addr[4];
	unsigned char	padding[18];
 }ARPFrame,*PARPFrame;
 
 //ARP-MAC帧封装
 typedef struct tagARP_Packet
 {
	DLC_Hdr 		m_DLCHdr;
	ARPFrame		m_ARPFrame;
 }ARP_Packet,*PARP_Packet;

 //IPv4数据包首部封装，固定20字节
 typedef struct tagIP_Hdr
 {
	unsigned char 	Version_Length;       //版本(4 bits) + 首部长度（4 bits）
	unsigned char 	Type_Service;         //一般不使用
	unsigned short 	Total_Length;		  //完整IP数据包的长度	
	unsigned short	Identification;		  //用于分片重组	
	unsigned short 	Flag_Offset;		  //标志(3 bits) + 片偏移(13 bits)
	unsigned char	TTL;				  //跳数限制
	unsigned char	Protocal;			  //数据使用的何种协议	
	unsigned short	Header_Check;		  //首部检验和
	unsigned char	Src_IP[4];			  //源IP地址
	unsigned char	Des_IP[4];            //目的IP地址
	//unsigned int 	OP_pad;	              //选项与填充
 }IP_Hdr,*PIP_Hdr;
 
 
 //TCP首部封装,固定20字节
 typedef struct tagTCP_Hdr
 {
	unsigned short	Src_Port;			  //源端口号
	unsigned short	Des_Port;			  //目的端口号
	unsigned long   Num;
	unsigned long   Ack_Num;
	unsigned short  Conp;                 //数据偏移，保留，控制位
    unsigned short  Window;
	unsigned short  CheckSum;
	unsigned short  UrgPointer;
 }TCP_Hdr,*PTCP_Hdr;
 
 //UDP首部封装 
 typedef struct tagUDP_Hdr
 {
	unsigned short	Src_Port;			  //源端口号
	unsigned short	Des_Port;			  //目的端口号
	unsigned short 	Total_Length;         //数据包总长度
	unsigned short	Packet_Check;         //检验和
 }UDP_Hdr,*PUDP_Hdr;

 //该结构用于协议解析函数
 typedef struct tagProtData
 {
	unsigned short  nCurProt;              //当前要解析的协议特征
	int             nLeftLength;           //下一层协议数据长度
	int             nFrameNum;             //网络帧序号
	int             nRow;                  //显示列
	bool            bflag;                 //true:静止显示
	bool            bAppProt;              //有无应用层协议具休数据 
	//HTREEITEM       UpperItem;             //上一层树结点句柄 
	const unsigned char   *Data;           //指向当前需要解析的数据
	unsigned int	Wire;
	unsigned int	Captured;
	struct timeval  ts;

	unsigned char	DestMAC[6];
	unsigned char 	SrcMAC[6];
	unsigned short 	Ethertype;

	unsigned char 	Version;			  //版本(4 bits) 
	unsigned char   Length;               //首部长度（4 bits）
	unsigned short 	Total_Length;		  //完整IP数据包的长度	
	//unsigned short	Identification;	  //用于分片重组	
	//unsigned short 	Flag_Offset;	  //标志(3 bits) + 片偏移(13 bits)
	unsigned char	TTL;				  //跳数限制
	unsigned char	Protocal;			  //数据使用的何种协议	
	unsigned short	Header_Check;		  //首部检验和
	unsigned char	Src_IP[4];			  //源IP地址
	unsigned char	Des_IP[4];            //目的IP地址

	union
	{
		struct
		{
			unsigned short	Src_Port;	   //源端口号
			unsigned short	Des_Port;	   //目的端口号
			unsigned long   Num;
			unsigned long   Ack_Num;
			unsigned char   Conp;          //数据偏移
			unsigned short  CheckSum;
		} TcpHead;
		struct
		{
			unsigned short	Src_Port;			  //源端口号
			unsigned short	Des_Port;			  //目的端口号
			unsigned short 	Total_Length;         //数据包总长度
			unsigned short	Packet_Check;         //检验和
		} UdpHead;
	}Trans_Head;
}ProtData,*pProtData;

typedef struct tagColumnInfo
{
	std::string no_;
	std::string time_;
	std::string source_;
	std::string destination_;
	std::string protocal_;
	std::string length_;
	std::string opcode_;
	std::string info_;
}ColumnInfo;


typedef struct tagPacketInfo
{
	unsigned long no_;
	struct timeval ts_;
	unsigned char * pureData_;
	unsigned char * data_;
	unsigned int leftLength_;
	unsigned int len_;
	unsigned int caplen_;
	unsigned short nCurProt;
	ColumnInfo columnInfo_;
	std::vector<RootedTree<std::string>* > detailInfo_;

	unsigned char	DestMAC[6];
	unsigned char 	SrcMAC[6];
	unsigned short  Ethertype;
	unsigned char	Src_IP[4];			  //源IP地址
	unsigned char	Des_IP[4];            //目的IP地址
}PacketInfo;

#pragma pack(pop) 
#endif