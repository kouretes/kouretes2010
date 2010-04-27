#ifndef BEHAVIOR_H
#define BEHAVIOR_H
#include "pub_sub/subscriber.h"
#include "pub_sub/publisher.h"
#include "system/Thread.h"
#include "messages/TestMessage.pb.h"

class BehaviorController : public Thread, public Publisher, public Subscriber{
public:
	BehaviorController(MessageQueue* mq = 0);
	void run();
	void process_messages();
	~BehaviorController(){;}

private:

	
	};


#endif  
