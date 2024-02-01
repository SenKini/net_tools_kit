#include "tcp_server.hpp"

TcpServer::TcpServer(int domain)
	: _domain(domain) {}

TcpServer::TcpServer(const TcpServer &server) {
	_servSockFd = server._servSockFd;
	_cliSockFd = server._cliSockFd;
	_cliAddr = server._cliAddr;
	_cliAddr6 = server._cliAddr6;
	_domain = server._domain;
}

TcpServer::~TcpServer() {
	close(_servSockFd);
	close(_cliSockFd);
}

TcpServer TcpServer::operator=(TcpServer &server) {
	_servSockFd = server._servSockFd;
	_cliSockFd = server._cliSockFd;
	_cliAddr = server._cliAddr;
	_cliAddr6 = server._cliAddr6;
	_domain = server._domain;

	return *this;
}

int TcpServer::creatServ(char *ip, int port, int backlog) {
	_servSockFd = socket(_domain, SOCK_STREAM, 0);

	if (_servSockFd == -1)
		throw KitException("tcp_server.cpp 30", "socket() error");

	/* IPv4 和 IPv6 使用不同结构体创建 */
	if (_domain == PF_INET) {
		sockaddr_in servAddr;

		memset(&servAddr, 0, sizeof(servAddr));	 // sockaddr_in 结构体中有一个 zero 数组需要填充 0
		inet_pton(AF_INET, ip, &servAddr.sin_addr);
		servAddr.sin_port = htons(port);
		servAddr.sin_family = AF_INET;

		int bindState = bind(_servSockFd, (sockaddr *)&servAddr, sizeof(servAddr));
		if (bindState == -1)
			throw KitException("tcp_server.cpp 44", "bind() error");

		int listenState = listen(_servSockFd, backlog);
		if (listenState == -1)
			throw KitException("tcp_server.cpp 48", "listen() error");
	}
	else {
		sockaddr_in6 servAddr6;

		memset(&servAddr6, 0, sizeof(servAddr6));
		inet_pton(AF_INET6, ip, &servAddr6.sin6_addr);
		servAddr6.sin6_port = htons(port);
		servAddr6.sin6_family = AF_INET6;

		int bindState = bind(_servSockFd, (sockaddr *)&servAddr6, sizeof(servAddr6));
		if (bindState == -1)
			throw KitException("tcp_server.cpp 60", "bind() error");

		int listenState = listen(_servSockFd, backlog);
		if (listenState == -1)
			throw KitException("tcp_server.cpp 64", "listen() error");
	}

	return _servSockFd;
}

int TcpServer::acceptCli() {
	if (_domain == PF_INET) {
		socklen_t cliAddrSize;
		cliAddrSize = sizeof(_cliAddr);
		_cliSockFd = accept(_servSockFd, (sockaddr *)&_cliAddr, &cliAddrSize);
	}
	else {
		socklen_t cliAddr6Size = sizeof(_cliAddr6);
		_cliSockFd = accept(_servSockFd, (sockaddr *)&_cliAddr6, &cliAddr6Size);
	}

	return _cliSockFd;
}

int TcpServer::sendToCli(std::string str) {
	const char *data = str.c_str();
	int writeLen = write(_cliSockFd, data, sizeof(data));

	if (writeLen == -1)
		throw KitException("tcp_server.cpp 88", "write() error");
	else
		return writeLen;
}

std::string TcpServer::recvFromCli() {
	char *data = new char[DATA_SIZE];
	int readLen = read(_cliSockFd, data, DATA_SIZE);
	std::string res;

	if (readLen == -1)
		throw KitException("tcp_server.cpp 98", "read() error");
	else
		res = (char *)data;

	free(data);

	return res;
}