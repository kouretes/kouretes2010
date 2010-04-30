#include "ObstacleAvoidance.h"

ObstacleAvoidanceController::ObstacleAvoidanceController(AL::ALPtr<AL::ALBroker> pbroker, MessageQueue* mq) :
	Thread(false), Publisher("ObstacleAvoidanceController"), Subscriber("ObstacleAvoidanceController") {
	cout << "ObstacleAvoidance Controller Constructor" << endl;
	try {
		memory = pbroker->getMemoryProxy();
	} catch (AL::ALError& e) {
		cout << "Error in getting memory proxy" << std::endl;
	}

	if (mq != 0) {
		mq->add_publisher(this);
		mq->add_subscriber(this);
		mq->subscribe("sensors", this, 0);
	}

	cout << "ObstacleAvoidance Controller Initialized" << endl;
}

void ObstacleAvoidanceController::run() {
	MessageBuffer* sub_buf = Subscriber::getBuffer();
	if (sub_buf->size() > 0)
		process_messages();
	//cout << "Localization " << endl;
	boost::thread::yield();

}

void ObstacleAvoidanceController::process_messages() {
	cout << "ObstacleAvoidanceController process messages" << endl;
	MessageBuffer* sub_buf = Subscriber::getBuffer();
	TestMessage* tm = (TestMessage*) sub_buf->remove_head();
	cout << "I received a message from " << tm->publisher() << " with counter " << tm->counter() << endl;
	tm->set_counter(tm->counter() + 1);
	tm->set_topic("behavior");
	publish(tm);
}
