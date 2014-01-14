#ifndef LEAKER_PROTOCAL_H_
#define LEAKER_PROTOCAL_H_

#include "stdafx.h"
#include <vector>
#include <string>
#include "RootedTree.hpp"

#pragma pack(push,1)

//�ú����ж�MAC֡���� 802.3 or DIX Ethernet V2
inline void getMacType(unsigned short rhs){
	
}
//����ΪTCP/IPЭ����ϵ�и���Э���������

#define MAC		  0X0001

//��װ��MAC֡�е��ϲ�Э��-����MACͷ�ġ����͡��ֶ�
#define IP		  0X0800
//#define IPV6      0X08dd
#define ARP       0X0806
#define RARP      0X0835
#define NOVEL_IPX 0X8137

//��װ��IP֡�е��ϲ�Э��-����IPͷ�еġ�Э�顱�ֶ�
#define ICMP 1
#define IGMP 2
#define TCP	 6
#define UDP  17
#define EGP  8
#define IGP  9
#define OSPF 89
#define IPV6 41

//��װ��TCP֡�е�Ӧ�ò�Э�飭����TCPͷ�еĶ˿ں�
#define HTTP 80
#define FTP  21

//��װ��UDP֡�е�Ӧ�ò�Э�飭����UDPͷ�еĶ˿ں�
#define TFTP 69


/*���ָ�ʽ����̫��֡ͷ�ṹ*/
//DIX Ethernet V2��׼
//sizeof() = 14
typedef struct tagDLC_Hdr
{
	unsigned char	DestMAC[6];
	unsigned char 	SrcMAC[6];
	unsigned short 	Ethertype;
}DLC_Hdr,*PDLC_Hdr;

//Novel Ethernet��׼
typedef struct tagNovel_Ethernet
{
	unsigned char	DestMAC[6];
	unsigned char 	SrcMAC[6];
	unsigned short  Length;             //����
	unsigned short  flag;               //Ϊ0XFFFFFFFF��־ΪNovel Ethernet
}Novel_Ethernet;

//IEEE802.3/802.2 SAP��׼
typedef struct tagIEEE802_3_SAP
{
	unsigned char	DestMAC[6];
	unsigned char 	SrcMAC[6];
	unsigned short  Length;             //����
	unsigned char   DSAP;               //Ŀ�ķ�����ʵ�
	unsigned char   SSAP;               //Դ������ʵ�
	unsigned char   Cntl;               //����
}IEEE802_3_SAP;

//Ethernet SNAP
typedef struct tagEthe_SNAP
{
	unsigned char	DestMAC[6];
	unsigned char 	SrcMAC[6];
	unsigned short  Length;             //����
	unsigned char   DSAP;               //Ŀ�ķ�����ʵ�
	unsigned char   SSAP;               //Դ������ʵ�
	unsigned char   Cntl;               //����
	unsigned char   Ogt_Code[3];        //һ����Ϊȫ0
	unsigned short  Type;               //����
}Ethe_SNAP;

/**************************/


