#include "tcp_server.h"

using namespace DevKit;

TcpServer::TcpServer(int d)
	: _domain(d), _BUF_SIZE(2048) {}

TcpServer::TcpServer(const TcpServer &rhs)
	: _BUF_SIZE(2048), _domain(rhs._domain), _servSockFd(rhs._servSockFd) {
	if (_domain == PF_INET)
		_cliAddr = rhs._cliAddr;
	else
		_cliAddr6 = rhs._cliAddr6;
}

TcpServer::~TcpServer() {
	close(_servSockFd);
}

ErrCode TcpServer::creatServ(const char *ip, const int port, int backlog, int &servSock) {
	_servSockFd = socket(_domain, SOCK_STREAM, 0);

	if (_servSockFd == -1)
		return SOCKET_ERROR;

	/* IPv4 和 IPv6 使用不同结构体创建 */
	int state;
	if (_domain == PF_INET) {
		sockaddr_in servAddr;

		memset(&servAddr, 0, sizeof(servAddr));		 // sockaddr_in 结构体中有一个 zero 数组需要填充 0
		inet_pton(AF_INET, ip, &servAddr.sin_addr);
		servAddr.sin_port = htons(port);
		servAddr.sin_family = AF_INET;

		state = bind(_servSockFd, (sockaddr *)&servAddr, sizeof(servAddr));
		if (state == -1)
			 return BIND_ERROR;

		state = listen(_servSockFd, backlog);
		if (state == -1)
			return LISTEN_ERROR;
	}
	else {
		sockaddr_in6 servAddr6;

		memset(&servAddr6, 0, sizeof(servAddr6));
		inet_pton(AF_INET6, ip, &servAddr6.sin6_addr);
		servAddr6.sin6_port = htons(port);
		servAddr6.sin6_family = AF_INET6;

		state = bind(_servSockFd, (sockaddr *)&servAddr6, sizeof(servAddr6));
		if (state == -1)
			return BIND_ERROR;

		state = listen(_servSockFd, backlog);
		if (state == -1)
			return LISTEN_ERROR;
	}

	servSock = _servSockFd;

	return OK;
}

ErrCode TcpServer::acceptCli(int &cliSock) {
	if (_domain == PF_INET) {
		socklen_t cliAddrSize;
		cliAddrSize = sizeof(_cliAddr);
		cliSock = accept(_servSockFd, (sockaddr *)&_cliAddr, &cliAddrSize);
	}
	else {
		socklen_t cliAddr6Size = sizeof(_cliAddr6);
		cliSock = accept(_servSockFd, (sockaddr *)&_cliAddr6, &cliAddr6Size);
	}

	return (cliSock != -1) ? OK : ACCEPT_ERROR;
}

ErrCode TcpServer::sendToCli(int cliSock, std::string str) {
	const char *buf = str.c_str();
	int writeSize = write(cliSock, buf, sizeof(buf));

	return (writeSize != -1) ? OK : WRITE_ERROR;
}

ErrCode TcpServer::recvFromCli(int cliSock, std::string &str) {
	char buf[_BUF_SIZE];
	int readSize;

	do {
		readSize = read(cliSock, buf, sizeof(buf));
		if (readSize > 0)
			str.append(buf, readSize);
		else if (readSize < 0)
			return READ_ERROR;
	} while (readSize > 0);

	return OK;
}