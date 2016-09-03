#include "ClientSocket.h"

std::string ClientSocket::get_host_name() {
	char hostname[20];
	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
		std::cerr << "Error " << WSAGetLastError() <<
			" when getting local host name." << std::endl;
	}
	return hostname;
}

std::string ClientSocket::get_address() {
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

void ClientSocket::connect_server() {
	sockaddr_in service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr(ip.c_str());
	service.sin_port = htons(port);
	if (connect(main, (SOCKADDR *)& service, sizeof(service)) == SOCKET_ERROR)
	{
		printf("Failed to connect.\n");
		WSACleanup();
		exit(0);
	}
}

ClientSocket::ClientSocket(std::string ip, int port){
	this->ip = ip;
	this->port = port;

	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
		printf("Initialization error.\n");

	main = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (main == INVALID_SOCKET)
	{
		printf("Error creating socket: %ld\n", WSAGetLastError());
	}
	std::string ip_s = get_address();
	std::string header = "Shell ---- IP: " + ip_s;
	this->header.append(header);
}

ClientSocket::ClientSocket() {
	this->ip = "127.0.0.1";
	this->port = 1337;

	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
		printf("Initialization error.\n");

	main = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (main == INVALID_SOCKET)
	{
		printf("Error creating socket: %ld\n", WSAGetLastError());
	}
	std::string ip_s = get_address();
	std::string header = "Shell ---- IP: " + ip_s;
	this->header.append(header);
}

void ClientSocket::send_command() {
	char recvbuf[1024];
	std::string sendbuf;
	recv(main, recvbuf, 1024, 0);
	printf("%s\n", recvbuf);
	send(main, this->header.c_str(), sizeof(this->header), 1);
	while (std::getline(std::cin, sendbuf)) {
		send(main, sendbuf.c_str(), sizeof(sendbuf), 1);
		recv(main, recvbuf, 1024, 0);
		printf("%s\n", recvbuf);
		sendbuf.clear();
		memset(recvbuf, 0, strlen(recvbuf));
	}
	
}
