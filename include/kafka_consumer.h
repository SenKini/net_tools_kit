#pragma once
#include <string>
#include <thread>
#include <future>
#include "librdkafka/rdkafkacpp.h"
#include "err_code.h"

namespace DevKit {
class KafkaConsumer {
protected:
	RdKafka::Consumer *_consumer;
	std::future<void> _loopFuture;
	std::string _topicName;
	int _partition;
	volatile bool _isStop;

	void loop(RdKafka::Topic *topic);
	virtual void callback(RdKafka::Message *) = 0;

public:
	KafkaConsumer(int p, std::string t);
	virtual ~KafkaConsumer();
	ErrCode init(const std::string broker);
	void startLoop();
	void stopLoop();
};
}  // namespace DevKit
