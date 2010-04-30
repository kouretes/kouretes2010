#ifndef OBSTACLEAVOIDANCE_H
#define OBSTACLEAVOIDANCE_H
#include "pub_sub/subscriber.h"
#include "pub_sub/publisher.h"
#include "system/Thread.h"
#include "messages/TestMessage.pb.h"
#include "almemoryproxy.h"
#include "alptr.h"
#include "alxplatform.h"
#include <albroker.h>
#include <alproxy.h>
#include "messages/SensorsMessage.pb.h"
#include <vector>
#include <map>
#include <string>

class ObstacleAvoidanceController: public Thread, public Publisher, public Subscriber {
	public:
		ObstacleAvoidanceController(AL::ALPtr<AL::ALBroker> pbroker, MessageQueue* mq = 0);
		void run();
		void process_messages();
		~ObstacleAvoidanceController() {
			;
		}

		UltaSoundSensorsMessage * USSM;

	private:
		AL::ALPtr<AL::ALMemoryProxy> memory;
};

#endif  

