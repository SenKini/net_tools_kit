#pragma once

#include <string>
#include <mutex>
#include <iostream>
#include <fstream>
#include <thread>
#include <sstream>
#include <memory>
#include "kit_exception.hpp"

enum LogLevel {
	LEVEL_NONE,	 // 仅用于初始化和筛选
	LEVEL_ERROR,
	LEVEL_WARNING,
	LEVEL_DEBUG,
	LEVEL_INFO
};	// 日志等级

enum LogTarget {
	TARGET_NONE,  // 仅用于初始化
	TARGET_CONSOLE,
	TARGET_FILE
};	// 日志输出方式

class Logger {
private:
	static std::shared_ptr<Logger> _logger;	 // 全局指针
	static std::mutex _mtx;					 // 互斥量，用于多线程中的日志输出
	std::string _logFilePath;				 // 日志文件路径
	LogTarget _target;						 // 日志输出方式，默认文件

	std::string levelToStr(LogLevel level);		// 将日志等级转换为字符串
	std::string pidToStr(std::thread::id pid);	// 将 pid 转换为字符串

public:
	Logger();

	static std::shared_ptr<Logger> getInstance();															  // 获得唯一对象指针
	void init(LogTarget target, std::string logFilePath);													  // 初始化
	void setTarget(LogTarget target);																		  // 设置日志输出方式
	int writeLog(LogLevel level, std::string fileName, std::string funcName, int lineNum, std::string mess);  // 写入日志
	void printLog(LogLevel level, LogTarget target, std::string path = "");									  // 筛选日志内容
};
