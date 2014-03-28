#include "Utility.h"


namespace Utility{

	
	bool g_isBroadcast(const unsigned char * mac){

		int i = 0;
		for (;i< 6; i++){
		if(mac[i]!= 0xff){
				break;
			}
		}
		return (i==6) ? true : false;
	}

	
	std::string g_getCodePoint(unsigned char dscp){

		char buf[10] = {0};
		unsigned char filter[6] = {0x80,0x40,0x20,0x10,0x08b,0x04};
	
		for (int i= 0; i< 4; i++){

			buf[i] = ((dscp&filter[i])==0) ? '0' : '1';
		}
		buf[4] = buf[5] = ' ';
		buf[6] = ((dscp&filter[5])==0) ? '0' : '1';
		buf[7] = ((dscp&filter[5])==0) ? '0' : '1';
		buf[8] = buf[9] = '.';

		return std::string(buf);
	}

	bool g_isFragment(unsigned short flag_offset){

		return ((flag_offset>>14)&0x0001)== 1 ? true : false;
	}


	unsigned short getIPCheckSum(unsigned char * header){
	
		return 0;
	}

	bool g_testTcpBit(unsigned short flag,TcpBit bit){

		switch(bit)
		{
		case URG:
			return ((flag>> 5 & 1) == 1) ? true : false;
		case ACK:
			return ((flag>>4 & 1) == 1) ? true : false;
		case PSH:
			return ((flag>>3 & 1)== 1) ? true : false;
		case RST:
			return ((flag>>2 & 1) == 1) ? true : false;
		case SYN:
			return ((flag>>1 & 1)== 1) ? true :false;
		case FIN:
			return ((flag & 1) == 1) ? true : false;
		}
	}

	std::string g_getTcpBitStr(unsigned short flag,TcpBit bit){

		char buf[16] = {'\0'};
		for (int i=0; i< 15; i++){
			buf[i]= '.';
		}
		buf[4] = buf[8] = ' ';
		if(g_testTcpBit(flag,bit)){ //±»ÖÃÎ»
			if(bit== URG){
				buf[7] = '1';
			}else if(bit== ACK){
				buf[8] = '1';
			}else if(bit== PSH){
				buf[10] = '1';
			}else if(bit== RST){
				buf[11] = '1';
			}else if(bit== SYN){
				buf[12]= '1';
			}else if(bit== FIN){
				buf[13]= '1';
			}
		}else{
			if(bit== URG){
				buf[7] = '0';
			}else if(bit== ACK){
				buf[8] = '0';
			}else if(bit== PSH){
				buf[10] = '0';
			}else if(bit== RST){
				buf[11] = '0';
			}else if(bit== SYN){
				buf[12]= '0';
			}else if(bit== FIN){
				buf[13]= '0';
			}
		}
		return std::string(buf);
	}

	bool g_getMacType(unsigned short flag){

		return flag>0x0600 ? true : false;
	}
}