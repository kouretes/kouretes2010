#include "sensors.h"


SensorController::SensorController(AL::ALPtr<AL::ALBroker> pbroker, MessageQueue* mq) :
	Thread(false), Publisher("SensorController")//,Subscriber("SensorsController")
{
	try {
		memory = pbroker->getMemoryProxy();
	} catch (AL::ALError& e) {
		cout << "Error in getting memory proxy" << std::endl;
	}

	if (mq != 0) {
		mq->add_publisher(this);
	}
	initialisation();
	rtm.start();

	cout << "Sensor Controller Initialized" << endl;
}

void SensorController::run() {
	//cout << "REading VAlues" << endl;

	unsigned int counter = 0;
	Values = memory->getListData(devicesInChains["Head"]);

	rtm.stop();
	timediff = rtm.diffNs();
	for (unsigned int i = 0; i < devicesInChains["Head"].size(); i++) {
		HJSM.mutable_sensordata(i)->set_sensorname(devicesNames["Head"][i]);
		HJSM.mutable_sensordata(i)->set_sensorvalue(Values[i]);
		//cout << " VAlue " << Values[i] << "Message data "<< HJSM.sensordata(i).sensorvalue() << endl;

		devicesValues[counter] = Values[i];

		HJSM.mutable_sensordata(i)->set_sensorvaluediff(Values[i] - devicesValues[counter]);
		HJSM.mutable_sensordata(i)->set_sensortimediff(timediff);
		//TODO ADD VALUEDIFF, TIMEDIFF
		counter++;
	}


	rtm.start();
//	Values = memory->getListData(devicesInChains["Body"]);
//	for (unsigned int i = 0; i < devicesInChains["Body"].size(); i++) {
//		BJSM.mutable_sensordata(i)->set_sensorname(devicesNames["Body"][i]);
//		BJSM.mutable_sensordata(i)->set_sensorvalue(Values[i]);
//		//TODO ADD VALUEDIFF, TIMEDIFF
//
//		BJSM.mutable_sensordata(i)->set_sensorvaluediff(Values[i] - devicesValues[counter]);
//		BJSM.mutable_sensordata(i)->set_sensortimediff(timediff);
//		devicesValues[counter] = Values[i];
//		counter++;
//	}
//
//	Values = memory->getListData(devicesInChains["Inertial"]);
//
//	for (unsigned int i = 0; i < devicesInChains["Inertial"].size(); i++) {
//		ISM.mutable_sensordata(i)->set_sensorname(devicesNames["Inertial"][i]);
//		ISM.mutable_sensordata(i)->set_sensorvalue(Values[i]);
//		//TODO ADD VALUEDIFF, TIMEDIFF
//
//		ISM.mutable_sensordata(i)->set_sensorvaluediff(Values[i] - devicesValues[counter]);
//		ISM.mutable_sensordata(i)->set_sensortimediff(timediff);
//		devicesValues[counter] = Values[i];
//		counter++;
//	}
	publish(&HJSM);
//	publish(&BJSM);
//	publish(&ISM);
	//cout << "Sended Values" << endl;
	SleepMs(300);

	//boost::thread::yield();

}

