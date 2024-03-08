#pragma once
#include <string>

namespace DevKit {
enum ErrCode {
	OK,
	SOCKET_ERROR,
	BIND_ERROR,
	LISTEN_ERROR,
	ACCEPT_ERROR,
	READ_ERROR,
	WRITE_ERROR,
	EPOLL_ERROR,
	BROKER_ERROR,
	PRODUCER_ERROR,
	CONSUMER_ERROR
};

std::string getErrStr(ErrCode errCode);

class saveErrInfo {
private:
	void saveSysErr();
	void saveKafkaErr();
};
}  // namespace DevKit
