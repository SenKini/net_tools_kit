#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <exception>
#include <cstring>
#include "kit_exception.hpp"

#define DATA_SIZE 2048

class TcpClient {
private:
	int _sockFd;
	int _domain;

public:
	TcpClient(int domain);
	TcpClient(const TcpClient &client);
	~TcpClient();
	TcpClient operator=(TcpClient &client);

	int connServ(char *ip, int port);  // 连接服务器
	int sendToServ(std::string str);   // 发送信息
	std::string recvFromServ();		   // 接收信息
};