#include "request_http.h"

void DevKit::RequestHttp::handleRequestLine(std::string line) {
	std::istringstream iss(line);
	iss >> _type >> _url >> _version;
}

void DevKit::RequestHttp::handleHead(StringList heads) {
	for (auto head : heads) {
		std::istringstream iss(head);
		std::string key, value;
		std::getline(iss, key, ':');  // �����ֶ���
		std::getline(iss, value);  // ����ֵ
		_heads[key] = value;
	}
}

DevKit::RequestHttp::RequestHttp(std::string m) {
	std::istringstream iss(m);	// �ַ�����

	// ��ȡ������
	std::string requestLine;
	std::getline(iss, requestLine);
	handleRequestLine(requestLine);

	// ��ȡ�ײ���
	StringList heads;
	while (true) {
		std::string buf;
		std::getline(iss, buf);
		if (buf != "\r")  // ʹ�� getline() ������ '\n'
			heads.push_back(buf);
		else
			break;
	}
	handleHead(heads);

	// ��ȡʵ����
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
