#pragma once
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <thread>
#include <list>

class ServerSocket {
public:
	ServerSocket();
	SOCKET accept_client();
	~ServerSocket();
private:
	SOCKET main;
	WSADATA wsaData;
	std::string shell_cmd;
};