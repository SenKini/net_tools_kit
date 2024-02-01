#include <chrono>
class Timer {
private:
	using TIME_POINT = std::chrono::steady_clock::time_point;
	using MICRO_SECONDS = std::chrono::microseconds;

	TIME_POINT _startTime;
	TIME_POINT _endTime;
	MICRO_SECONDS _elapsedTime;

public:
	void start();
	void stop();
	void restart();
	long long getMicro();
	long long getMilli();
	long long getSecond();
};