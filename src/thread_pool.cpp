#include "thread_pool.h"

ThreadPool::ThreadPool(int mtn)
	: _maxThreadNum(mtn), _curThreadNum(0), _idleThreadNum(0), _isQuit(false) {}

ThreadPool::~ThreadPool() {
	_isQuit = true;
	_cv.notify_all();

	for (auto &pair : _threads)
		if (pair.second.joinable())
			pair.second.join();
}

int ThreadPool::getCurThreadNum() {
	std::lock_guard lg(_mtx);

	return _curThreadNum;
}

void ThreadPool::worker() {
	while (true) {
		std::function<void()> task;	 // ��������
		bool isTimeout;				 // �Ƿ�ȴ���ʱ

		{
			std::unique_lock ul(_mtx);

			_idleThreadNum++;
			isTimeout = !_cv.wait_for(ul, std::chrono::seconds(WAIT_SECONDS), [=] {
				return _isQuit || !_tasks.empty();
			});
			_idleThreadNum--;

			if (_tasks.empty()) {
				_curThreadNum--;

				if (isTimeout)	// ��ʱ˵���̹߳��࣬�����߳�
					joinFinishedThread(std::this_thread::get_id());

				return;
			}
			else {
				task = _tasks.front();
				_tasks.pop();
			}
		}

		if (task != nullptr)
			task();	 // ִ������
	}
}

void ThreadPool::joinFinishedThread(std::thread::id threadId) {
	if (_threads[threadId].joinable())
		_threads[threadId].join();

	_threads.erase(threadId);
}