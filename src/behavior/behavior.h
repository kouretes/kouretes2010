#ifndef BEHAVIOR_H
#define BEHAVIOR_H
#include "pub_sub/subscriber.h"
#include "pub_sub/publisher.h"
#include "system/Thread.h"
#include "messages/TestMessage.pb.h"
#include "messages/motion.pb.h"
#include "alxplatform.h"
#include <albroker.h>
#include <alproxy.h>
#include <almemoryfastaccess.h>
#include "almotionproxy.h"
#include "almemoryproxy.h"
#include "albrokermanager.h"
#include "alptr.h"
#include "alvalue.h"
class BehaviorController: public Thread, public Publisher, public Subscriber {
	public:
		BehaviorController(AL::ALPtr<AL::ALBroker> pbroker, MessageQueue* mq = 0);
		void run();
		void process_messages();
		~BehaviorController() {
			;
		}

	private:
		AL::ALPtr<AL::ALMotionProxy> motion;
		AL::ALPtr<AL::ALMemoryProxy> memory;

		MotionMessage* mot;
};

#endif  
