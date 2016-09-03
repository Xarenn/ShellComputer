#include "Headers\ServerSocket.h"

ServerSocket::ServerSocket(){
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
		printf("Initialization error.\n");

	main = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (main == INVALID_SOCKET)
	{
		printf("Error creating socket: %ld\n", WSAGetLastError());
	}

	sockaddr_in service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(1337);

	if (bind(main, (SOCKADDR *)& service, sizeof(service)) == SOCKET_ERROR)
	{
		printf("bind() failed.\n");
		closesocket(main);
	}
}

ServerSocket::~ServerSocket() {
	closesocket(main);
}

SOCKET ServerSocket::accept_client() {
	SOCKET acceptSocket = SOCKET_ERROR;
	if (listen(main, 1) == SOCKET_ERROR) {
		printf("Error listening on socket.\n");
	}
	while (acceptSocket == SOCKET_ERROR) {
		acceptSocket = accept(main, NULL, NULL);
	}
	printf("Client joined\n");
	return acceptSocket;
}

std::string get_host_name() {
	char hostname[20];
	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
		std::cerr << "Error " << WSAGetLastError() <<
			" when getting local host name." << std::endl;
	}
	return hostname;
}

std::string get_address() {
	hostent *phe = gethostbyname(get_host_name().c_str());
	in_addr addr;
	if (phe == 0) {
		std::cerr << "Bad host lookup." << std::endl;
	}
	for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
		memcpy(&addr, phe->h_addr_list[i], sizeof(in_addr));
	}
	return inet_ntoa(addr);
}

void handle_new_connection(SOCKET sock) {
	int bytesRecv = SOCKET_ERROR;
	int bytesSent = SOCKET_ERROR;
	std::string server_header = "Hello im server. My IP: "+get_address();
	char recvbuf[4096];
	bytesSent = send(sock, server_header.c_str(), server_header.size(), 1);
	while (sock != SOCKET_ERROR) {
		bytesRecv = recv(sock, recvbuf, 4096, 0);
		if (bytesRecv < 0) {
			sock = SOCKET_ERROR;
		}
		printf("%s\n", recvbuf);
		if (recvbuf > 0) {
			bytesSent = send(sock, ".!.", 4, 1);
		}
		memset(recvbuf, 0, sizeof(recvbuf));
	}
}

int main(void) {
	ServerSocket sock;
	std::list<std::thread*> threads;
	for (;;) {
		SOCKET socket = sock.accept_client();
		if (socket == NULL) {
			std::cerr << "errorr";
			continue;
		}
		threads.push_back(new std::thread(handle_new_connection, socket));
	}
}


