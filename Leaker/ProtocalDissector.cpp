#include "stdafx.h"
#include "ProtocalDissector.h"
#include "ProtocalManager.h"
#include "Protocal.h"
#include <time.h>
#include <map>
#include "Utility.h"

using namespace Utility;

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
	unsigned short type = ntohs(Mac_Head->Ethertype);
	char buf[MAX_PATH];
	memset(buf,0,sizeof(buf));
	sprintf(buf,"%d",AnalysePacket->no_);
	AnalysePacket->columnInfo_.no_ = std::string(buf);
	memset(buf,0,sizeof(buf));

	sprintf(buf,"%d",AnalysePacket->caplen_);
	AnalysePacket->columnInfo_.length_ = std::string(buf);
	memset(buf,0,sizeof(buf));

	time_t local_tv_sec = AnalysePacket->ts_.tv_sec;
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

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Frame Number:%d",AnalysePacket->no_);
	tree_->insert(root_,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Frame Length:%d bytes (%d bits)",AnalysePacket->len_,AnalysePacket->len_*8);
	tree_->insert(root_,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Capture Length:%d bytess (%d bits)",AnalysePacket->caplen_,AnalysePacket->caplen_*8);
	tree_->insert(root_,std::string(buf));

	memset(buf,0,sizeof(buf));
	RootedTree<std::string> *tree1_ = new RootedTree<std::string>;
	AnalysePacket->detailInfo_.push_back(tree1_);
	HITEM root2;	
	if(g_getMacType(ntohs(Mac_Head->Ethertype))){  //Ｅthernet II
	
		sprintf(buf,"Ethernet II,Src: %02x:%02x:%02x:%02x:%02x:%02x,Destination: \
				%02x:%02x:%02x:%02x:%02x:%02x",Mac_Head->SrcMAC[0],Mac_Head->SrcMAC[1],\
				Mac_Head->SrcMAC[2],Mac_Head->SrcMAC[3],Mac_Head->SrcMAC[4],Mac_Head->SrcMAC[5],\
				Mac_Head->DestMAC[0],Mac_Head->DestMAC[1],Mac_Head->DestMAC[2],Mac_Head->DestMAC[3],\
				Mac_Head->DestMAC[4],Mac_Head->DestMAC[5]);
		root2 = tree1_->insert(NULL,std::string(buf));

		memset(buf,0,sizeof(buf));
		const char * name = ProtocalManager::instance().getProtocalName(type);
		if(name!= NULL){
			sprintf(buf,"Type: %d (%s)",type,name);
		}else{
			sprintf(buf,"Type: %d",type);
		}
		tree1_->insert(root2,std::string(buf));
	}else{ //IEEE 802.3
		root2 = tree1_->insert(NULL,"IEEE 802.3 Ethernet");
		memset(buf,0,sizeof(buf));
		sprintf(buf,"Length: %d",ntohs(Mac_Head->Ethertype));
		tree1_->insert(root2,std::string(buf));

		//LLC layer
		RootedTree<std::string> *tree2_ = new RootedTree<std::string>;
		AnalysePacket->detailInfo_.push_back(tree2_);
		IEEE802_3_SAP * header = (IEEE802_3_SAP*)pBuffer;
		HITEM root3 = tree2_->insert(NULL,"Logical-Link Control");

		memset(buf,0,sizeof(buf));
		sprintf(buf,"DSAP: %d",header->DSAP);
		tree2_->insert(root3,std::string(buf));

		memset(buf,0,sizeof(buf));
		sprintf(buf,"SSAP: %d",header->SSAP);
		tree2_->insert(root3,std::string(buf));

		memset(buf,0,sizeof(buf));
		sprintf(buf,"Control: %d",header->Cntl);
		tree2_->insert(root3,std::string(buf));
	}

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Destination: %02x:%02x:%02x:%02x:%02x:%02x",Mac_Head->DestMAC[0],Mac_Head->DestMAC[1],Mac_Head->DestMAC[2],Mac_Head->DestMAC[3],\
				Mac_Head->DestMAC[4],Mac_Head->DestMAC[5]);
	tree1_->insert(root2,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Source:  %02x:%02x:%02x:%02x:%02x:%02x",Mac_Head->SrcMAC[0],Mac_Head->SrcMAC[1],\
			Mac_Head->SrcMAC[2],Mac_Head->SrcMAC[3],Mac_Head->SrcMAC[4],Mac_Head->SrcMAC[5]);
	tree1_->insert(root2,std::string(buf));	

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

	RootedTree<std::string> *tree_ = new RootedTree<std::string>;
	AnalysePacket->detailInfo_.push_back(tree_);

	memset(buf,0,sizeof(buf));
	sprintf(buf,"%s","Address Resolution Protocal");
	if(opcode== 1){
		strcat(buf,"(request)");
	}
	else{
		strcat(buf,"(response)");
	}
	HITEM root = tree_->insert(NULL,std::string(buf));
	tree_->insert(root,std::string("Hardware type:Ethernet(1)"));
	tree_->insert(root,std::string("Protocal type:Ip (0x0800)"));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Hardware size:%d",pArpFrame->HW_Addr_Len);
	tree_->insert(root,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Protocal size:%d",pArpFrame->Prot_Addr_Len);
	tree_->insert(root,std::string(buf));

	memset(buf,0,sizeof(buf));
	if(opcode==1){
		sprintf(buf,"%s","Opcode:requeset");
	}
	else{
		sprintf(buf,"%s","Opcode:response");
	}
	tree_->insert(root,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Sender MAC Address: %02x:%02x:%02x:%02x:%02x:%02x",\
		pArpFrame->Send_HW_Addr[0],pArpFrame->Send_HW_Addr[1],\
		pArpFrame->Send_HW_Addr[2],pArpFrame->Send_HW_Addr[3],\
		pArpFrame->Send_HW_Addr[4],pArpFrame->Send_HW_Addr[5]);

	tree_->insert(root,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Sender IP Address: %d.%d.%d.%d",\
		pArpFrame->Send_Prot_Addr[0],pArpFrame->Send_Prot_Addr[1],\
		pArpFrame->Send_Prot_Addr[2],pArpFrame->Send_Prot_Addr[3]);
	
	tree_->insert(root,std::string(buf));


	memset(buf,0,sizeof(buf));
	sprintf(buf,"Target MAC Address: %02x:%02x:%02x:%02x:%02x:%02x",\
		pArpFrame->Targ_HW_Addr[0],pArpFrame->Targ_HW_Addr[1],\
		pArpFrame->Targ_HW_Addr[2],pArpFrame->Targ_HW_Addr[3],\
		pArpFrame->Targ_HW_Addr[4],pArpFrame->Targ_HW_Addr[5]);

	tree_->insert(root,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Target IP Address: %d.%d.%d.%d",\
		pArpFrame->Tart_Prot_Addr[0],pArpFrame->Tart_Prot_Addr[1],\
		pArpFrame->Tart_Prot_Addr[2],pArpFrame->Tart_Prot_Addr[3]);
	
	tree_->insert(root,std::string(buf));
}

void g_IpAnalyse(PacketInfo* AnalysePacket){    

	const unsigned char * pBuffer = AnalysePacket->data_;
	IP_Hdr * Ip_Hdr = (PIP_Hdr)pBuffer;
	AnalysePacket->nCurProt = Ip_Hdr->Protocal;
	char buf[MAX_PATH];
	memset(buf,0,sizeof(buf));
	for (int i= 0; i<4; ++i)
	{
		AnalysePacket->Src_IP[i] = Ip_Hdr->Src_IP[i];
		AnalysePacket->Des_IP[i] = Ip_Hdr->Des_IP[i];
	}
	
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

	RootedTree<std::string> *tree_ = new RootedTree<std::string>;
	AnalysePacket->detailInfo_.push_back(tree_);
	
	int ip_version = Ip_Hdr->Version_Length >> 4;
	int header_len = (Ip_Hdr->Version_Length & 15) * 4;

	sprintf(buf,"Internet Protocal Version %d, Src: %d.%d.%d.%d,Dst:%d.%d.%d.%d",\
				ip_version,AnalysePacket->Src_IP[0],AnalysePacket->Src_IP[1],\
				AnalysePacket->Src_IP[2],AnalysePacket->Src_IP[3],\
				AnalysePacket->Des_IP[0],AnalysePacket->Des_IP[1],\
				AnalysePacket->Des_IP[2],AnalysePacket->Des_IP[3]);

	HITEM root = tree_->insert(NULL,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"version:%d",ip_version);
	tree_->insert(root,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Header Lenght:%d",header_len);
	tree_->insert(root,std::string(buf));

	memset(buf,0,sizeof(buf));
	
	//区分码点服务
	int dscp = Ip_Hdr->Type_Service;
	sprintf(buf,"Differentiated Services Filed: %02x (DSCP %02x)",dscp,dscp);
	HITEM node1 = tree_->insert(root,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"%s = Differentiated Services Code Point: Default (%02x)",\
		g_getCodePoint(Ip_Hdr->Type_Service),Ip_Hdr->Type_Service>>6);
	tree_->insert(node1,std::string(buf));

	memset(buf,0,sizeof(buf));
	int total_len = ntohs(Ip_Hdr->Total_Length);
	sprintf(buf,"Total length:%d",total_len);
	tree_->insert(node1,std::string(buf));

	memset(buf,0,sizeof(buf));
	int identification = ntohs(Ip_Hdr->Identification);
	sprintf(buf,"Identification:0x%x (%d)",identification);
	tree_->insert(node1,std::string(buf));

	memset(buf,0,sizeof(buf));
	unsigned short flag_offset = ntohs(Ip_Hdr->Flag_Offset);
	unsigned short flag = flag_offset >> 13;
	unsigned short offset = flag_offset & 0x1FFF;
	if(g_isFragment(flag_offset)){
		sprintf(buf,"Flags: 0x%02x (Don't Fragment)");
	}else{
		sprintf(buf,"Flags: 0x%02x (Can Fragment)");
	}
	HITEM node2 = tree_->insert(root,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Fragment Offset: %d",offset);
	tree_->insert(node2,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Time To Live: %d",Ip_Hdr->TTL);
	tree_->insert(node2,std::string(buf));

	memset(buf,0,sizeof(buf));
	const char * name = ProtocalManager::instance().getProtocalName(Ip_Hdr->Protocal);
	if(name!= NULL){
		sprintf(buf,"Protocal: %d (%s)",Ip_Hdr->Protocal,name);
	}else{
		sprintf(buf,"Protocal: %d",Ip_Hdr->Protocal);
	}
	tree_->insert(node2,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Header Checksum: 0x%x",ntohs(Ip_Hdr->Header_Check));

	tree_->insert(root,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Source: %d.%d.%d.%d",AnalysePacket->Src_IP[0],AnalysePacket->Src_IP[1],\
		AnalysePacket->Src_IP[2],AnalysePacket->Src_IP[3]);
	tree_->insert(root,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Destination: %d.%d.%d.%d",AnalysePacket->Des_IP[0],AnalysePacket->Des_IP[1],\
		AnalysePacket->Des_IP[2],AnalysePacket->Des_IP[3]);

	AnalysePacket->data_ += header_len;
	AnalysePacket->leftLength_ -= header_len; 

}
void g_TcpAnalyse(PacketInfo* AnalysePacket){

	const unsigned char * pBuffer = AnalysePacket->data_;
	TCP_Hdr * Tcp_Hdr = (PTCP_Hdr)pBuffer;
	
	
	AnalysePacket->columnInfo_.protocal_ = std::string(_T("Tcp"));

	char buf[MAX_PATH];
	RootedTree<std::string> *tree_ = new RootedTree<std::string>;
	AnalysePacket->detailInfo_.push_back(tree_);

	unsigned short s_port = ntohs(Tcp_Hdr->Src_Port);
	unsigned short d_port = ntohs(Tcp_Hdr->Des_Port);
	unsigned long seq_num = ntohl(Tcp_Hdr->Num);
	unsigned long ack_num = ntohl(Tcp_Hdr->Ack_Num);
	unsigned short header_len = (ntohs(Tcp_Hdr->Conp) >> 12) * 4;
	unsigned short window = ntohs(Tcp_Hdr->Window);
	unsigned short flag = ntohs(Tcp_Hdr->Conp) & 0x003F;
	unsigned short checksum = ntohs(Tcp_Hdr->CheckSum);


	memset(buf,0,sizeof(buf));
	sprintf(buf,"Transmission Contoal Protocal,Src port:%d,Dst port:%d",\
		s_port,d_port);
	HITEM root = tree_->insert(NULL,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Source Port: %d",s_port);
	tree_->insert(root,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Destination Port: %d",d_port);
	tree_->insert(root,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Sequence number: %ul",seq_num);
	tree_->insert(root,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Ack number: %ul",ack_num);
	tree_->insert(root,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Header length: %d",header_len);
	tree_->insert(root,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Flags: 0x%03x",flag);
	HITEM node1 = tree_->insert(root,std::string(buf));

	std::map<TcpBit,std::string> setInfo_;
	std::map<TcpBit,std::string> notSetInfo_;

	setInfo_[URG] = " = Urgent: set";
	setInfo_[ACK] = " = Acklowledgement: set";
	setInfo_[PSH] = " = Push: set";
	setInfo_[RST] = " = Reset: set";
	setInfo_[SYN] = " = Syn: set";
	setInfo_[FIN] = " =  Fin: set";

	notSetInfo_[URG] = " = Urgent: Not set";
	notSetInfo_[ACK] = " = Acklowledgement: Not set";
	notSetInfo_[PSH] = " = Push: Not set";
	notSetInfo_[RST] = " = Reset: Not set";
	notSetInfo_[SYN] = " = Syn: Not set";
	notSetInfo_[FIN] = " = Fin: Not set";


	std::map<TcpBit,std::string>::iterator it_;
	for (TcpBit tmp= URG; tmp<=FIN; tmp = (TcpBit)((int)tmp + 1)){
		memset(buf,0,sizeof(buf));
		std::string str = g_getTcpBitStr(flag,tmp);
		strcat(buf,str.c_str());
		if(g_testTcpBit(flag,tmp)){
			it_ = setInfo_.find(tmp);
		}else{
			it_ = notSetInfo_.find(tmp);
		}
		strcat(buf,it_->second.c_str());
		tree_->insert(node1,std::string(buf));
	}

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Window size value: %d",window);
	tree_->insert(root,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Checksum: %d",checksum);
	tree_->insert(root,std::string(buf));

	if(ProtocalManager::instance().isExisting(s_port)){
		AnalysePacket->nCurProt = s_port;
	}
	if(ProtocalManager::instance().isExisting(d_port)){
		AnalysePacket->nCurProt = d_port;
	}

	AnalysePacket->data_ += header_len;
	AnalysePacket->leftLength_ -= header_len; 

	if(AnalysePacket->leftLength_!= 0){//不只是ＴＣＰ首部

		if(ProtocalManager::instance().isExisting(s_port)){
			AnalysePacket->nCurProt = s_port;
		}
		if(ProtocalManager::instance().isExisting(d_port)){
			AnalysePacket->nCurProt = d_port;
		}
	}

}

void g_UdpAnalyse(PacketInfo* AnalysePacket){

	const unsigned char * pBuffer = AnalysePacket->data_;
	UDP_Hdr * Udp_Hdr = (PUDP_Hdr)pBuffer;

	AnalysePacket->columnInfo_.protocal_ = std::string(_T("udp"));

	AnalysePacket->data_ += 8;
	AnalysePacket->leftLength_ -= 8;

	char buf[MAX_PATH] = {0};
	RootedTree<std::string> *tree_ = new RootedTree<std::string>;
	AnalysePacket->detailInfo_.push_back(tree_);

	unsigned short s_port = ntohs(Udp_Hdr->Src_Port);
	unsigned short d_port = ntohs(Udp_Hdr->Des_Port);
	unsigned short len	  = ntohs(Udp_Hdr->Total_Length);
	unsigned short checksum = ntohs(Udp_Hdr->Packet_Check);

	sprintf(buf,"User Datagram Protocal,src port: %d,dest port: %d",s_port,d_port);
	HITEM root = tree_->insert(NULL,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Source port:%d,Destination port:%d",s_port,d_port);
	AnalysePacket->columnInfo_.info_ = std::string(buf);

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Source Port: %d",s_port);
	tree_->insert(root,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"Destination Port: %d",d_port);
	tree_->insert(root,std::string(buf));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"CheckSum: 0x%x",checksum);
	tree_->insert(root,std::string(buf));

	if(ProtocalManager::instance().isExisting(s_port)){
		AnalysePacket->nCurProt = s_port;
	}

	if(ProtocalManager::instance().isExisting(d_port)){
		AnalysePacket->nCurProt = d_port;
	}

	AnalysePacket->data_ += 8;
	AnalysePacket->leftLength_ -= 8; 
}
   

