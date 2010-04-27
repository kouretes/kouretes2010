#ifndef LOC_H
#define LOC_H
#include "pub_sub/subscriber.h"
#include "pub_sub/publisher.h"
#include "system/Thread.h"
#include "messages/TestMessage.pb.h"

class LocController : public Thread, public Publisher, public Subscriber{
public:
	LocController(MessageQueue* mq = 0);
	void run();
	void process_messages();
	~LocController(){;}

private:

	
	};


#endif  
