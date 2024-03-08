#include "kafka_producer.h"

using namespace DevKit;

KafkaProducer::KafkaProducer(int p, std::string t)
	: _partition(p), _topicName(t), _producer(nullptr) {}

ErrCode KafkaProducer::init(const std::string broker) {
	RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
	std::string errStr;

	if (conf->set("bootstrap.servers", broker, errStr) != RdKafka::Conf::CONF_OK)
		return BROKER_ERROR;

	_producer = RdKafka::Producer::create(conf, errStr);

	delete conf;

	return _producer ? OK : PRODUCER_ERROR;
}

ErrCode KafkaProducer::send(std::string data, RdKafka::Headers *headers) {
	RdKafka::ErrorCode errCode;

	errCode = _producer->produce(_topicName,
								 _partition, RdKafka::Producer::RK_MSG_COPY,
								 const_cast<char *>(data.c_str()), data.size(),
								 nullptr, 0,
								 0, headers, nullptr);
	_producer->poll(0);
	if (errCode != RdKafka::ERR_NO_ERROR)
		return PRODUCER_ERROR;
	else
		return OK;
}
