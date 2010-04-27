#include "behavior.h"



BehaviorController::BehaviorController(MessageQueue* mq): Thread(false), Subscriber("BehaviorController"),Publisher("BehaviorController")
{
	cout << "beh Constructor" << endl;
	if(mq != 0)
	{
		cout << "fooooooooooooooooooo" << endl;
			mq->add_publisher(this);
			mq->add_subscriber(this);
			mq->subscribe("behavior",this,0);
	}
	
	TestMessage* tt = new TestMessage();
	tt->set_topic("localization");
	Publisher::publish(tt);
	cout << "be Controller Initialized" <<endl;
}


void BehaviorController::run()
{
	MessageBuffer* sub_buf = Subscriber::getBuffer();
	if(sub_buf->size() > 0)
		process_messages();
	//else
//	{
		//cout << "create traffic" << endl;
	//	TestMessage* tt = new TestMessage();
//	tt->set_topic("localization");
	//Publisher::publish(tt);
	//delete tt;
	//}
	//cout << "behavior " << endl;
	boost::thread::yield();
	
}

void BehaviorController::process_messages()
{
	MessageBuffer* sub_buf = Subscriber::getBuffer();
	TestMessage* tm = (TestMessage*)sub_buf->remove_head();
	cout << "I received a message from " << tm->publisher() << " with counter " << tm->counter() << endl;
	tm->set_counter(tm->counter() + 1);
	tm->set_topic("localization");
	publish(tm);
}
