#ifndef SENSORS_H
#define SENSORS_H
#include "pub_sub/subscriber.h"
#include "pub_sub/publisher.h"
#include "system/Thread.h"
#include "messages/TestMessage.pb.h"

class SensorController : public Thread, public Publisher, public Subscriber{
public:
	SensorController(MessageQueue* mq = 0);
	void run();
	void process_messages();
	~SensorController(){;}

private:

	
	};


#endif  
