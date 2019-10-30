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
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;  //定义一个地址结构体变量

	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  //设定服务器地址，因为是在本机上，所以用本地回路地址
	addrSrv.sin_family = AF_INET;  
	addrSrv.sin_port = htons(6000);  //设定发送目的端口

	connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	//接收服务器发送的数据并打印
	char recvBuf[100];
	recv(sockClient, recvBuf, 100, 0);  //接收数据
	printf("%s\n", recvBuf);  //打印接收到的数据

	//向服务器发送数据
	send(sockClient, "The request is from huahua", strlen("The request is from huahua") + 1, 0);

	closesocket(sockClient);
	WSACleanup();  //终止对套接字库的使用

	//sockConn是服务器用于连接的套接口  sockSrv是处于监听状态的套接口  sockClient是客户端用于连接的套接口
}