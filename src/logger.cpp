#include "logger.hpp"

/* 对静态成员变量初始化 */
std::shared_ptr<Logger> Logger::_logger = nullptr;
std::mutex Logger::_mtx;

Logger::Logger()
	: _target(TARGET_FILE), _logFilePath("./") {}

std::string Logger::levelToStr(LogLevel level) {
	switch (level) {
		case LEVEL_ERROR:
			return "LEVEL_ERROR";
		case LEVEL_WARNING:
			return "LEVEL_WARNING";
		case LEVEL_DEBUG:
			return "LEVEL_DEBUG";
		case LEVEL_INFO:
			return "LEVEL_INFO";
		default:
			return "";
	}
}

std::string Logger::pidToStr(std::thread::id pid) {
	std::stringstream ss;

	ss << pid;

	return ss.str();
}

std::shared_ptr<Logger> Logger::getInstance() {
	std::lock_guard lg(_mtx);

	if (_logger == nullptr)
		_logger = std::make_shared<Logger>();
	else
		return _logger;
}

void Logger::init(LogTarget target, std::string logFilePath) {
	_target = target;
	_logFilePath = logFilePath;
}

void Logger::setTarget(LogTarget target) {
	_target = target;
}

int Logger::writeLog(LogLevel level, std::string fileName, std::string funcName,
					 int lineNum, std::string mess) {
	std::string levelStr = levelToStr(level);					// 字符串形式的日志等级
	std::string pidStr = pidToStr(std::this_thread::get_id());	// 字符串形式的 pid
	std::string logStr;											// 要输入的日志信息
	std::ofstream output;										// 要写入的日志文件流

	if (levelStr.empty())
		return -1;

	logStr.append("[" + pidStr + "]");
	logStr.append("[" + levelStr + "]");
	logStr.append("[" + fileName + "]");
	logStr.append("[" + funcName + "]");
	logStr.append("[" + std::to_string(lineNum) + "]");
	logStr.append(" " + mess);

	std::lock_guard lg(_mtx);
	switch (_target) {
		case TARGET_FILE:
			output.open(_logFilePath, std::ios::out);
			if (output.is_open())
				output << logStr << std::endl;
			else
				throw KitException("logger.cpp 71", "open() error");
			return logStr.size();
		case TARGET_CONSOLE:
			std::cout << logStr << std::endl;
			return logStr.size();
		default:
			return -1;
	}
}

void Logger::printLog(LogLevel level, LogTarget target, std::string path) {
	std::ifstream input;   // 日志文件流
	std::ofstream output;  // 筛选后的日志文件流
	char info[100];		   // 每一行日志信息

	output.open(path, std::ios::out);
	input.open(_logFilePath, std::ios::in);
	if (!input.is_open())
		throw KitException("logger.cpp 91", "open() error");

	if (target == TARGET_FILE)
		if (!output.is_open())
			throw KitException("logger.cpp 90", "open() error");
		else {
			while (!input.eof()) {
				input.getline(info, sizeof(info));

				if (level == LEVEL_NONE)
					output << info << std::endl;
				else {
					std::string infoStr = info;

					if (infoStr.find(levelToStr(level)) != std::string::npos)
						output << info << std::endl;
				}
			}
		}
	else
		while (!input.eof()) {
			input.getline(info, sizeof(info));

			if (level == LEVEL_NONE)
				std::cout << info << std::endl;
			else {
				std::string infoStr = info;

				if (infoStr.find(levelToStr(level)) != std::string::npos)
					std::cout << info << std::endl;
			}
		}
}