//ARP֡��װ
 typedef struct tagARPFrame
 {
	unsigned short	HW_Type;
	unsigned short 	prot_Type;
	unsigned char	HW_Addr_Len;
	unsigned char	Prot_Addr_Len;
	unsigned short	Opcode;              //1����2��Ӧ
	
	unsigned char	Send_HW_Addr[6];
	unsigned char	Send_Prot_Addr[4];
	unsigned char	Targ_HW_Addr[6];
	unsigned char	Tart_Prot_Addr[4];
	unsigned char	padding[18];
 }ARPFrame,*PARPFrame;
 
 //ARP-MAC֡��װ
 typedef struct tagARP_Packet
 {
	DLC_Hdr 		m_DLCHdr;
	ARPFrame		m_ARPFrame;
 }ARP_Packet,*PARP_Packet;

 //IPv4���ݰ��ײ���װ���̶�20�ֽ�
 typedef struct tagIP_Hdr
 {
	unsigned char 	Version_Length;       //�汾(4 bits) + �ײ����ȣ�4 bits��
	unsigned char 	Type_Service;         //һ�㲻ʹ��
	unsigned short 	Total_Length;		  //����IP���ݰ��ĳ���	
	unsigned short	Identification;		  //���ڷ�Ƭ����	
	unsigned short 	Flag_Offset;		  //��־(3 bits) + Ƭƫ��(13 bits)
	unsigned char	TTL;				  //��������
	unsigned char	Protocal;			  //����ʹ�õĺ���Э��	
	unsigned short	Header_Check;		  //�ײ������
	unsigned char	Src_IP[4];			  //ԴIP��ַ
	unsigned char	Des_IP[4];            //Ŀ��IP��ַ
	//unsigned int 	OP_pad;	              //ѡ�������
 }IP_Hdr,*PIP_Hdr;
 
 
 //TCP�ײ���װ,�̶�20�ֽ�
 typedef struct tagTCP_Hdr
 {
	unsigned short	Src_Port;			  //Դ�˿ں�
	unsigned short	Des_Port;			  //Ŀ�Ķ˿ں�
	unsigned long   Num;
	unsigned long   Ack_Num;
	unsigned short  Conp;                 //����ƫ�ƣ�����������λ
    unsigned short  Window;
	unsigned short  CheckSum;
	unsigned short  UrgPointer;
 }TCP_Hdr,*PTCP_Hdr;
 
 //UDP�ײ���װ 
 typedef struct tagUDP_Hdr
 {
	unsigned short	Src_Port;			  //Դ�˿ں�
	unsigned short	Des_Port;			  //Ŀ�Ķ˿ں�
	unsigned short 	Total_Length;         //���ݰ��ܳ���
	unsigned short	Packet_Check;         //�����
 }UDP_Hdr,*PUDP_Hdr;

 //�ýṹ����Э���������
 typedef struct tagProtData
 {
	unsigned short  nCurProt;              //��ǰҪ������Э������
	int             nLeftLength;           //��һ��Э�����ݳ���
	int             nFrameNum;             //����֡���
	int             nRow;                  //��ʾ��
	bool            bflag;                 //true:��ֹ��ʾ
	bool            bAppProt;              //����Ӧ�ò�Э��������� 
	//HTREEITEM       UpperItem;             //��һ��������� 
	const unsigned char   *Data;           //ָ��ǰ��Ҫ����������
	unsigned int	Wire;
	unsigned int	Captured;
	struct timeval  ts;

	unsigned char	DestMAC[6];
	unsigned char 	SrcMAC[6];
	unsigned short 	Ethertype;

	unsigned char 	Version;			  //�汾(4 bits) 
	unsigned char   Length;               //�ײ����ȣ�4 bits��
	unsigned short 	Total_Length;		  //����IP���ݰ��ĳ���	
	//unsigned short	Identification;	  //���ڷ�Ƭ����	
	//unsigned short 	Flag_Offset;	  //��־(3 bits) + Ƭƫ��(13 bits)
	unsigned char	TTL;				  //��������
	unsigned char	Protocal;			  //����ʹ�õĺ���Э��	
	unsigned short	Header_Check;		  //�ײ������
	unsigned char	Src_IP[4];			  //ԴIP��ַ
	unsigned char	Des_IP[4];            //Ŀ��IP��ַ

	union
	{
		struct
		{
			unsigned short	Src_Port;	   //Դ�˿ں�
			unsigned short	Des_Port;	   //Ŀ�Ķ˿ں�
			unsigned long   Num;
			unsigned long   Ack_Num;
			unsigned char   Conp;          //����ƫ��
			unsigned short  CheckSum;
		} TcpHead;
		struct
		{
			unsigned short	Src_Port;			  //Դ�˿ں�
			unsigned short	Des_Port;			  //Ŀ�Ķ˿ں�
			unsigned short 	Total_Length;         //���ݰ��ܳ���
			unsigned short	Packet_Check;         //�����
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
	unsigned char	Src_IP[4];			  //ԴIP��ַ
	unsigned char	Des_IP[4];            //Ŀ��IP��ַ
}PacketInfo;

#pragma pack(pop) 
#endif