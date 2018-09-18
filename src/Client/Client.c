#include <winsock2.h>
// #include <ws2tcpip.h> // InetPton
#include <tchar.h>    // _tprintf
#include <stdlib.h>   // system

#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")
// #pragma comment (lib, "AdvApi32.lib")

#define MESSAGE_LENGTH 128
#define SERVER_IP      "127.0.0.1" // "37.190.32.12" "127.0.0.1" "192.168.1.65"
#define SERVER_PORT    20000

int __cdecl main(int argc, char **argv)
{
	WSADATA wsaData; 
	INT     iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		_tprintf(TEXT("WSAStartup failed with error: %d\n"), iResult);

		system("pause");
		return 1;
	}
	_tprintf(TEXT("Startup finished\n"));

	SOCKET connect_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect_sock == INVALID_SOCKET)
	{
		_tprintf(TEXT("socket failed with error: %ld\n"), WSAGetLastError());

		WSACleanup();

		system("pause");
		return 1;
	}
	_tprintf(TEXT("Connecting...\n"));

	SOCKADDR_IN connect_sockaddr;
	ZeroMemory(&connect_sockaddr, sizeof(SOCKADDR_IN));
	connect_sockaddr.sin_family      = AF_INET;
	connect_sockaddr.sin_addr.s_addr = inet_addr(argc > 1 ? argv[1] : SERVER_IP); // _WINSOCK_DEPRECATED_NO_WARNINGS
	connect_sockaddr.sin_port        = htons(SERVER_PORT);
	//InetPton(AF_INET, TEXT("127.0.0.1"), &connect_sockaddr.sin_addr.s_addr); 
	if (connect(connect_sock, (LPSOCKADDR)&connect_sockaddr, sizeof(struct sockaddr)) == SOCKET_ERROR)
	{
		_tprintf(TEXT("connect failed with error: %ld\n"), WSAGetLastError());

		closesocket(connect_sock);
		WSACleanup();

		system("pause");
		return 1;
	}
	_tprintf(TEXT("Connected\n"));

	if (argc > 2) // _CRT_SECURE_NO_WARNINGS
	{
		char message[MESSAGE_LENGTH] = { 0 };
		strcpy(message, argv[2]);
		strcat(strcat(message, " "), argv[3]);
		INT bytes = send(connect_sock, message, MESSAGE_LENGTH, 0);
		_tprintf(TEXT("%d bytes sent1\n"), bytes);
	}
	else 
	{
		char message[MESSAGE_LENGTH] = "05 netstat /release";
		INT bytes = send(connect_sock, message, MESSAGE_LENGTH, 0);
		_tprintf(TEXT("%d bytes sent2\n"), bytes);
	}

	char result[MESSAGE_LENGTH] = { 0 };
	recv(connect_sock, result, MESSAGE_LENGTH, 0);
	_tprintf(TEXT("Result: %hs\n"), result);

	closesocket(connect_sock);
	WSACleanup();

	system("pause");
	return 0;
}
