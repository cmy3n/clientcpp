#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>

using namespace std;

int main()
{
	setlocale(LC_ALL, "RUS");

	// Создаем переменные
	WSADATA wsadata;
	char recvBuffer[512];
	ADDRINFO hints;
	ADDRINFO* addrresult;
	SOCKET ConnectSocket = INVALID_SOCKET;
	const char* sendBuffer = "Hello from  client";
	const char* sendBuffer2 = "сообщение 2";

	// Инициализация библиотеки Winsock
	int result = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (result != 0)
	{
		cout << "WSAStartup failed\n";
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Получение информации о сетевом адресе сервера
	result = getaddrinfo("localhost", "1161", &hints, &addrresult);
	if (result != 0)
	{
		cout << "Getaddrinfo error\n";
		WSACleanup();
		return 1;
	}

	// Создание сокета-клиента
	ConnectSocket = socket(addrresult->ai_family, addrresult->ai_socktype, addrresult->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET)
	{
		cout << "Socket Error\n";
		freeaddrinfo(addrresult);
		WSACleanup();
		return 1;
	}

	// Подключение сокета-клиента к сокету-серверу
	result = connect(ConnectSocket, addrresult->ai_addr, (int)addrresult->ai_addrlen);
	if (result == SOCKET_ERROR)
	{
		cout << "Connection failed\n"; 
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
		freeaddrinfo(addrresult);
		WSACleanup();
		return 1;
	}

	// Отправка первого сообщения на сервер
	result = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0);
	if (result == SOCKET_ERROR)
	{
		cout << "Send error" << result << endl;
		freeaddrinfo(addrresult);
		WSACleanup();
		return 1;
	}
	cout << "Send " << result << endl;


	// Отправка второго сообщения на сервер
	result = send(ConnectSocket, sendBuffer2, (int)strlen(sendBuffer2), 0);
	if (result == SOCKET_ERROR)
	{
		cout << "Send error" << result << endl;
		freeaddrinfo(addrresult);
		WSACleanup();
		return 1;
	}
	cout << "Send " << result << endl;


	// Завершение передачи данных на сервер
	result = shutdown(ConnectSocket, SD_SEND);
	if (result == SOCKET_ERROR)
	{
		cout << "Shutdown error " << result << endl;
		freeaddrinfo(addrresult);
		WSACleanup();
		return 1;
	}

	// Цикл для приема данных от сервера
	do
	{
		ZeroMemory(recvBuffer, sizeof(recvBuffer), 0);
		result = recv(ConnectSocket, recvBuffer, sizeof(recvBuffer), 0);
		if (result > 0)
		{
			cout << "Recieved: " << result << " bytes\n";
			cout << "Recieved data: " << recvBuffer << endl;
		}
		else if (result == 0)
		{
			cout << "Connection closed\n";
		}
		else
		{
			cout << "Received error\n";
		}
		cout << endl;
	} while (result > 0);

	freeaddrinfo(addrresult);
	WSACleanup();
}