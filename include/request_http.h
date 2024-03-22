#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

namespace DevKit {
class RequestHttp {
private:
	using StringList = std::vector<std::string>;
	using HeadList=std::unordered_map<std::string,std::string>;
	std::string _message;
	std::string _type;
	std::string _version;
	std::string _url;
	HeadList _heads;
	std::string _body;

	void handleRequestLine(std::string line);
	void handleHead(StringList heads);

public:
	RequestHttp(std::string m);
	std::string getType();
	std::string getVersion();
	std::string getUrl();
	std::string getHead(std::string head);
	std::string getBody();
};
}  // namespace DevKit
