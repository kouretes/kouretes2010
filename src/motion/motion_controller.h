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


class MotionController : public Thread, public Publisher, public Subscriber{

private:

	AL::ALPtr<AL::ALMotionProxy> motion;
	AL::ALPtr<AL::ALMemoryProxy> memory;
	
	float *AccZ;

	MotionCommand command;

	bool robotDown;
	bool walkActive;
	bool headActive;
	bool actionActive;

	int walkPID;
	int headPID;
	int actionPID;

	int counter;


public:

	MotionController(AL::ALPtr<AL::ALBroker> pbroker, MessageQueue* mq = 0) {
	    	try {motion = pbroker->getMotionProxy();} 
		catch (AL::ALError& e) {cout << "Error in getting motion proxy" << std::endl;}

	    	try {memory = pbroker->getMemoryProxy();} 
		catch (AL::ALError& e) {cout << "Error in getting memory proxy" << std::endl;}
				
		AccZ = (float*) memory->getDataPtr("Device/SubDeviceList/InertialSensor/AccZ/Sensor/Value");

		if (mq != 0){
  			mq->add_subscriber(this);
			mq->subscribe("motion",this,0);
		}

		robotDown = 0;
		walkPID = 0;
		headPID = 0;
		actionPID = 0;

		counter = 0;
	}

	void run();

	void mglrun();
	
	void ALstandUp();
	
	void ALstandUpCross();
	
	void ALstandUpFront();
	
	void ALstandUpBack();

	

};

#endif
