#include "localization.h"


LocController::LocController(MessageQueue* mq): Thread(false), Publisher("LocController"),Subscriber("LocController")
{
	cout << "Loc Controller Constructor" << endl;
	if(mq != 0)
	{
			mq->add_publisher(this);
			mq->add_subscriber(this);
			mq->subscribe("localization",this,0);
	}
	cout << "Localization Controller Initialized" <<endl;
}


void LocController::run()
{
	MessageBuffer* sub_buf = Subscriber::getBuffer();
	if(sub_buf->size() > 0)
		process_messages();
//cout << "Localization " << endl;
	boost::thread::yield();
	
}

void LocController::process_messages()
{
	MessageBuffer* sub_buf = Subscriber::getBuffer();
	TestMessage* tm = (TestMessage*)sub_buf->remove_head();
	cout << "I received a message from " << tm->publisher() << " with counter " << tm->counter() << endl;
	tm->set_counter(tm->counter() + 1);
	tm->set_topic("sensors");
	publish(tm);
}