void SensorController::initialisation() {
	HJSM.set_topic("sensors"); //HeadJointSensorsMessage
	HJSM.set_timeout(250);

	BJSM.set_topic("sensors"); //BodyJointSensorsMessage
	BJSM.set_timeout(250);

	ISM.set_topic("sensors"); //InertialSensorsMessage
	ISM.set_timeout(250);

	FSM.set_topic("sensors"); //FSRSensorsMessage
	FSM.set_timeout(250);

	USSM.set_topic("sensors"); //UltaSoundSensorsMessage
	USSM.set_timeout(250);


	devicesInChains["Head"].push_back(std::string("Device/SubDeviceList/HeadYaw/Position/Sensor/Value"));
	devicesInChains["Head"].push_back(std::string("Device/SubDeviceList/HeadPitch/Position/Sensor/Value"));

	for (unsigned int i = 0; i < devicesInChains["Head"].size(); i++)
		HJSM.add_sensordata();

	devicesInChains["LeftArm"].push_back(std::string("Device/SubDeviceList/LShoulderPitch/Position/Sensor/Value"));
	devicesInChains["LeftArm"].push_back(std::string("Device/SubDeviceList/LShoulderRoll/Position/Sensor/Value"));
	devicesInChains["LeftArm"].push_back(std::string("Device/SubDeviceList/LElbowYaw/Position/Sensor/Value"));
	devicesInChains["LeftArm"].push_back(std::string("Device/SubDeviceList/LElbowRoll/Position/Sensor/Value"));
	devicesInChains["LeftLeg"].push_back(std::string("Device/SubDeviceList/LHipYawPitch/Position/Sensor/Value"));
	devicesInChains["LeftLeg"].push_back(std::string("Device/SubDeviceList/LHipPitch/Position/Sensor/Value"));
	devicesInChains["LeftLeg"].push_back(std::string("Device/SubDeviceList/LHipRoll/Position/Sensor/Value"));
	devicesInChains["LeftLeg"].push_back(std::string("Device/SubDeviceList/LKneePitch/Position/Sensor/Value"));
	devicesInChains["LeftLeg"].push_back(std::string("Device/SubDeviceList/LAnklePitch/Position/Sensor/Value"));
	devicesInChains["LeftLeg"].push_back(std::string("Device/SubDeviceList/LAnkleRoll/Position/Sensor/Value"));
	devicesInChains["RightLeg"].push_back(std::string("Device/SubDeviceList/RHipYawPitch/Position/Sensor/Value"));
	devicesInChains["RightLeg"].push_back(std::string("Device/SubDeviceList/RHipPitch/Position/Sensor/Value"));
	devicesInChains["RightLeg"].push_back(std::string("Device/SubDeviceList/RHipRoll/Position/Sensor/Value"));
	devicesInChains["RightLeg"].push_back(std::string("Device/SubDeviceList/RKneePitch/Position/Sensor/Value"));
	devicesInChains["RightLeg"].push_back(std::string("Device/SubDeviceList/RAnklePitch/Position/Sensor/Value"));
	devicesInChains["RightLeg"].push_back(std::string("Device/SubDeviceList/RAnkleRoll/Position/Sensor/Value"));
	devicesInChains["RightArm"].push_back(std::string("Device/SubDeviceList/RShoulderPitch/Position/Sensor/Value"));
	devicesInChains["RightArm"].push_back(std::string("Device/SubDeviceList/RShoulderRoll/Position/Sensor/Value"));
	devicesInChains["RightArm"].push_back(std::string("Device/SubDeviceList/RElbowYaw/Position/Sensor/Value"));
	devicesInChains["RightArm"].push_back(std::string("Device/SubDeviceList/RElbowRoll/Position/Sensor/Value"));

	//devicesInChains["Body"].push_back(std::string("Device/SubDeviceList/HeadYaw/Position/Sensor/Value"));
	//devicesInChains["Body"].push_back(std::string("Device/SubDeviceList/HeadPitch/Position/Sensor/Value"));
	devicesInChains["Body"].push_back(std::string("Device/SubDeviceList/LShoulderPitch/Position/Sensor/Value"));
	devicesInChains["Body"].push_back(std::string("Device/SubDeviceList/LShoulderRoll/Position/Sensor/Value"));
	devicesInChains["Body"].push_back(std::string("Device/SubDeviceList/LElbowYaw/Position/Sensor/Value"));
	devicesInChains["Body"].push_back(std::string("Device/SubDeviceList/LElbowRoll/Position/Sensor/Value"));
	devicesInChains["Body"].push_back(std::string("Device/SubDeviceList/LHipYawPitch/Position/Sensor/Value"));
	devicesInChains["Body"].push_back(std::string("Device/SubDeviceList/LHipPitch/Position/Sensor/Value"));
	devicesInChains["Body"].push_back(std::string("Device/SubDeviceList/LHipRoll/Position/Sensor/Value"));
	devicesInChains["Body"].push_back(std::string("Device/SubDeviceList/LKneePitch/Position/Sensor/Value"));
	devicesInChains["Body"].push_back(std::string("Device/SubDeviceList/LAnklePitch/Position/Sensor/Value"));
	devicesInChains["Body"].push_back(std::string("Device/SubDeviceList/LAnkleRoll/Position/Sensor/Value"));
	devicesInChains["Body"].push_back(std::string("Device/SubDeviceList/RHipYawPitch/Position/Sensor/Value"));
	devicesInChains["Body"].push_back(std::string("Device/SubDeviceList/RHipPitch/Position/Sensor/Value"));
	devicesInChains["Body"].push_back(std::string("Device/SubDeviceList/RHipRoll/Position/Sensor/Value"));
	devicesInChains["Body"].push_back(std::string("Device/SubDeviceList/RKneePitch/Position/Sensor/Value"));
	devicesInChains["Body"].push_back(std::string("Device/SubDeviceList/RAnklePitch/Position/Sensor/Value"));
	devicesInChains["Body"].push_back(std::string("Device/SubDeviceList/RAnkleRoll/Position/Sensor/Value"));
	devicesInChains["Body"].push_back(std::string("Device/SubDeviceList/RShoulderPitch/Position/Sensor/Value"));
	devicesInChains["Body"].push_back(std::string("Device/SubDeviceList/RShoulderRoll/Position/Sensor/Value"));
	devicesInChains["Body"].push_back(std::string("Device/SubDeviceList/RElbowYaw/Position/Sensor/Value"));
	devicesInChains["Body"].push_back(std::string("Device/SubDeviceList/RElbowRoll/Position/Sensor/Value"));

	for (unsigned int i = 0; i < devicesInChains["Body"].size(); i++)
		BJSM.add_sensordata();

	devicesInChains["Inertial"].push_back(std::string("Device/SubDeviceList/InertialSensor/AccX/Sensor/Value"));
	devicesInChains["Inertial"].push_back(std::string("Device/SubDeviceList/InertialSensor/AccY/Sensor/Value"));
	devicesInChains["Inertial"].push_back(std::string("Device/SubDeviceList/InertialSensor/AccZ/Sensor/Value"));
	devicesInChains["Inertial"].push_back(std::string("Device/SubDeviceList/InertialSensor/GyrX/Sensor/Value"));
	devicesInChains["Inertial"].push_back(std::string("Device/SubDeviceList/InertialSensor/GyrY/Sensor/Value"));
	devicesInChains["Inertial"].push_back(std::string("Device/SubDeviceList/InertialSensor/AngleX/Sensor/Value"));
	devicesInChains["Inertial"].push_back(std::string("Device/SubDeviceList/InertialSensor/AngleY/Sensor/Value"));
	for (unsigned int i = 0; i < devicesInChains["Inertial"].size(); i++)
		ISM.add_sensordata();

	//TODO This is not real FSR
	devicesInChains["FSR"].push_back(std::string("Device/SubDeviceList/LFoot/FSR/CenterOfPressure/X/Sensor/Value"));
	devicesInChains["FSR"].push_back(std::string("Device/SubDeviceList/LFoot/FSR/CenterOfPressure/Y/Sensor/Value"));
	devicesInChains["FSR"].push_back(std::string("Device/SubDeviceList/LFoot/FSR/TotalWeight/Sensor/Value"));
	devicesInChains["FSR"].push_back(std::string("Device/SubDeviceList/RFoot/FSR/CenterOfPressure/X/Sensor/Value"));
	devicesInChains["FSR"].push_back(std::string("Device/SubDeviceList/RFoot/FSR/CenterOfPressure/Y/Sensor/Value"));
	devicesInChains["FSR"].push_back(std::string("Device/SubDeviceList/RFoot/FSR/TotalWeight/Sensor/Value"));

	for (unsigned int i = 0; i < devicesInChains["FSR"].size(); i++)
		FSM.add_sensordata();

	//TODO add Ultra Sound sensors
	//UltaSoundSensorsMessage USSM;

	///SMALL STRING NAMES


	devicesNames["Head"].push_back(std::string("HeadYaw"));
	devicesNames["Head"].push_back(std::string("HeadPitch"));
	devicesNames["LeftArm"].push_back(std::string("LShoulderPitch"));
	devicesNames["LeftArm"].push_back(std::string("LShoulderRoll"));
	devicesNames["LeftArm"].push_back(std::string("LElbowYaw"));
	devicesNames["LeftArm"].push_back(std::string("LElbowRoll"));
	devicesNames["LeftLeg"].push_back(std::string("LHipYawPitch"));
	devicesNames["LeftLeg"].push_back(std::string("LHipPitch"));
	devicesNames["LeftLeg"].push_back(std::string("LHipRoll"));
	devicesNames["LeftLeg"].push_back(std::string("LKneePitch"));
	devicesNames["LeftLeg"].push_back(std::string("LAnklePitch"));
	devicesNames["LeftLeg"].push_back(std::string("LAnkleRoll"));
	devicesNames["RightLeg"].push_back(std::string("RHipYawPitch"));
	devicesNames["RightLeg"].push_back(std::string("RHipPitch"));
	devicesNames["RightLeg"].push_back(std::string("RHipRoll"));
	devicesNames["RightLeg"].push_back(std::string("RKneePitch"));
	devicesNames["RightLeg"].push_back(std::string("RAnklePitch"));
	devicesNames["RightLeg"].push_back(std::string("RAnkleRoll"));
	devicesNames["RightArm"].push_back(std::string("RShoulderPitch"));
	devicesNames["RightArm"].push_back(std::string("RShoulderRoll"));
	devicesNames["RightArm"].push_back(std::string("RElbowYaw"));
	devicesNames["RightArm"].push_back(std::string("RElbowRoll"));

	//devicesNames["Body"].push_back(std::string("HeadYaw"));
	//devicesNames["Body"].push_back(std::string("HeadPitch"));
	devicesNames["Body"].push_back(std::string("LShoulderPitch"));
	devicesNames["Body"].push_back(std::string("LShoulderRoll"));
	devicesNames["Body"].push_back(std::string("LElbowYaw"));
	devicesNames["Body"].push_back(std::string("LElbowRoll"));
	devicesNames["Body"].push_back(std::string("LHipYawPitch"));
	devicesNames["Body"].push_back(std::string("LHipPitch"));
	devicesNames["Body"].push_back(std::string("LHipRoll"));
	devicesNames["Body"].push_back(std::string("LKneePitch"));
	devicesNames["Body"].push_back(std::string("LAnklePitch"));
	devicesNames["Body"].push_back(std::string("LAnkleRoll"));
	devicesNames["Body"].push_back(std::string("RHipYawPitch"));
	devicesNames["Body"].push_back(std::string("RHipPitch"));
	devicesNames["Body"].push_back(std::string("RHipRoll"));
	devicesNames["Body"].push_back(std::string("RKneePitch"));
	devicesNames["Body"].push_back(std::string("RAnklePitch"));
	devicesNames["Body"].push_back(std::string("RAnkleRoll"));
	devicesNames["Body"].push_back(std::string("RShoulderPitch"));
	devicesNames["Body"].push_back(std::string("RShoulderRoll"));
	devicesNames["Body"].push_back(std::string("RElbowYaw"));
	devicesNames["Body"].push_back(std::string("RElbowRoll"));

	devicesNames["Inertial"].push_back(std::string("AccX"));
	devicesNames["Inertial"].push_back(std::string("AccY"));
	devicesNames["Inertial"].push_back(std::string("AccZ"));
	devicesNames["Inertial"].push_back(std::string("GyrX"));
	devicesNames["Inertial"].push_back(std::string("GyrY"));
	devicesNames["Inertial"].push_back(std::string("AngleX"));
	devicesNames["Inertial"].push_back(std::string("AngleY"));

	devicesNames["FSR"].push_back(std::string("LFoot_FSR_CenterOfPressure_X"));
	devicesNames["FSR"].push_back(std::string("LFoot_FSR_CenterOfPressure_Y"));
	devicesNames["FSR"].push_back(std::string("LFoot_FSR_TotalWeight"));
	devicesNames["FSR"].push_back(std::string("RFoot_FSR_CenterOfPressure_X"));
	devicesNames["FSR"].push_back(std::string("RFoot_FSR_CenterOfPressure_Y"));
	devicesNames["FSR"].push_back(std::string("RFoot_FSR_TotalWeight"));

	memset(devicesValues, 0, sizeof devicesValues);

}

void SensorController::process_messages() {
	cout << "Sensor process messages" << endl;
	//	MessageBuffer* sub_buf = Subscriber::getBuffer();
	//	TestMessage* tm = (TestMessage*) sub_buf->remove_head();
	//	cout << "I received a message from " << tm->publisher() << " with counter "
	//			<< tm->counter() << endl;
	//	tm->set_counter(tm->counter() + 1);
	//	tm->set_topic("behavior"));
	//	publish(tm);
}
