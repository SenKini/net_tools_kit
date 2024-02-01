#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include "kit_exception.hpp"

#define DATA_SIZE 2048

class TcpServer {
private:
	int _servSockFd;
	int _cliSockFd;
	sockaddr_in _cliAddr;
	sockaddr_in6 _cliAddr6;
	int _domain;

public:
	TcpServer(int domain);
	TcpServer(const TcpServer &server);
	~TcpServer();
	TcpServer operator=(TcpServer &server);

	int creatServ(char *ip, int port, int backlog);	 // 创建服务器套接字
	int acceptCli();								 // 接收客户端
	int sendToCli(std::string str);					 // 发送信息
	std::string recvFromCli();						 // 接收信息
};