#include <functional>
#include <unordered_map>
#include <queue>
#include <thread>
#include <sys/epoll.h>
#include "thread_pool.hpp"
#include "kit_exception.hpp"

#define EPOLL_SIZE 50	  // 创建 _epollFd 的长度
#define MAX_EVENT_NUM 10  // 最大能接收的事件数

class EventPoller {
private:
	using EventFunc = std::function<void(int fd)>;
	using DelFunc = std::function<void()>;

	bool _isStop;								 // 是否停止了 loop 循环
	int _epollFd;								 // epoll 的文件描述符
	ThreadPool *_threadPool;					 // 线程池
	std::unordered_map<int, EventFunc> _events;	 // 文件描述符和对应的回调函数

public:
	EventPoller();
	~EventPoller();
	void addEvent(int fd, int eventType, EventFunc func);  // 添加事件
	void delEvent(int fd, DelFunc func = nullptr);		   // 删除事件
	void modifyEvent(int fd, int eventType);			   // 更改事件的监听类型
	void pollEvent(int fd);								   // 执行某一事件
	void startLoop();									   // 开始监听循环
	void stopLoop();									   // 停止监听循环
};