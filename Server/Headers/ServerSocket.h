
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
#include <thread>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>
#include <direct.h>
#include <errno.h>
#include <sstream>

#define HEADER_SIZE 256
#define BUFFER_MAX 128

static std::string PREFIX_ERROR = "[ERROR] ";

static std::string NULL_COMMAND = (PREFIX_ERROR.append("Command is null\n"));
static std::string COMMAND_NOT_FOUND = (PREFIX_ERROR.append("Command not found. Use /cmd help\n"));
static std::string SOCKET_CREATING_ERROR = (PREFIX_ERROR.append("Error creating socket\n"));
static std::string BIND_FAILED = (PREFIX_ERROR.append("Bind() failed\n"));
static std::string LISTEN_FAILED = (PREFIX_ERROR.append("Error listening on socket\n"));
static std::string COMMAND_TOO_LONG = (PREFIX_ERROR.append("Command is too long\n"));
static std::string HELP = "Usage: shellcomputer.exe <ip> <port> \n Commands: /cmd chdir, /cmd help, /cmd exe, /cmd checkconn\n";
static std::string INITIALIZATION_ERROR = (PREFIX_ERROR.append("Initialization error.\n"));
static std::string COMMAND_EXECUTION_FAIL = (PREFIX_ERROR.append("Execution command fault\n"));
static std::string SOCKET_DISCONNECT = (PREFIX_ERROR.append("Client disconnected\n"));
static std::string COMMAND_ACCEPTED = ("Command accepted\n");
static std::string NULL_CLIENT = (PREFIX_ERROR.append("Client NULL\n"));
static std::string INVALID_COMMAND = (PREFIX_ERROR.append("Invalid command\n"));
static std::string DIRECTORY_NOT_FOUND = (PREFIX_ERROR.append("Directory not found \n"));

namespace Client {
	char header[HEADER_SIZE];
	static int online_clients = 0;
	std::vector<std::string> clients;
}

using Client::online_clients;
using Client::clients;

class ServerSocket {
	using Socket = SOCKET;
public:
	std::string parse_header(char* header);

	void change_dir(Socket sock, const char* dir);
	std::string command_exec(Socket sock, std::string& cmd);
	void exec_cmd(Socket sock, const char* cmd);
	void check_connections_cmd(Socket sock, std::vector<std::string>& clients);

	std::string parse_cmd(Socket sock, std::string& cmd);
	std::string check_cmd(const std::string& cmd);
	std::string find_cmd(const std::string& cmd);
	

	explicit ServerSocket();
	ServerSocket(const std::string ip, int port);
	ServerSocket(bool);
	Socket accept_client();
	void output(Socket sock, const std::string message);
	void socket_output(const std::string& output);
	void error_output(Socket sock, std::string& error);
	void error_output(std::string& error);
	~ServerSocket();
private:
	Socket main;
	WSADATA wsaData;
};