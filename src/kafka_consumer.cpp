#include "kafka_consumer.h"

using namespace DevKit;

void KafkaConsumer::loop(RdKafka::Topic *topic) {
	_consumer->start(topic, 0, RdKafka::Topic::OFFSET_BEGINNING);

	while (!_isStop) {
		RdKafka::Message *msg = _consumer->consume(topic, _partition, 5000);
		if (msg->err() == RdKafka::ERR_NO_ERROR)
			callback(msg);

		delete msg;
	}
}

KafkaConsumer::KafkaConsumer(int p, std::string t)
	: _partition(p), _topicName(t), _consumer(nullptr), _isStop(false) {}

KafkaConsumer::~KafkaConsumer() {
	_loopFuture.wait();
}

ErrCode KafkaConsumer::init(const std::string broker) {
	RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
	std::string errStr;

	if (conf->set("bootstrap.servers", broker, errStr) != RdKafka::Conf::CONF_OK)
		return BROKER_ERROR;

	_consumer = RdKafka::Consumer::create(conf, errStr);

	delete conf;

	return _consumer ? OK : CONSUMER_ERROR;
}

void KafkaConsumer::startLoop() {
	RdKafka::Conf *topicConf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
	std::string errStr;
	RdKafka::Topic *topic = RdKafka::Topic::create(_consumer, _topicName, topicConf, errStr);

	_loopFuture = std::async(std::launch::deferred, &KafkaConsumer::loop, this, topic);
}

void KafkaConsumer::stopLoop() {
	_isStop = true;
}
