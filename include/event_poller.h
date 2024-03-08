#pragma once
#include <unordered_map>
#include <queue>
#include <thread>
#include <future>
#include <sys/epoll.h>
#include "err_code.h"

namespace DevKit {
class EventPoller {
protected:
	const int _EPOLL_SIZE;				// epoll 长度
	const int _MAX_EVENT_NUM;			// 最大事件数
	volatile bool _isStop;				// 是否停止了 loop 循环
	int _epollFd;						// epoll 的文件描述符
	std::future<void> _callbackFuture;	// 管理回调函数产生的子线程

	void loop();
	virtual void callback(int fd) = 0;

public:
	EventPoller();
	virtual ~EventPoller();
	ErrCode addEvent(int fd, int eventType);	 // 添加事件
	ErrCode delEvent(int fd);					 // 删除事件
	ErrCode modifyEvent(int fd, int eventType);	 // 更改事件的监听类型
	ErrCode startLoop();						 // 开始监听循环
	void stopLoop();							 // 停止监听循环
};
}  // namespace DevKit
