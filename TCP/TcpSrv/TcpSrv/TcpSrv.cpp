#include <Winsock2.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")

void main(){
	WORD wVersionRequested;  //WORD表示2byte的无符号整数，表示范围0~65535
	WSADATA wsaData;  //WSADATA这个结构被用来存储被WSAStartup函数调用后返回的Windows Sockets数据。它包含Winsock.dll执行的数据。 https://baike.baidu.com/item/WSADATA/3031763?fr=aladdin
	int err;

	wVersionRequested = MAKEWORD(1, 1);  //MAKEWORD是一个宏，这里定义了socket版本号1.1

	err = WSAStartup(wVersionRequested, &wsaData);  //允许应用程序或DLL指明Windows Sockets API的版本号及获得特定Windows Sockets实现的细节。应用程序或DLL只能在一次成功的WSAStartup()调用之后才能调用进一步的Windows Sockets API函数。
	if (err != 0)
	{
		return;
	}
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)  //如果不是1.1
	{
		WSACleanup();  //终止winsock库使用
		return;
	}
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);
							/*创建一个套接口（）。
							int socket( int af, int type, int protocol);
							af：一个地址描述。目前仅支持AF_INET格式，也就是说ARPA Internet地址格式。
							type：指定socket类型。新套接口的类型描述类型，如TCP（SOCK_STREAM）和UDP（SOCK_DGRAM）。常用的socket类型有，SOCK_STREAM、SOCK_DGRAM、SOCK_RAW、SOCK_PACKET、SOCK_SEQPACKET等等。
							protocol：顾名思义，就是指定协议。套接口所用的协议。如调用者不想指定，可用0。常用的协议有，IPPROTO_TCP、IPPROTO_UDP、IPPROTO_STCP、IPPROTO_TIPC等，它们分别对应TCP传输协议、UDP传输协议、STCP传输协议、TIPC传输协议。*/
	SOCKADDR_IN addrSrv;
							/*下边三者都是表示网络地址的结构体
							sockaddr，sockaddr_in，in_addr
							区别：
							sockaddr是通用的socket地址结构体
							sockaddr_in是Internet 和socket通用的地址结构体
							in_addr就是32位IP地址
							inet_addr()是将一个点分制的IP地址(如192.168.0.1)转换为上述结构中需要的32位IP地址(0xC0A80001)*/

	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);  //htonl ， htons从host字节序到TCOP/IP字节序
	addrSrv.sin_family = AF_INET;  //sin_family是地址家族，一般都是“AF_xxx”的形式。通常大多用的是都是AF_INET,代表TCP/IP协议族。
	addrSrv.sin_port = htons(6000);  //设定等待端口，需使用1024以上的端口号  1024以下系统指定

	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));  //绑定端口

	listen(sockSrv, 5);  //监听，最多处理5个请求

	SOCKADDR_IN addrClient;  //定义一个地址结构体变量
	int len = sizeof(SOCKADDR);
	while (1)
	{
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);  //sockConn是服务器用于连接的套接字  sockSrv是处于监听状态的套接字  sockClient是客户端用于连接的套接字
		
		//向客户端发送数据
		char sendBuf[100];
		//sprintf_s将一个数据格式化到Buf中
		sprintf_s(sendBuf, "Welcome %s to my world", inet_ntoa(addrClient.sin_addr));  //inet_ntoa返回点分十进制的IP地址字符串
		send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);

		//接收客户端数据并打印
		char recvBuf[100];
		recv(sockConn, recvBuf, 100, 0);
		printf("%s\n", recvBuf);

		closesocket(sockConn);  //由于这里一直等待，就没写关闭监听和cleanup函数终止winsock库的使用而直接关闭
	}
}