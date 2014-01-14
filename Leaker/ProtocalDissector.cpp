#include "stdafx.h"
#include "ProtocalDissector.h"
#include "Protocal.h"
#include <time.h>

bool g_isBroadcast(const unsigned char * mac){

	int i = 0;
	for (;i< 6; i++)
	{
		if(mac[i]!= 0xff){
			break;
		}
	}
	return (i==6) ? true : false;
}
void g_MacAnalyse(PacketInfo* AnalysePacket){
	
	const unsigned char * pBuffer = AnalysePacket->data_;
	DLC_Hdr * Mac_Head = (PDLC_Hdr)pBuffer;
	for (int i= 0; i< 6; ++i)
	{
		AnalysePacket->DestMAC[i] = Mac_Head->DestMAC[i];
		AnalysePacket->SrcMAC[i]  = Mac_Head->SrcMAC[i];
	}

	//一定要转换字节顺序，否则无法识别
	AnalysePacket->nCurProt = ntohs(Mac_Head->Ethertype);

	char buf[MAX_PATH];
	memset(buf,0,sizeof(buf));
	sprintf(buf,"%d",AnalysePacket->no_);
	AnalysePacket->columnInfo_.no_ = std::string(buf);
	memset(buf,0,sizeof(buf));

	sprintf(buf,"%d",AnalysePacket->caplen_);
	AnalysePacket->columnInfo_.length_ = std::string(buf);
	memset(buf,0,sizeof(buf));

	time_t  local_tv_sec = AnalysePacket->ts_.tv_sec;
	struct tm * ltime = localtime(&local_tv_sec);
	strftime(buf,sizeof(buf),"%H:%M:%S",ltime);
	AnalysePacket->columnInfo_.time_ =std::string(buf);
	memset(buf,0,sizeof(buf));
	sprintf(buf,"%02x:%02x:%02x:%02x:%02x:%02x",AnalysePacket->SrcMAC[0],AnalysePacket->SrcMAC[1],AnalysePacket->SrcMAC[2],AnalysePacket->SrcMAC[3],\
					AnalysePacket->SrcMAC[4],AnalysePacket->SrcMAC[5]);
	AnalysePacket->columnInfo_.source_ = std::string(buf);
	memset(buf,0,sizeof(buf));
	if(g_isBroadcast(AnalysePacket->DestMAC)){
		AnalysePacket->columnInfo_.destination_ = std::string(_T("Broadcast"));
	}
	else{
		sprintf(buf,"%02x:%02x:%02x:%02x:%02x:%02x",AnalysePacket->DestMAC[0],AnalysePacket->DestMAC[1],\
					AnalysePacket->DestMAC[2],AnalysePacket->DestMAC[3],AnalysePacket->DestMAC[4],AnalysePacket->DestMAC[5]);
		AnalysePacket->columnInfo_.destination_ = std::string(buf);
		memset(buf,0,sizeof(buf));
	}
	AnalysePacket->columnInfo_.protocal_ = std::string(_T("Mac"));


	RootedTree<std::string> *tree_ = new RootedTree<std::string>;
	AnalysePacket->detailInfo_.push_back(tree_);
	sprintf(buf,"Frame %d:%d bytes on wire,%d bytes captured",AnalysePacket->no_,AnalysePacket->len_,AnalysePacket->caplen_);
	HITEM root_ = tree_->insert(NULL,std::string(buf));
	memset(buf,0,sizeof(buf));
	sprintf(buf,"Arrival time:%s",AnalysePacket->columnInfo_.time_.c_str());
	tree_->insert(root_,std::string(buf));

	AnalysePacket->data_ += 14;
	AnalysePacket->leftLength_ -= 14; 
}


