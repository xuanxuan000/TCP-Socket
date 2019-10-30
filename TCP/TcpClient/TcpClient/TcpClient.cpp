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
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;  //����һ����ַ�ṹ�����

	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  //�趨��������ַ����Ϊ���ڱ����ϣ������ñ��ػ�·��ַ
	addrSrv.sin_family = AF_INET;  
	addrSrv.sin_port = htons(6000);  //�趨����Ŀ�Ķ˿�

	connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	//���շ��������͵����ݲ���ӡ
	char recvBuf[100];
	recv(sockClient, recvBuf, 100, 0);  //��������
	printf("%s\n", recvBuf);  //��ӡ���յ�������

	//���������������
	send(sockClient, "The request is from huahua", strlen("The request is from huahua") + 1, 0);

	closesocket(sockClient);
	WSACleanup();  //��ֹ���׽��ֿ��ʹ��

	//sockConn�Ƿ������������ӵ��׽ӿ�  sockSrv�Ǵ��ڼ���״̬���׽ӿ�  sockClient�ǿͻ����������ӵ��׽ӿ�
}