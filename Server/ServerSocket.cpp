#include "Headers\ServerSocket.h"

static std::string PREFIX_ERROR = "[ERROR] ";

static std::string NULL_COMMAND = (PREFIX_ERROR.append("Command is null\n"));
static std::string COMMAND_NOT_FOUND = (PREFIX_ERROR.append("Command not found. Use /cmd help\n"));
static std::string SOCKET_CREATING_ERROR = (PREFIX_ERROR.append("Error creating socket\n"));
static std::string BIND_FAILED = (PREFIX_ERROR.append("Bind() failed\n"));
static std::string LISTEN_FAILED = (PREFIX_ERROR.append("Error listening on socket\n"));
static std::string COMMAND_TOO_LONG = (PREFIX_ERROR.append("Command is too long\n"));
static std::string HELP = "Usage: shellcomputer.exe <ip> <port> \n Commands: /cmd help, /cmd exe <text>, /cmd list, /cmd checkconn\n";
static std::string INITIALIZATION_ERROR = (PREFIX_ERROR.append("Initialization error.\n"));
static std::string COMMAND_EXECUTION_FAIL = (PREFIX_ERROR.append("Execution command fault\n"));
static std::string SOCKET_DISCONNECT = (PREFIX_ERROR.append("Client disconnected\n"));
static std::string COMMAND_ACCEPTED = ("Command accepted\n");
static std::string NULL_CLIENT = (PREFIX_ERROR.append("Client NULL"));

ServerSocket::ServerSocket(bool) {}

ServerSocket::ServerSocket(std::string ip, int port) {
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
		printf("%s", INITIALIZATION_ERROR);

	this->main = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->main == INVALID_SOCKET)
		printf("%s: %ld", SOCKET_CREATING_ERROR, WSAGetLastError());

	sockaddr_in service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr(ip.c_str());
	service.sin_port = htons(port);

	if (bind(this->main, (SOCKADDR *)& service, sizeof(service)) == SOCKET_ERROR) {
		printf("%s", BIND_FAILED);
		closesocket(this->main);
	}
}

ServerSocket::ServerSocket() {
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
		printf("%s", INITIALIZATION_ERROR);

	this->main = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->main == INVALID_SOCKET)
		printf("%s: %ld", SOCKET_CREATING_ERROR, WSAGetLastError());

	sockaddr_in service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(1337);

	if (bind(this->main, (SOCKADDR *)& service, sizeof(service)) == SOCKET_ERROR) {
		printf("%s", BIND_FAILED);
		closesocket(this->main);
	}
}

ServerSocket::~ServerSocket() {
	closesocket(this->main);
}

SOCKET ServerSocket::accept_client() {
	SOCKET acceptSocket = SOCKET_ERROR;
	if (listen(this->main, 1) == SOCKET_ERROR) {
		printf("%s.", LISTEN_FAILED);
	}
	while (acceptSocket == SOCKET_ERROR) {
		acceptSocket = accept(this->main, NULL, NULL);
	}
	printf("Client joined\n");
	return acceptSocket;
}

std::string get_host_name() {
	char hostname[20];
	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
		std::cerr << "Error " << WSAGetLastError() <<
			" when getting local host name." << std::endl;
	}
	return hostname;
}

