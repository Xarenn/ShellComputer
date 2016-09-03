
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

class ServerSocket {
public:
	std::string find_cmd(std::string& recvbuff);
	void parse_cmd(std::string& cmd);
	

	void exec_cmd(std::string cmd);
	void list_cmd();
	void check_connections_cmd();

	ServerSocket();
	SOCKET accept_client();
	~ServerSocket();
private:
	SOCKET main;
	WSADATA wsaData;
	std::string help;
};