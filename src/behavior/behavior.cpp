#include "behavior.h"
#include "messages/motion.pb.h"
#include "messages/VisionObservations.pb.h"
#include "messages/SensorsMessage.pb.h"

BehaviorController::BehaviorController(AL::ALPtr<AL::ALBroker> pbroker, MessageQueue* mq) :
	Thread(false), Subscriber("BehaviorController"), Publisher("BehaviorController") {
	//cout << "beh Constructor" << endl;
	if (mq != 0) {
		mq->add_publisher(this);
		mq->add_subscriber(this);
		mq->subscribe("vision", this, 0);
		mq->subscribe("sensors", this, 0);

	}
	try {
		memory = pbroker->getMemoryProxy();
	} catch (AL::ALError& e) {
		cout << "Error in getting memory proxy" << std::endl;
	}

	mot = new MotionMessage();
	mot->add_parameter(0.0f);
	mot->add_parameter(0.0f);
	mot->add_parameter(0.0f);
	mot->add_parameter(0.0f);
	mot->add_parameter(0.0f);

	cout << "Behavior Controller Initialized" << endl;
}

void BehaviorController::run() {
	MessageBuffer* sub_buf = Subscriber::getBuffer();
	while (sub_buf->size() > 0) {
		process_messages();
	}
	SleepMs(30);

	boost::thread::yield();
}

void BehaviorController::process_messages() {
	MessageBuffer* sub_buf = Subscriber::getBuffer();
	google::protobuf::Message* cur = sub_buf->remove_head();
	//cout<<"ProcessMessages"<<endl;
	if (cur != NULL) {
		//cout << "ProcessMessages found message" << endl;
		if (cur->GetTypeName() == "BallTrackMessage") {

			BallTrackMessage * bmsg = static_cast<BallTrackMessage*> (cur);

			float overshootfix = bmsg->radius();
			overshootfix = 2 * (0.4f - overshootfix);
			//cout << "Overshoot Value: " << overshootfix << endl;
			float cx = bmsg->cx();
			float cy = bmsg->cy();
			//cout << "I want the freaking head to move towards (cx,cy):" << 0.9f * (cx) << " " << -0.9f * (cy) << endl;

			if (fabs(cx) > 0.015 || fabs(cy) > 0.015) {
				//Sending command to motion
				mot->set_topic("motion");
				mot->set_command("changeHead");
				mot->set_parameter(0, 0.9f * overshootfix * (cx));
				mot->set_parameter(1, -0.9f * overshootfix * (cy));
				Publisher::publish( mot);
				cout << "I realy want the freaking head to move towards (cx,cy):" << 0.9f * (cx) << " " << -0.9f * (cy) << endl;
			}
			cout << "Ball Found ole " << endl;
			//cout << "At Cx " << (float) memory->getData("kouretes/Ball/cx") << " Cy: " << (float) memory->getData("kouretes/Ball/cy") << endl;

		}

		if (cur->GetTypeName() == "HeadJointSensorsMessage") {
			//cout << "Received HeadJointSensorMessage" << endl;
			HeadJointSensorsMessage * hjsm = static_cast<HeadJointSensorsMessage*> (cur);
			//cout << "My headjoint  :" << hjsm->sensordata(0).sensorname() << " has value  " << hjsm->sensordata(0).sensorvalue() << endl;
			SensorPair HeapYaw = hjsm->sensordata(0);
			SensorPair HeapPitch = hjsm->sensordata(1);

			float X, Y, theta;
			//Calculate approach
			X = 0; Y= 0; theta = 0;
			//Check max values !
			if (fabs(HeapYaw.sensorvalue()) > 0.06) {
				theta = HeapYaw.sensorvalue() * 0.5*(0.6 -fabs(HeapPitch.sensorvalue()));
			}
			if (fabs(HeapPitch.sensorvalue()) < 0.487) {
				X = 0.6 * (0.5 - HeapPitch.sensorvalue());
				Y = HeapYaw.sensorvalue()*0.6 * (1.4 - fabs(HeapYaw.sensorvalue()));
			}

			mot->set_topic("motion");
			mot->set_command("setWalkTargetVelocity");
			mot->set_parameter(0,X);
			mot->set_parameter(1,Y);
			mot->set_parameter(2,theta);
			mot->set_parameter(3,1.3-fabs(HeapPitch.sensorvalue()));
			//cout << "SEnding Demo Commands  setWalkTargetVelocity " << endl;
			Publisher::publish(mot);
		}
		delete cur;
	}
}
