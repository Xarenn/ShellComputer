#include "ClientSocket.h"


int main(int argc, char** argv) {


                // argv for socket
	ClientSocket socket("127.0.0.1",1337);
	socket.connect_server();
	socket.send_command();
	return 0;
}