std::string get_address() {
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

void ServerSocket::exec_cmd(const std::string&& cmd) {
#ifdef __linux__
	FILE* in;
	char buff[512];

	if (!(in = popen(cmd.c_str(), "r"))) {
		error_output(COMMAND_EXECUTION_FAIL);
	}
	while (fgets(buff, sizeof(buff), in) != NULL) {
		send(sock, buff, strlen(buff), 1);
	}
	pclose(in);
#endif

	// EXECUTION CMD -> WINDOWS #TODO
}

void ServerSocket::list_cmd() {
	// SCAN PORTS 1337 or use FILES;)
}

void ServerSocket::check_connections_cmd(Socket sock, std::vector<std::string>& clients) {
	std::string connection_list = "\nConnection list: \n";
	for (auto const& client : clients) {
		connection_list.append(client).append("\n");
	}

	send(sock, connection_list.c_str(), connection_list.size() + 1, 1);
}

void ServerSocket::socket_output(const std::string& output) {
	std::string header_message = Client::header;
	std::string tag = "IP:";
	auto it = std::find_first_of(header_message.begin(), header_message.end(), tag.begin(), tag.end());
	if (it == header_message.end()) {
		return;
	}
	int pos = std::distance(header_message.begin(), it);
	std::cout << "CLIENT IP: " << header_message.substr(pos + tag.size()) << std::endl;
	std::cout << output << std::endl;
}

void ServerSocket::error_output(Socket sock, std::string& error) {
	send(sock, error.c_str(), error.size() + 1, 1);
	return;
}

void ServerSocket::error_output(std::string& error) {
	std::cout << error;
	return;
}

std::string ServerSocket::parse_cmd(Socket sock, std::string & cmd) {
	if (cmd == COMMAND_NOT_FOUND || cmd == COMMAND_TOO_LONG) {
		error_output(sock, NULL_COMMAND);
	}
	cmd.erase(remove_if(cmd.begin(), cmd.end(), isspace), cmd.end());

	if (cmd.empty()) {
		error_output(sock, NULL_COMMAND);
	}
	return cmd;

}

std::string ServerSocket::check_cmd(const std::string& cmd) {
	if (cmd.size() == 0 || cmd.size() < 4) {
		error_output(COMMAND_NOT_FOUND);
		return COMMAND_NOT_FOUND;
	}
	if (cmd.size() >= BUFFER_MAX) {
		error_output(COMMAND_TOO_LONG);
		return COMMAND_TOO_LONG;
	}
	return cmd;
}

std::string ServerSocket::find_cmd(const std::string& cmd) {
	std::string tag = "/cmd";
	auto it = std::find_first_of(cmd.begin(), cmd.end(), tag.begin(), tag.end());
	if (it == cmd.end()) {
		return NULL_COMMAND;
	}
	else {
		int pos = std::distance(cmd.begin(), it);
		if (cmd.size() > tag.size() && cmd[tag.size()] == 0x20) {
			return cmd.substr(pos + tag.size());
		}
		else {
			return COMMAND_NOT_FOUND;
		}
	}
}

std::string ServerSocket::parse_header(char* header) {
	std::string client_header = header;
	std::string::iterator end = client_header.end();
	std::string ip_tag = "IP:";
	std::string client;
	auto ip = std::find_first_of(client_header.begin(), end, ip_tag.begin(), ip_tag.end());
	if (ip == end) {
		return NULL_CLIENT;
	}
	client = client_header.substr(std::distance(client_header.begin(), ip));
	return client;
}

std::string ServerSocket::command_exec(Socket client, std::string& cmd) {
	const std::string help = "help";
	const std::string exec = "exe";
	const std::string list = "list";
	const std::string check_connections = "checkconn";
	const std::string EXIT = "exit";
	if (cmd.empty()) {
		error_output(client, COMMAND_NOT_FOUND);
	}
	std::string _cmd = check_cmd(cmd);
	if (_cmd == EXIT) {
		return SOCKET_DISCONNECT;
	}
	if (_cmd != COMMAND_NOT_FOUND || _cmd != COMMAND_TOO_LONG) {
		_cmd = find_cmd(_cmd);
		parse_cmd(client, _cmd);
		if (_cmd == help) {
			error_output(client, HELP);
		}else if (_cmd == list) {
			list_cmd();
		}else if (_cmd == check_connections) {
			check_connections_cmd(client, Client::clients);
		}else {
			error_output(client, HELP);
		}
	}
	else {
		error_output(client, COMMAND_NOT_FOUND);
	}
	return COMMAND_ACCEPTED;
}

void handle_new_connection(SOCKET sock) {
	ServerSocket s(true);
	int header_recv = SOCKET_ERROR;
	header_recv = recv(sock, Client::header, HEADER_BUFFER, 0);
	int bytes_recv = SOCKET_ERROR;
	int bytes_sent = SOCKET_ERROR;
	std::string server_header = "Hello im server. My IP: " + get_address();
	bytes_sent = send(sock, server_header.c_str(), server_header.size(), 1);
	char buffer[BUFFER_MAX];
	buffer[BUFFER_MAX - 1] = '\0';
	memset(buffer, 0, sizeof(buffer));
	std::cout << Client::header << std::endl;
	clients.push_back(s.parse_header(Client::header));
	memset(Client::header, 0, strlen(Client::header));
	while (sock != SOCKET_ERROR) {
			bytes_recv = recv(sock, buffer, BUFFER_MAX, 0);
			if (bytes_recv < 0) {
				break;
			}
			if (bytes_recv >= BUFFER_MAX) {
				s.error_output(sock, COMMAND_TOO_LONG);
				memset(buffer, 0, strlen(buffer));
				continue;
			}
			if (s.command_exec(sock, (std::string)buffer) == COMMAND_ACCEPTED) {
				memset(buffer, 0, strlen(buffer));
				continue;
			}
			else {
				--online_clients;
				s.socket_output(SOCKET_DISCONNECT);
				sock = SOCKET_ERROR;
			}
			/*std::cout << bytes_recv << std::endl;
			buffer[bytes_recv] = '\0';
			printf("%s\n", buffer);*/
	}
}

int main(void) {
	ServerSocket sock;
	std::list<std::thread*> threads;
	for (;;) {
		SOCKET socket = sock.accept_client();
		if (socket == NULL) {
			std::cerr << "errorr";
			continue;
		}
		++online_clients;
		threads.push_back(new std::thread(handle_new_connection, socket));
	}
}


