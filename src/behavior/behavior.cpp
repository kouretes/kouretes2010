#include "behavior.h"
#include "messages/motion.pb.h"
#include "messages/VisionObservations.pb.h"
BehaviorController::BehaviorController(AL::ALPtr<AL::ALBroker> pbroker, MessageQueue* mq) :
	Thread(false), Subscriber("BehaviorController"), Publisher("BehaviorController") {
	//cout << "beh Constructor" << endl;
	if (mq != 0) {
		//cout << "fooooooooooooooooooo" << endl;
		mq->add_publisher(this);
		mq->add_subscriber(this);
		mq->subscribe("vision", this, 0);
	}
	try {
		memory = pbroker->getMemoryProxy();
	} catch (AL::ALError& e) {
		cout << "Error in getting memory proxy" << std::endl;
	}

	mot = new MotionCommand();
	//TestMessage* tt = new TestMessage();
	//tt->set_topic("localization");
	//Publisher::publish(tt);
	cout << "Behavior Controller Initialized" << endl;
}

void BehaviorController::run() {
	MessageBuffer* sub_buf = Subscriber::getBuffer();
	if (sub_buf->size() > 0)
        process_messages();
	//else
	//	{
	//cout << "create traffic" << endl;
	//	TestMessage* tt = new TestMessage();
	//	tt->set_topic("localization");
	//Publisher::publish(tt);
	//delete tt;
	//}
	//cout << "behavior " << endl;


	//while (true) {

		/*if ((float) memory->getData("kouretes/Ball/found") == 1) {

			float overshootfix = (float) memory->getData("kouretes/Ball/radius");
			overshootfix = 0.8 * (0.4f - overshootfix);
			cout << "Overshoot Value: " << overshootfix << endl;
			float cx = memory->getData("kouretes/Ball/cx");
			float cy = memory->getData("kouretes/Ball/cy");
			if (abs(cx) > 0.015 && abs(cy) > 0.015) {
				//Sending command to motion
				mot->set_topic("motion");
				mot->set_command("changeHead");
				mot->add_parameter(0.85f * overshootfix * (cx));
				mot->add_parameter(-1.1f * overshootfix * (cy));
				Publisher::publish(mot);
			}

			cout << "Ball Found ole " << endl;
			cout << "At Cx " << (float) memory->getData("kouretes/Ball/cx") << " Cy: " << (float) memory->getData("kouretes/Ball/cy") << endl;
		}*/

	//}
	boost::thread::yield();

}

void BehaviorController::process_messages() {
	MessageBuffer* sub_buf = Subscriber::getBuffer();
    google::protobuf::Message*  cur = sub_buf->remove_head();
    //cout<<"ProcessMessages"<<endl;
    while(cur!=NULL)
    {
        cout<<"ProcessMessages found message"<<endl;
        if(cur->GetTypeName() == "BallTrackMessage")
        {

            BallTrackMessage * bmsg= static_cast<BallTrackMessage*>(cur);

            float overshootfix = bmsg->radius();
            overshootfix = 0.8 * (0.4f - overshootfix);
			cout << "Overshoot Value: " << overshootfix << endl;
			float cx = bmsg->cx();
			float cy = bmsg->cy();
			//if (abs(cx) > 0.015 && abs(cy) > 0.015) {
				//Sending command to motion
				mot->set_topic("motion");
				mot->set_command("changeHead");
				mot->add_parameter(0.85f * overshootfix * (cx));
				mot->add_parameter(-1.1f * overshootfix * (cy));
				//Publisher::publish(mot);
				 //if (abs(cx) > 0.015 && abs(cy) > 0.015) {
				     cout<<"Track Ball changeHead"<<endl;
				     memory->insertData("kouretes/HeadCommand", AL::ALValue("changeHead"));
                     memory->insertData("kouretes/HeadParam1", 0.85f * overshootfix * (cx)); // change in Head Yaw
                     memory->insertData("kouretes/HeadParam2", -1.1f * overshootfix * (cy)); // change in Head Pitch
                //}
			//}

			cout << "Ball Found ole " << endl;
			//cout << "At Cx " << (float) memory->getData("kouretes/Ball/cx") << " Cy: " << (float) memory->getData("kouretes/Ball/cy") << endl;
			delete bmsg;
        }

        cur=sub_buf->remove_head();

    }

	//TestMessage* tm = (TestMessage*) sub_buf->remove_head();
	//cout << "I received a message from " << tm->publisher() << " with counter " << tm->counter() << endl;
	//tm->set_counter(tm->counter() + 1);
	//tm->set_topic("motion");
	//publish(tm);
}
