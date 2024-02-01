#include "tcp_client.hpp"

int main() {
	auto tc = new TcpClient(PF_INET);
	std::string mess = "hello world";
	tc->connServ("127.0.0.1", 8080);
	tc->sendToServ(mess);

	return 0;
}