#pragma once
#include <string>
#include "librdkafka/rdkafkacpp.h"
#include "err_code.h"

namespace DevKit {
class KafkaProducer {
private:
	RdKafka::Producer *_producer;
	int _partition;
	std::string _topicName;

public:
	KafkaProducer(int p, std::string t);
	ErrCode init(const std::string broker);
	ErrCode send(std::string data, RdKafka::Headers *headers = nullptr);
};
}  // namespace DevKit
