/*
 #####                                  #####
#     # #    # ###### #      #         #     #  ####  #    # #####  #    # ##### ###### #####
#       #    # #      #      #         #       #    # ##  ## #    # #    #   #   #      #    #
 #####  ###### #####  #      #         #       #    # # ## # #    # #    #   #   #####  #    #
      # #    # #      #      #         #       #    # #    # #####  #    #   #   #      #####
#     # #    # #      #      #         #     # #    # #    # #      #    #   #   #      #   #
 #####  #    # ###### ###### ######     #####   ####  #    # #       ####    #   ###### #    #
*/
#include <WinSock2.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

using Socket = SOCKET;

class ClientSocket {

#define BUFFER_MAX 128

public:
	ClientSocket();
	ClientSocket(std::string ip, int port);
	void connect_server();
	void client_loop();
	std::string get_host_name();
	std::string get_address();
private:
	std::string header;
	int port;
	std::string ip;
	WSADATA wsaData;
	Socket main_socket;
};