#include "sensors.h"


SensorController::SensorController(MessageQueue* mq): Thread(false), Publisher("SensorController"),Subscriber("SensorsController")
{
	cout << "Sensor Controller Constructor" << endl;
	if(mq != 0)
	{
			mq->add_publisher(this);
			mq->add_subscriber(this);
			mq->subscribe("sensors",this,0);
	}
	cout << "Sensor Controller Initialized" <<endl;
}


void SensorController::run()
{
	MessageBuffer* sub_buf = Subscriber::getBuffer();
	if(sub_buf != 0)
		if(sub_buf->size() > 0)
			process_messages();
//cout << "Sensor " << endl;
boost::thread::yield();	
}

void SensorController::process_messages()
{
	cout << "Sensor process messages" << endl;
	MessageBuffer* sub_buf = Subscriber::getBuffer();
	TestMessage* tm = (TestMessage*)sub_buf->remove_head();
	cout << "I received a message from " << tm->publisher() << " with counter " << tm->counter() << endl;
	tm->set_counter(tm->counter() + 1);
	tm->set_topic("behavior");
	publish(tm);
}
