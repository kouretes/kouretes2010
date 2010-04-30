#ifndef SENSORS_H
#define SENSORS_H
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
#include <rttime.h>
#define NUMBER_OF_SENSORS 35//TODO Please check the number devices
class SensorController: public Thread, public Publisher/*, public Subscriber*/{
	public:
		SensorController(AL::ALPtr<AL::ALBroker> pbroker, MessageQueue* mq = 0);
		void run();
		void process_messages();
		~SensorController() {
			;
		}

		InertialSensorsMessage ISM;
		FSRSensorsMessage FSM;
		UltaSoundSensorsMessage USSM;
		HeadJointSensorsMessage HJSM;
		BodyJointSensorsMessage BJSM;

	private:
		AL::ALPtr<AL::ALMemoryProxy> memory;

		void initialisation();

		std::map<std::string, std::vector<std::string> > devicesInChains;
		std::map<std::string, std::vector<std::string> > devicesNames;

		std::vector<float> Values;

		RtTime rtm;
		unsigned int timediff;
		//Indexing according initialization
		float devicesValues[NUMBER_OF_SENSORS];
};

#endif  

