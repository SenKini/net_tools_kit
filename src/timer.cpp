#include "timer.hpp"

void Timer::start() {
	_startTime = std::chrono::steady_clock::now();
}

void Timer::stop() {
	_endTime = std::chrono::steady_clock::now();
	_elapsedTime = std::chrono::duration_cast<MICRO_SECONDS>(_endTime - _startTime);
}

void Timer::restart() {
	_elapsedTime = std::chrono::microseconds::zero();
	_startTime = std::chrono::steady_clock::now();
}

long long Timer::getMicro() {
	return _elapsedTime.count();
}

long long Timer::getMilli() {
	using MILLI_SECONDS = std::chrono::milliseconds;
	return std::chrono::duration_cast<MILLI_SECONDS>(_elapsedTime).count();
}

long long Timer::getSecond() {
	using SECONDS = std::chrono::seconds;

	return std::chrono::duration_cast<SECONDS>(_elapsedTime).count();
}
