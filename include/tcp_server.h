#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include "err_code.h"

namespace DevKit {
class TcpServer {
private:
	const int _BUF_SIZE;
	int _servSockFd;
	sockaddr_in _cliAddr;
	sockaddr_in6 _cliAddr6;
	int _domain;

public:
	TcpServer(int d);
	TcpServer(const TcpServer &rhs);
	~TcpServer();
	TcpServer operator=(TcpServer &) = delete;

	ErrCode creatServ(const char *ip, const int port, int backlog, int &servSock);	// 创建服务器套接字
	ErrCode acceptCli(int &cliSock);												// 接收客户端
	ErrCode sendToCli(int cliSock, std::string str);								// 发送信息
	ErrCode recvFromCli(int cliSock, std::string &str);								// 接收信息
};
}  // namespace DevKit
