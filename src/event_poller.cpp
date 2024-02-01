#include "event_poller.hpp"

EventPoller::EventPoller()
	: _isStop(true), _epollFd(epoll_create(EPOLL_SIZE)), _threadPool(new ThreadPool(5)) {}

EventPoller::~EventPoller() {
	delete _threadPool;
}

void EventPoller::addEvent(int fd, int eventType, EventFunc func) {
	epoll_event ee;
	int state;

	ee.events = eventType;
	ee.data.fd = fd;
	state = epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &ee);

	if (state == -1)
		throw KitException("event_poller.cpp 17", "epoll_ctl() error");
	else
		_events[fd] = func;
}

void EventPoller::delEvent(int fd, DelFunc func) {
	int state = epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, NULL);

	_events.erase(fd);

	if (state == -1)
		throw KitException("event_poller.cpp 22", "epoll_ctl() error");
	else {
		std::thread afterDel(func);
		afterDel.detach();
	}
}

void EventPoller::modifyEvent(int fd, int eventType) {
	epoll_event ee;
	int state;

	ee.events = eventType;
	ee.data.fd = fd;
	state = epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &ee);

	if (state == -1)
		throw KitException("event_poller.cpp 44", "epoll_ctl() error");
}

void EventPoller::pollEvent(int fd) {
	if (_events.find(fd) != _events.end()) {
		auto fut = _threadPool->addTask(_events[fd], fd);  // 在线程池中创建新线程执行回调函数

		fut.get();
	}
}

void EventPoller::startLoop() {
	auto happEvents = new epoll_event[MAX_EVENT_NUM];  // 发生的事件

	while (!_isStop) {	// 没有停止就一直循环
		int eventNum = epoll_wait(_epollFd, happEvents, MAX_EVENT_NUM, -1);

		if (eventNum == -1)
			throw KitException("event_poller.cpp 52", "epoll_wait() error");
		else
			for (int i = 0; i < eventNum; i++)
				pollEvent(happEvents[i].data.fd);
	}

	delete[] happEvents;
}

void EventPoller::stopLoop() {
	_isStop = true;
}