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

public:

	MotionController(AL::ALPtr<AL::ALBroker> pbroker, MessageQueue* mq);

	void run();

private:

	AL::ALPtr<AL::ALMotionProxy> motion;
	AL::ALPtr<AL::ALMemoryProxy> memory;

	float *AccZ;

	bool robotDown;
	bool robotUp;
	float AccZvalue;

	int counter;

	int walkPID;
	float walkParam1, walkParam2, walkParam3, walkParam4;

	int headPID;
	float headParam1, headParam2;

	int actionPID;
	
	AL::ALValue names, values;

	MessageBuffer* sub_buffer;
	MotionMessage* mm;

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
	
	void loadActions();
	AL::ALValue LieDown_names, LieDown_times, LieDown_keys;

};

#endif
