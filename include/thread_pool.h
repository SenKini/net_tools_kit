#pragma once
#include <mutex>
#include <vector>
#include <thread>
#include <queue>
#include <future>
#include <memory>
#include <functional>
#include <unordered_map>
#include <condition_variable>

#define WAIT_SECONDS 2

class ThreadPool {
private:
	std::mutex _mtx;											// ������
	std::condition_variable _cv;								// ��������
	std::unordered_map<std::thread::id, std::thread> _threads;	// ����̣߳���ֵ�Ե���ʽ������ǰ���������߳�
	std::queue<std::function<void()>> _tasks;					// �������
	int _maxThreadNum;											// ����߳���
	int _curThreadNum;											// �������е��߳�
	int _idleThreadNum;											// ���е��߳�
	bool _isQuit;												// �Ƿ��˳�

	void worker();										// ִ�� task
	void joinFinishedThread(std::thread::id threadId);	// ��ǰ����������߳�

public:
	ThreadPool(int mtn);
	~ThreadPool();
	ThreadPool(const ThreadPool &) = delete;
	ThreadPool &operator=(const ThreadPool &) = delete;

	int getCurThreadNum();	// ��ȡ�������е��߳���

	template <typename Func, typename... Arg>
	auto addTask(Func &&func, Arg &&...args) {	// �������
		using ReturnType = decltype(func(args...));

		auto taskFunc = std::bind(std::forward<Func>(func), std::forward<Arg>(args)...);
		auto taskPackge = new std::packaged_task<ReturnType()>(taskFunc);
		auto res = taskPackge->get_future();

		if (!_isQuit) {
			std::lock_guard lg(_mtx);

			_tasks.emplace([taskPackge] {
				(*taskPackge)();
			});

			if (_idleThreadNum > 0)	 // ����п����߳��򲻱��½��߳�
				_cv.notify_one();
			else if (_curThreadNum < _maxThreadNum) {  // ��֤�߳������ᳬ�����ֵ
				std::thread t(&ThreadPool::worker, this);

				_threads[t.get_id()] = std::move(t);
				_curThreadNum++;
			}
		}

		return res;
	}
};