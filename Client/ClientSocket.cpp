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
	if (connect(this->main_socket, (SOCKADDR *)& service, sizeof(service)) == SOCKET_ERROR)
	{
		printf("Failed to connect.\n");
		WSACleanup();
		exit(0);
	}
	send(this->main_socket, this->header.c_str(), header.size() + 1 != 128 ? 128 : header.size() + 1, 1);
}

ClientSocket::ClientSocket(std::string ip, int port) {
	this->ip = ip;
	this->port = port;

	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
		printf("Initialization error.\n");

	this->main_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->main_socket == INVALID_SOCKET)
	{
		printf("Error creating socket: %ld\n", WSAGetLastError());
	}
	std::string ip_s = get_address();
	std::string header = "Shell Client Joined ---- IP: " + ip_s;
	this->header.append(header);
}

ClientSocket::ClientSocket() {
	this->ip = "127.0.0.1";
	this->port = 1337;

	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
		printf("Initialization error.\n");

	this->main_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->main_socket == INVALID_SOCKET)
	{
		printf("Error creating socket: %ld\n", WSAGetLastError());
	}
	std::string ip_s = get_address();
	std::string name = get_host_name();
	std::string header = "Client Joined ----->\nIP: ";
	this->header = header.append(ip_s).append("\n").append("Name: ").append(name).append("\n");
}
void send_command(Socket sock) {
	std::string sendbuf;
	while (std::getline(std::cin, sendbuf)) {
		send(sock, sendbuf.c_str(), (strlen(sendbuf.c_str())) + 1, 1);
		if (sendbuf == "exit") {
			exit(1);
			sock = SOCKET_ERROR;
		}
		sendbuf.clear();
	}

}

void receive_info(Socket sock) {
	int recv_bytes = SOCKET_ERROR;
	char recvbuf[BUFFER_MAX];
	while (sock != SOCKET_ERROR) {
		recv_bytes = recv(sock, recvbuf, BUFFER_MAX, 0);
		if (recv_bytes < 0) {
			break;
		}
		printf("[Bytes]: %i, Text: %s\n", recv_bytes, recvbuf);
		memset(recvbuf, 0, sizeof(recvbuf));
	}
}

void ClientSocket::client_loop() {
	std::thread receive_thr(receive_info, this->main_socket);
	std::thread send_thr(send_command, this->main_socket);
	receive_thr.join();
	send_thr.join();

}