void g_ArpAnalyse(PacketInfo* AnalysePacket){

	in_addr send_ip,targ_ip;
	bool bGratuitous = false;      
	char buf[MAX_PATH];
	memset(buf,0,sizeof(buf));
	const unsigned char * pBuffer = AnalysePacket->data_;
	PARPFrame pArpFrame = (PARPFrame)pBuffer;
	send_ip.S_un.S_un_b.s_b1 = pArpFrame->Send_Prot_Addr[0];
	send_ip.S_un.S_un_b.s_b2 = pArpFrame->Send_Prot_Addr[1];
	send_ip.S_un.S_un_b.s_b3 = pArpFrame->Send_Prot_Addr[2];
	send_ip.S_un.S_un_b.s_b4 = pArpFrame->Send_Prot_Addr[3];

	targ_ip.S_un.S_un_b.s_b1 = pArpFrame->Tart_Prot_Addr[0];
	targ_ip.S_un.S_un_b.s_b2 = pArpFrame->Tart_Prot_Addr[1];
	targ_ip.S_un.S_un_b.s_b3 = pArpFrame->Tart_Prot_Addr[2];
	targ_ip.S_un.S_un_b.s_b4 = pArpFrame->Tart_Prot_Addr[3];
	
	for (int i= 0; i< 4;++i)
	{
		if(pArpFrame->Send_Prot_Addr[i] != pArpFrame->Tart_Prot_Addr[i])
		{
			bGratuitous = true;
			break;
		}
	}
	unsigned short opcode = ntohs(pArpFrame->Opcode);
	if(opcode== 1){

		AnalysePacket->columnInfo_.opcode_ = std::string(_T("request"));
		if(!bGratuitous)
		{	
			sprintf(buf,_T("Gratuious Arp for %s (Request)"),inet_ntoa(targ_ip));
		}
		else{
			sprintf(buf,_T("who has %s,tell %s"),inet_ntoa(targ_ip),inet_ntoa(send_ip));
		}
	}
	else{
		AnalysePacket->columnInfo_.opcode_ = std::string(_T("response"));
		sprintf(buf,_T("Arp respond to %s"),inet_ntoa(targ_ip));
	}
	AnalysePacket->columnInfo_.protocal_ = std::string(_T("Arp"));
	AnalysePacket->columnInfo_.info_ = std::string(buf);
}

void g_IpAnalyse(PacketInfo* AnalysePacket){    

	const unsigned char * pBuffer = AnalysePacket->data_;
	IP_Hdr * Ip_Hdr = (PIP_Hdr)pBuffer;

	char buf[MAX_PATH];
	memset(buf,0,sizeof(buf));
	for (int i= 0; i<4; ++i)
	{
		AnalysePacket->Src_IP[i] = Ip_Hdr->Src_IP[i];
		AnalysePacket->Des_IP[i] = Ip_Hdr->Des_IP[i];
	}
	//AnalysePacket->Data += AnalysePacket->Length;
	//AnalysePacket->nLeftLength -= AnalysePacket->Length;
	struct in_addr s_ip,d_ip;
	s_ip.S_un.S_un_b.s_b1 = AnalysePacket->Src_IP[0];
	s_ip.S_un.S_un_b.s_b2 = AnalysePacket->Src_IP[1];
	s_ip.S_un.S_un_b.s_b3 = AnalysePacket->Src_IP[2];
	s_ip.S_un.S_un_b.s_b4 = AnalysePacket->Src_IP[3];

	d_ip.S_un.S_un_b.s_b1 = AnalysePacket->Des_IP[0];
	d_ip.S_un.S_un_b.s_b2 = AnalysePacket->Des_IP[1];
	d_ip.S_un.S_un_b.s_b3 = AnalysePacket->Des_IP[2];
	d_ip.S_un.S_un_b.s_b4 = AnalysePacket->Des_IP[3];

	AnalysePacket->columnInfo_.source_ = std::string(inet_ntoa(s_ip));
	AnalysePacket->columnInfo_.destination_ = std::string(inet_ntoa(d_ip));
	AnalysePacket->columnInfo_.protocal_ = std::string(_T("ip"));
}
void g_TcpAnalyse(PacketInfo* AnalysePacket){

	const unsigned char * pBuffer = AnalysePacket->data_;
	TCP_Hdr * Tcp_Hdr = (PTCP_Hdr)pBuffer;
}
void g_UdpAnalyse(PacketInfo* AnalysePacket){

	const unsigned char * pBuffer = AnalysePacket->data_;
	UDP_Hdr * Udp_Hdr = (PUDP_Hdr)pBuffer;

	AnalysePacket->columnInfo_.protocal_ = std::string(_T("udp"));

	AnalysePacket->data_ += 8;
	AnalysePacket->leftLength_ -= 8;
}


