#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996)

#include <string>
#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>

class ClientSocket {
private:
	WSADATA WSAData;
	SOCKET server;
	SOCKADDR_IN addr;
	char buffer[32];

public:
	ClientSocket() { }

	void connecting(char* ipServer, int portServer) {
		std::cout << "\nConnected on servidor IP: " << '[' << ipServer << ']';
		WSAStartup(MAKEWORD(2, 0), &WSAData);
		this->server = socket(AF_INET, SOCK_STREAM, 0);
		this->addr.sin_addr.s_addr = inet_addr(ipServer);
		this->addr.sin_family = AF_INET;
		this->addr.sin_port = htons(portServer);
		connect(this->server, (SOCKADDR*)& this->addr, sizeof(this->addr));
		std::cout << "\n\nConnected on servidor!\n\n";
	}

	void sendFrame(char* data) {
		std::cout << "\nSend frame: " << data << "\n";
		send(this->server, data, sizeof(this->buffer), 0);
		memset(this->buffer, 0, sizeof(this->buffer));
	}

	void closeSocket() {
		closesocket(this->server);
		WSACleanup();
		std::cout << "Socket closed.\n\n";
	}
};
