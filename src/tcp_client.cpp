#include "tcp_client.hpp"

TcpClient::TcpClient(int domain)
	: _domain(domain) {}

TcpClient::TcpClient(const TcpClient &client) {
	_domain = client._domain;
	_sockFd = client._sockFd;
}

TcpClient::~TcpClient() {
	close(_sockFd);
}

TcpClient TcpClient::operator=(TcpClient &client) {
	_domain = client._domain;
	_sockFd = client._sockFd;

	return *this;
}

int TcpClient::connServ(char *ip, int port) {
	_sockFd = socket(_domain, SOCK_STREAM, 0);
	if (_sockFd == -1)
		throw KitException("tcp_client.cpp 23", "socket() error");

	/* IPv4 和 IPv6 使用不同结构体创建 */
	if (_domain == PF_INET) {
		sockaddr_in servAddr;

		memset(&servAddr, 0, sizeof(servAddr));	 // sockaddr_in 结构体中有一个 zero 数组需要填充 0
		inet_pton(AF_INET, ip, &servAddr.sin_addr);
		servAddr.sin_port = htons(port);
		servAddr.sin_family = AF_INET;

		int connState = connect(_sockFd, (sockaddr *)&servAddr, sizeof(servAddr));
		if (connState == -1)
			throw KitException("tcp_client.cpp 36", "connect()");
	}
	else {
		sockaddr_in6 servAddr6;

		memset(&servAddr6, 0, sizeof(servAddr6));
		inet_pton(AF_INET6, ip, &servAddr6.sin6_addr);
		servAddr6.sin6_port = htons(port);
		servAddr6.sin6_family = AF_INET;

		int connState = connect(_sockFd, (sockaddr *)&servAddr6, sizeof(servAddr6));
		if (connState == -1)
			throw KitException("tcp_client.cpp 48", "connect()");
	}

	return _sockFd;
}

int TcpClient::sendToServ(std::string str) {
	const char *data = str.c_str();
	int writeLen = write(_sockFd, data, DATA_SIZE);

	if (writeLen == -1)
		throw KitException("tcp_client.cpp 58", "write() error");
	else
		return writeLen;
}

std::string TcpClient::recvFromServ() {
	char *data = new char[DATA_SIZE];
	int readLen = read(_sockFd, data, sizeof(data));
	std::string res = data;

	if (readLen == -1)
		throw KitException("tcp_client.cpp 68", "read() error");
	else
		res = data;

	delete data;

	return res;
}