#pragma once
#include <string>
#include "librdkafka/rdkafkacpp.h"

namespace DevKit {
class KafkaProducer {
private:
	RdKafka::Producer *_producer;
	int _partition;
	std::string _topicName;

public:
	KafkaProducer(int p, std::string t);
	bool init(const std::string broker);
	bool send(std::string data, RdKafka::Headers *headers = nullptr);
};
}  // namespace DevKit
