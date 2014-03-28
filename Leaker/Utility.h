/*

   ���ļ�����һЩ�����Ժ���������Э�����
   by zhucong 2014/03/15

*/

#ifndef LEAKER_UTILITY_H_
#define LEKAER_UTILITY_H_

#include <string>

namespace Utility{

	//Tcp�ײ��е���Ԫ����
	enum TcpBit
	{
		URG = 0,
		ACK,
		PSH,
		RST,
		SYN,
		FIN
	};

	//�ж�һ���ͣ��õ�ַ�Ƿ�Ϊ�㲥��
	bool g_isBroadcast(const unsigned char * mac);

	//���ڣɣ�Э������е�DSCP
	//ȡ���ַ������磺"������������������"����ʽ
	std::string g_getCodePoint(unsigned char dscp);


	//�ж�һ���ɣа��Ƿ���Խ��з�Ƭ
	//�������ɣ��ײ���"��־��ƫ��"�ֶ�
	//����ֵ��true:�����Է�Ƭ(�ģ�)��false:�ܷ�Ƭ
	bool g_isFragment(unsigned short flag_offset);

	//����ɣ��ײ������
	unsigned short getIPCheckSum(unsigned char * header);

	//�жϣԣã��ײ��е���Ԫ������λ���
	//���������flag,��λΪ����λ
	//���أ�true:����λ��false:δ
	bool g_testTcpBit(unsigned short flag,TcpBit bit);

	//������".... ..0. .... "����ʽ�����ԣã��ײ�����λ���
	std::string g_getTcpBitStr(unsigned short flag,TcpBit bit);

	//�ú����ж�MAC֡���� 802.3 or DIX Ethernet V2
	//����ֵ��true:Ethernet II,false:IEEE 802.3
	bool g_getMacType(unsigned short flag);

}



#endif