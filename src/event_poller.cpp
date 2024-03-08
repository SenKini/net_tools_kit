#include "event_poller.h"

using namespace DevKit;

void EventPoller::loop() {
	auto happEvents = new epoll_event[_MAX_EVENT_NUM];	// 发生的事件

	while (!_isStop) {	// 没有停止就一直循环
		int eventNum = epoll_wait(_epollFd, happEvents, _MAX_EVENT_NUM, 5000);
		if (eventNum != -1)
			for (int i = 0; i < eventNum; i++)
				callback(happEvents[i].data.fd);
	}

	delete[] happEvents;
}

EventPoller::EventPoller()
	: _isStop(false), _epollFd(epoll_create(_EPOLL_SIZE)), _EPOLL_SIZE(50), _MAX_EVENT_NUM(10) {}

EventPoller::~EventPoller() {
	_isStop = true;
}

ErrCode EventPoller::addEvent(int fd, int eventType) {
	epoll_event ee;
	int state;

	ee.events = eventType;
	ee.data.fd = fd;
	state = epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &ee);

	return (state != -1) ? OK : EPOLL_ERROR;
}

ErrCode EventPoller::delEvent(int fd) {
	int state = epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, NULL);

	return (state != -1) ? OK : EPOLL_ERROR;
}

ErrCode EventPoller::modifyEvent(int fd, int eventType) {
	epoll_event ee;
	int state;

	ee.events = eventType;
	ee.data.fd = fd;
	state = epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &ee);

	return (state != -1) ? OK : EPOLL_ERROR;
}

ErrCode EventPoller::startLoop() {
	_callbackFuture = std::async(std::launch::deferred, &EventPoller::loop, this);
}

void EventPoller::stopLoop() {
	_isStop = true;
}