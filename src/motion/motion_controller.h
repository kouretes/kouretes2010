#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H
#include "system/Thread.h"
#include "pub_sub/publisher.h"
#include "pub_sub/subscriber.h"
#include "alxplatform.h"
#include <albroker.h>
#include <alproxy.h>
#include <almemoryfastaccess.h>
#include "almotionproxy.h"
#include "almemoryproxy.h"
#include "albrokermanager.h"
#include "alptr.h"
#include "alvalue.h"
#include "messages/motion.pb.h"

#define WEBOTS


class MotionController : public Thread, public Publisher, public Subscriber{

private:

	AL::ALPtr<AL::ALMotionProxy> motion;
	AL::ALPtr<AL::ALMemoryProxy> memory;

	float *AccZ;

	bool robotDown;
	bool robotUp;
	float AccZvalue;

	int counter;

	int walkPID;
	std::string walkCommand;
	float walkParam1, walkParam2, walkParam3, walkParam4;

	int headPID;
	std::string headCommand;
	float headParam1, headParam2;
	AL::ALValue names, values;

	int actionPID;

	MessageBuffer* sub_buffer;
	MotionMessage* mm;

public:

	MotionController(AL::ALPtr<AL::ALBroker> pbroker, MessageQueue* mq) {
	    	try {motion = pbroker->getMotionProxy();}
		catch (AL::ALError& e) {cout << "Error in getting motion proxy" << std::endl;}
		motion->setStiffnesses("Body", 1.0);

	    	try {memory = pbroker->getMemoryProxy();}
		catch (AL::ALError& e) {cout << "Error in getting memory proxy" << std::endl;}

		AccZ = (float*) memory->getDataPtr("Device/SubDeviceList/InertialSensor/AccZ/Sensor/Value");

		if (mq != 0){
			mq->add_publisher(this);
  			mq->add_subscriber(this);
			mq->subscribe("motion",this,0);
		}
		sub_buffer = Subscriber::getBuffer();

		robotDown = false;
		robotUp = true;

		walkPID = 0;
		headPID = 0;
		actionPID = 0;

		counter = 0;

	}

	void run();
	void commands();
	void mglrun();
	void process_messages();

	void ALstandUp();
	void ALstandUpCross();
	void ALstandUpFront();
	void ALstandUpBack();

	void ALstandUp2010();
	void ALstandUpFront2010();
	void ALstandUpBack2010();

};

#endif
