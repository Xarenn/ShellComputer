
/*
	 #####                                  #####                                                  
	#     # #    # ###### #      #         #     #  ####  #    # #####  #    # ##### ###### #####  
	#       #    # #      #      #         #       #    # ##  ## #    # #    #   #   #      #    # 
	 #####  ###### #####  #      #         #       #    # # ## # #    # #    #   #   #####  #    # 
	      # #    # #      #      #         #       #    # #    # #####  #    #   #   #      #####  
	#     # #    # #      #      #         #     # #    # #    # #      #    #   #   #      #   #  
	 #####  #    # ###### ###### ######     #####   ####  #    # #       ####    #   ###### #    # 
*/

#pragma once
#include <WinSock2.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <thread>
#include <list>

#define HEADER_BUFFER 256
#define BUFFER_MAX 128

char header[HEADER_BUFFER];

class ServerSocket {
	using Socket = SOCKET;
public:
	std::string command_exec(Socket sock, std::string& cmd);
	void exec_cmd(const std::string&& cmd);
	void list_cmd();
	void check_connections_cmd();

	std::string parse_cmd(Socket sock, std::string & cmd);
	std::string check_cmd(const std::string& cmd);
	std::string find_cmd(const std::string& cmd);


	ServerSocket(const std::string ip, int port);
	ServerSocket(bool);
	ServerSocket();
	Socket accept_client();
	void socket_output(const std::string& output);
	void error_output(Socket sock, std::string& error);
	void error_output(std::string& error);
	~ServerSocket();
private:
	Socket main;
	WSADATA wsaData;
};