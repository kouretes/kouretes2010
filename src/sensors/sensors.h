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
#define NUMBER_OF_SENSORS 42//TODO Please check the number devices
// Use DCM proxy
#include "dcmproxy.h"

//#define USE_POINTERS

namespace AL {
	class ALBroker;
	class DCMProxy;
}

class SensorController: public Thread, public Publisher/*, public Subscriber*/{
	public:
		SensorController(AL::ALPtr<AL::ALBroker> pbroker, MessageQueue* mq = 0);
		void run();
		void process_messages();
		~SensorController() {
			;

			AL::ALValue commands;

			commands.arraySetSize(3);
			commands[0] = std::string("Device/SubDeviceList/US/Actuator/Value");
			commands[1] = std::string("Merge");
			commands[2].arraySetSize(1);
			commands[2][0].arraySetSize(2);
			commands[2][0][0] = 0.0;
			commands[2][0][1] = dcm->getTime(10);

			dcm->set(commands);

		}

		InertialSensorsMessage ISM;
		FSRSensorsMessage FSM;
		UltaSoundSensorsMessage USSM;
		HeadJointSensorsMessage HJSM;
		BodyJointSensorsMessage BJSM;

	private:
		AL::DCMProxy* dcm;

		AL::ALPtr<AL::ALMemoryProxy> memory;

		void initialisation();

		std::map<std::string, std::vector<float *> > SensorDataPtr;

		std::map<std::string, std::vector<std::string> > devicesInChains;
		std::map<std::string, std::vector<std::string> > devicesNames;

		std::map<std::string, std::vector<float> > Values;

		RtTime rtm;
		unsigned int timediff;

		unsigned int period;
		//Indexing according initialization
		float devicesValues[NUMBER_OF_SENSORS];
};

#endif  

