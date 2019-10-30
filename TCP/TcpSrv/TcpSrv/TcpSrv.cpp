#include <Winsock2.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")

void main(){
	WORD wVersionRequested;  //WORD��ʾ2byte���޷�����������ʾ��Χ0~65535
	WSADATA wsaData;  //WSADATA����ṹ�������洢��WSAStartup�������ú󷵻ص�Windows Sockets���ݡ�������Winsock.dllִ�е����ݡ� https://baike.baidu.com/item/WSADATA/3031763?fr=aladdin
	int err;

	wVersionRequested = MAKEWORD(1, 1);  //MAKEWORD��һ���꣬���ﶨ����socket�汾��1.1

	err = WSAStartup(wVersionRequested, &wsaData);  //����Ӧ�ó����DLLָ��Windows Sockets API�İ汾�ż�����ض�Windows Socketsʵ�ֵ�ϸ�ڡ�Ӧ�ó����DLLֻ����һ�γɹ���WSAStartup()����֮����ܵ��ý�һ����Windows Sockets API������
	if (err != 0)
	{
		return;
	}
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)  //�������1.1
	{
		WSACleanup();  //��ֹwinsock��ʹ��
		return;
	}
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);
							/*����һ���׽ӿڣ�����
							int socket( int af, int type, int protocol);
							af��һ����ַ������Ŀǰ��֧��AF_INET��ʽ��Ҳ����˵ARPA Internet��ַ��ʽ��
							type��ָ��socket���͡����׽ӿڵ������������ͣ���TCP��SOCK_STREAM����UDP��SOCK_DGRAM�������õ�socket�����У�SOCK_STREAM��SOCK_DGRAM��SOCK_RAW��SOCK_PACKET��SOCK_SEQPACKET�ȵȡ�
							protocol������˼�壬����ָ��Э�顣�׽ӿ����õ�Э�顣������߲���ָ��������0�����õ�Э���У�IPPROTO_TCP��IPPROTO_UDP��IPPROTO_STCP��IPPROTO_TIPC�ȣ����Ƿֱ��ӦTCP����Э�顢UDP����Э�顢STCP����Э�顢TIPC����Э�顣*/
	SOCKADDR_IN addrSrv;
							/*�±����߶��Ǳ�ʾ�����ַ�Ľṹ��
							sockaddr��sockaddr_in��in_addr
							����
							sockaddr��ͨ�õ�socket��ַ�ṹ��
							sockaddr_in��Internet ��socketͨ�õĵ�ַ�ṹ��
							in_addr����32λIP��ַ
							inet_addr()�ǽ�һ������Ƶ�IP��ַ(��192.168.0.1)ת��Ϊ�����ṹ����Ҫ��32λIP��ַ(0xC0A80001)*/

	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);  //htonl �� htons��host�ֽ���TCOP/IP�ֽ���
	addrSrv.sin_family = AF_INET;  //sin_family�ǵ�ַ���壬һ�㶼�ǡ�AF_xxx������ʽ��ͨ������õ��Ƕ���AF_INET,����TCP/IPЭ���塣
	addrSrv.sin_port = htons(6000);  //�趨�ȴ��˿ڣ���ʹ��1024���ϵĶ˿ں�  1024����ϵͳָ��

	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));  //�󶨶˿�

	listen(sockSrv, 5);  //��������ദ��5������

	SOCKADDR_IN addrClient;  //����һ����ַ�ṹ�����
	int len = sizeof(SOCKADDR);
	while (1)
	{
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);  //sockConn�Ƿ������������ӵ��׽���  sockSrv�Ǵ��ڼ���״̬���׽���  sockClient�ǿͻ����������ӵ��׽���
		
		//��ͻ��˷�������
		char sendBuf[100];
		//sprintf_s��һ�����ݸ�ʽ����Buf��
		sprintf_s(sendBuf, "Welcome %s to my world", inet_ntoa(addrClient.sin_addr));  //inet_ntoa���ص��ʮ���Ƶ�IP��ַ�ַ���
		send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);

		//���տͻ������ݲ���ӡ
		char recvBuf[100];
		recv(sockConn, recvBuf, 100, 0);
		printf("%s\n", recvBuf);

		closesocket(sockConn);  //��������һֱ�ȴ�����ûд�رռ�����cleanup������ֹwinsock���ʹ�ö�ֱ�ӹر�
	}
}