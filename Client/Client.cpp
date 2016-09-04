#include "ClientSocket.h"


int main(int argc, char** argv) {


                // argv for socket
	ClientSocket socket;
        socket.connect_server();
	socket.client_loop();
	return 0;
}
