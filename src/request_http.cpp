#include "request_http.h"

void DevKit::RequestHttp::handleRequestLine(std::string line) {
	std::istringstream iss(line);
	iss >> _type >> _url >> _version;
}

void DevKit::RequestHttp::handleHead(StringList heads) {
	for (auto head : heads) {
		std::istringstream iss(head);
		std::string key, value;
		std::getline(iss, key, ':');  // 分离字段名
		std::getline(iss, value);  // 分离值
		_heads[key] = value;
	}
}

DevKit::RequestHttp::RequestHttp(std::string m) {
	std::istringstream iss(m);	// 字符串流

	// 提取请求行
	std::string requestLine;
	std::getline(iss, requestLine);
	handleRequestLine(requestLine);

	// 提取首部行
	StringList heads;
	while (true) {
		std::string buf;
		std::getline(iss, buf);
		if (buf != "\r")  // 使用 getline() 会舍弃 '\n'
			heads.push_back(buf);
		else
			break;
	}
	handleHead(heads);

	// 提取实体体
	char buf[1024];
	std::string body;
	while (!iss.eof()) {
		iss.read(buf, sizeof(buf));
		body.append(buf, iss.gcount());
	}
	_body = body;
}

std::string DevKit::RequestHttp::getType() {
	return _type;
}

std::string DevKit::RequestHttp::getVersion() {
	return _version;
}

std::string DevKit::RequestHttp::getUrl() {
	return _url;
}

std::string DevKit::RequestHttp::getHead(std::string head) {
	auto pair = _heads.find(head);
	return (pair != _heads.end()) ? pair->second : "";
}

std::string DevKit::RequestHttp::getBody() {
	return _body;
}
