#include <WinSock2.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

using Socket = SOCKET;

class ClientSocket {
public:
	ClientSocket();
	ClientSocket(std::string ip, int port);
	void send_command();
	void connect_server();
	void receive_info();
	void client_loop();
	std::string get_host_name();
	std::string get_address();
private:
	int port;
	std::string ip;
	WSADATA wsaData;
	Socket main;
	std::string header;
};