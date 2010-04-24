#include "motion_controller.h"



void MotionController::run() {
	//std::cout << " MotionController START" << std::endl;
	//mglrun();  /* Uncomment of action! */
	//std::cout << " MotionController END" << std::endl;
	return;
}




void MotionController::mglrun(){

	if (robotDown) {
		cout << "Will stand up in 2 seconds ... " << std::endl; 
		sleep(2);
		motion->setStiffnesses("Body", 0.9);
		ALstandUp();
		sleep(10);
		return;
	}

	float AccZvalue = memory->getData("Device/SubDeviceList/InertialSensor/AccZ/Sensor/Value");

	if ( !robotDown && (AccZvalue < 1.0) ) {
		cout << "Robot falling: Stiffness off" << std::endl;
		motion->setStiffnesses("Body", 0.0);
		robotDown = true; 
		if (walkPID != 0) {
			motion->killWalk();
			walkPID = 0; 
		}
		return;
	}

	if ( !robotDown && (walkPID==0) ) {
		//walkPid = motion->post.setWalkTargetVelocity(0.5, 0.5, 0.5, 0.5);
		walkPID = motion->post.walkTo(1.0, 0.0, 0.0);

		std::cout << "   Walk ID: " << walkPID << std::endl;
	}
	else
		counter++;
	//sleep(1);
	//std::cout << " i am running for " << counter << endl;	

	if ( !robotDown && !motion->isRunning(walkPID) && !motion->walkIsActive() ) {
		walkPID = 0;
		std::cout << "   Walk finished " << counter << std::endl;
		sleep(10);
		counter = 0;
	}

	//sleep(5);
	//motion->post.killWalk();
	//sleep(5);
	

	return;
}



void MotionController::ALstandUp(){

	float AngleY = memory->getData("Device/SubDeviceList/InertialSensor/AngleY/Sensor/Value");
        
        if ( (AngleY > 1.0) || (AngleY < -1.0) ) {
		ALstandUpCross();	
	}
	
	sleep(3);

	AngleY = memory->getData("Device/SubDeviceList/InertialSensor/AngleY/Sensor/Value");
        
        if (AngleY > 1.0) {
		ALstandUpBack();
	}
	else if (AngleY < -1.0) {
		ALstandUpFront();
	}
	return;
}



void MotionController::ALstandUpFront(){

AL::ALValue jointCodes, angles, times;
jointCodes.arraySetSize(21);
angles.arraySetSize(21);
times.arraySetSize(21);

jointCodes[0] = "HeadPitch";
angles[0].arraySetSize(6);
times[0].arraySetSize(6);
angles[0][0] = -0.57596f;
times[0][0] = 1.40000f;
angles[0][1] = 0.00000f;
times[0][1] = 2.40000f;
angles[0][2] = -0.34907f;
times[0][2] = 3.70000f;
angles[0][3] = -0.48869f;
times[0][3] = 5.20000f;
angles[0][4] = 0.00000f;
times[0][4] = 6.20000f;
angles[0][5] = 0.27925f;
times[0][5] = 8.40000f;

jointCodes[1] = "LAnklePitch";
angles[1].arraySetSize(8);
times[1].arraySetSize(8);
angles[1][0] = -1.13446f;
times[1][0] = 1.40000f;
angles[1][1] = -1.13446f;
times[1][1] = 2.40000f;
angles[1][2] = -0.78365f;
times[1][2] = 3.70000f;
angles[1][3] = 0.08727f;
times[1][3] = 4.40000f;
angles[1][4] = -0.31241f;
times[1][4] = 5.20000f;
angles[1][5] = -0.71558f;
times[1][5] = 6.20000f;
angles[1][6] = -1.04720f;
times[1][6] = 7.40000f;
angles[1][7] = -0.17453f;
times[1][7] = 8.40000f;

jointCodes[2] = "LAnkleRoll";
angles[2].arraySetSize(8);
times[2].arraySetSize(8);
angles[2][0] = 0.00000f;
times[2][0] = 1.40000f;
angles[2][1] = 0.00000f;
times[2][1] = 2.40000f;
angles[2][2] = -0.68068f;
times[2][2] = 3.70000f;
angles[2][3] = -0.55501f;
times[2][3] = 4.40000f;
angles[2][4] = -0.29671f;
times[2][4] = 5.20000f;
angles[2][5] = -0.10472f;
times[2][5] = 6.20000f;
angles[2][6] = 0.00000f;
times[2][6] = 7.40000f;
angles[2][7] = 0.00000f;
times[2][7] = 8.40000f;

jointCodes[3] = "LElbowRoll";
angles[3].arraySetSize(8);
times[3].arraySetSize(8);
angles[3][0] = 0.00000f;
times[3][0] = 1.40000f;
angles[3][1] = -0.61087f;
times[3][1] = 2.40000f;
angles[3][2] = -1.65806f;
times[3][2] = 3.70000f;
angles[3][3] = -0.13963f;
times[3][3] = 4.40000f;
angles[3][4] = -0.71558f;
times[3][4] = 5.20000f;
angles[3][5] = -1.29154f;
times[3][5] = 6.20000f;
angles[3][6] = -1.39626f;
times[3][6] = 7.40000f;
angles[3][7] = -1.25664f;
times[3][7] = 8.40000f;

jointCodes[4] = "LElbowYaw";
angles[4].arraySetSize(8);
times[4].arraySetSize(8);
angles[4][0] = -1.57080f;
times[4][0] = 1.40000f;
angles[4][1] = -1.57080f;
times[4][1] = 2.40000f;
angles[4][2] = -1.57080f;
times[4][2] = 3.70000f;
angles[4][3] = -1.57080f;
times[4][3] = 4.40000f;
angles[4][4] = -0.24435f;
times[4][4] = 5.20000f;
angles[4][5] = -0.92502f;
times[4][5] = 6.20000f;
angles[4][6] = -1.57080f;
times[4][6] = 7.40000f;
angles[4][7] = -1.23918f;
times[4][7] = 8.40000f;

jointCodes[5] = "LHipPitch";
angles[5].arraySetSize(8);
times[5].arraySetSize(8);
angles[5][0] = 0.00000f;
times[5][0] = 1.40000f;
angles[5][1] = -1.57080f;
times[5][1] = 2.40000f;
angles[5][2] = -1.57080f;
times[5][2] = 3.70000f;
angles[5][3] = -1.57080f;
times[5][3] = 4.40000f;
angles[5][4] = -1.57080f;
times[5][4] = 5.20000f;
angles[5][5] = -1.06989f;
times[5][5] = 6.20000f;
angles[5][6] = -1.04720f;
times[5][6] = 7.40000f;
angles[5][7] = -0.17453f;
times[5][7] = 8.40000f;

jointCodes[6] = "LHipRoll";
angles[6].arraySetSize(8);
times[6].arraySetSize(8);
angles[6][0] = 0.00000f;
times[6][0] = 1.40000f;
angles[6][1] = 0.00000f;
times[6][1] = 2.40000f;
angles[6][2] = 0.00000f;
times[6][2] = 3.70000f;
angles[6][3] = 0.00000f;
times[6][3] = 4.40000f;
angles[6][4] = 0.08727f;
times[6][4] = 5.20000f;
angles[6][5] = 0.10472f;
times[6][5] = 6.20000f;
angles[6][6] = -0.01047f;
times[6][6] = 7.40000f;
angles[6][7] = -0.01047f;
times[6][7] = 8.40000f;

jointCodes[7] = "LHipYawPitch";
angles[7].arraySetSize(8);
times[7].arraySetSize(8);
angles[7][0] = 0.00000f;
times[7][0] = 1.40000f;
angles[7][1] = 0.00000f;
times[7][1] = 2.40000f;
angles[7][2] = -0.87266f;
times[7][2] = 3.70000f;
angles[7][3] = -0.87266f;
times[7][3] = 4.40000f;
angles[7][4] = -0.96517f;
times[7][4] = 5.20000f;
angles[7][5] = -0.78540f;
times[7][5] = 6.20000f;
angles[7][6] = 0.00000f;
times[7][6] = 7.40000f;
angles[7][7] = 0.00000f;
times[7][7] = 8.40000f;

jointCodes[8] = "LKneePitch";
angles[8].arraySetSize(8);
times[8].arraySetSize(8);
angles[8][0] = 2.09440f;
times[8][0] = 1.40000f;
angles[8][1] = 2.09440f;
times[8][1] = 2.40000f;
angles[8][2] = 1.04720f;
times[8][2] = 3.70000f;
angles[8][3] = 1.01229f;
times[8][3] = 4.40000f;
angles[8][4] = 2.15548f;
times[8][4] = 5.20000f;
angles[8][5] = 2.16421f;
times[8][5] = 6.20000f;
angles[8][6] = 2.09440f;
times[8][6] = 7.40000f;
angles[8][7] = 0.34907f;
times[8][7] = 8.40000f;

jointCodes[9] = "LShoulderPitch";
angles[9].arraySetSize(8);
times[9].arraySetSize(8);
angles[9][0] = -1.57080f;
times[9][0] = 1.40000f;
angles[9][1] = -0.87266f;
times[9][1] = 2.40000f;
angles[9][2] = -0.17453f;
times[9][2] = 3.70000f;
angles[9][3] = 0.00000f;
times[9][3] = 4.40000f;
angles[9][4] = 0.61087f;
times[9][4] = 5.20000f;
angles[9][5] = 1.11701f;
times[9][5] = 6.20000f;
angles[9][6] = 1.62316f;
times[9][6] = 7.40000f;
angles[9][7] = 1.83260f;
times[9][7] = 8.40000f;

jointCodes[10] = "LShoulderRoll";
angles[10].arraySetSize(8);
times[10].arraySetSize(8);
angles[10][0] = 0.00000f;
times[10][0] = 1.40000f;
angles[10][1] = 0.00000f;
times[10][1] = 2.40000f;
angles[10][2] = 0.00000f;
times[10][2] = 3.70000f;
angles[10][3] = 0.00000f;
times[10][3] = 4.40000f;
angles[10][4] = 0.03491f;
times[10][4] = 5.20000f;
angles[10][5] = 0.13090f;
times[10][5] = 6.20000f;
angles[10][6] = 0.17453f;
times[10][6] = 7.40000f;
angles[10][7] = 0.19199f;
times[10][7] = 8.40000f;

jointCodes[11] = "RAnklePitch";
angles[11].arraySetSize(8);
times[11].arraySetSize(8);
angles[11][0] = -1.13446f;
times[11][0] = 1.40000f;
angles[11][1] = -1.13446f;
times[11][1] = 2.40000f;
angles[11][2] = -0.78365f;
times[11][2] = 3.70000f;
angles[11][3] = 0.08727f;
times[11][3] = 4.40000f;
angles[11][4] = -0.31241f;
times[11][4] = 5.20000f;
angles[11][5] = -0.71558f;
times[11][5] = 6.20000f;
angles[11][6] = -1.04720f;
times[11][6] = 7.40000f;
angles[11][7] = -0.17453f;
times[11][7] = 8.40000f;

jointCodes[12] = "RAnkleRoll";
angles[12].arraySetSize(8);
times[12].arraySetSize(8);
angles[12][0] = 0.00000f;
times[12][0] = 1.40000f;
angles[12][1] = 0.00000f;
times[12][1] = 2.40000f;
angles[12][2] = 0.68068f;
times[12][2] = 3.70000f;
angles[12][3] = 0.55501f;
times[12][3] = 4.40000f;
angles[12][4] = 0.29671f;
times[12][4] = 5.20000f;
angles[12][5] = 0.10472f;
times[12][5] = 6.20000f;
angles[12][6] = 0.00000f;
times[12][6] = 7.40000f;
angles[12][7] = 0.00000f;
times[12][7] = 8.40000f;

jointCodes[13] = "RElbowRoll";
angles[13].arraySetSize(8);
times[13].arraySetSize(8);
angles[13][0] = 0.00000f;
times[13][0] = 1.40000f;
angles[13][1] = 0.61087f;
times[13][1] = 2.40000f;
angles[13][2] = 1.65806f;
times[13][2] = 3.70000f;
angles[13][3] = 0.13963f;
times[13][3] = 4.40000f;
angles[13][4] = 0.71558f;
times[13][4] = 5.20000f;
angles[13][5] = 1.29154f;
times[13][5] = 6.20000f;
angles[13][6] = 1.39626f;
times[13][6] = 7.40000f;
angles[13][7] = 1.25664f;
times[13][7] = 8.40000f;

jointCodes[14] = "RElbowYaw";
angles[14].arraySetSize(8);
times[14].arraySetSize(8);
angles[14][0] = 1.57080f;
times[14][0] = 1.40000f;
angles[14][1] = 1.57080f;
times[14][1] = 2.40000f;
angles[14][2] = 1.57080f;
times[14][2] = 3.70000f;
angles[14][3] = 1.57080f;
times[14][3] = 4.40000f;
angles[14][4] = 0.24435f;
times[14][4] = 5.20000f;
angles[14][5] = 0.92502f;
times[14][5] = 6.20000f;
angles[14][6] = 1.57080f;
times[14][6] = 7.40000f;
angles[14][7] = 1.23918f;
times[14][7] = 8.40000f;

jointCodes[15] = "RHipPitch";
angles[15].arraySetSize(8);
times[15].arraySetSize(8);
angles[15][0] = 0.00000f;
times[15][0] = 1.40000f;
angles[15][1] = -1.57080f;
times[15][1] = 2.40000f;
angles[15][2] = -1.57080f;
times[15][2] = 3.70000f;
angles[15][3] = -1.57080f;
times[15][3] = 4.40000f;
angles[15][4] = -1.57080f;
times[15][4] = 5.20000f;
angles[15][5] = -1.06989f;
times[15][5] = 6.20000f;
angles[15][6] = -1.04720f;
times[15][6] = 7.40000f;
angles[15][7] = -0.17453f;
times[15][7] = 8.40000f;

jointCodes[16] = "RHipRoll";
angles[16].arraySetSize(8);
times[16].arraySetSize(8);
angles[16][0] = 0.00000f;
times[16][0] = 1.40000f;
angles[16][1] = 0.00000f;
times[16][1] = 2.40000f;
angles[16][2] = 0.00000f;
times[16][2] = 3.70000f;
angles[16][3] = 0.00000f;
times[16][3] = 4.40000f;
angles[16][4] = -0.08727f;
times[16][4] = 5.20000f;
angles[16][5] = -0.10472f;
times[16][5] = 6.20000f;
angles[16][6] = 0.01047f;
times[16][6] = 7.40000f;
angles[16][7] = 0.01047f;
times[16][7] = 8.40000f;

jointCodes[17] = "RHipYawPitch";
angles[17].arraySetSize(3);
times[17].arraySetSize(3);
angles[17][0] = 0.00000f;
times[17][0] = 1.40000f;
angles[17][1] = 0.00000f;
times[17][1] = 2.40000f;
angles[17][2] = -0.87266f;
times[17][2] = 3.70000f;

jointCodes[18] = "RKneePitch";
angles[18].arraySetSize(8);
times[18].arraySetSize(8);
angles[18][0] = 2.09440f;
times[18][0] = 1.40000f;
angles[18][1] = 2.09440f;
times[18][1] = 2.40000f;
angles[18][2] = 1.04720f;
times[18][2] = 3.70000f;
angles[18][3] = 1.01229f;
times[18][3] = 4.40000f;
angles[18][4] = 2.15548f;
times[18][4] = 5.20000f;
angles[18][5] = 2.16421f;
times[18][5] = 6.20000f;
angles[18][6] = 2.09440f;
times[18][6] = 7.40000f;
angles[18][7] = 0.34907f;
times[18][7] = 8.40000f;

jointCodes[19] = "RShoulderPitch";
angles[19].arraySetSize(8);
times[19].arraySetSize(8);
angles[19][0] = -1.57080f;
times[19][0] = 1.40000f;
angles[19][1] = -0.87266f;
times[19][1] = 2.40000f;
angles[19][2] = -0.17453f;
times[19][2] = 3.70000f;
angles[19][3] = 0.00000f;
times[19][3] = 4.40000f;
angles[19][4] = 0.61087f;
times[19][4] = 5.20000f;
angles[19][5] = 1.11701f;
times[19][5] = 6.20000f;
angles[19][6] = 1.62316f;
times[19][6] = 7.40000f;
angles[19][7] = 1.83260f;
times[19][7] = 8.40000f;

jointCodes[20] = "RShoulderRoll";
angles[20].arraySetSize(8);
times[20].arraySetSize(8);
angles[20][0] = 0.00000f;
times[20][0] = 1.40000f;
angles[20][1] = 0.00000f;
times[20][1] = 2.40000f;
angles[20][2] = 0.00000f;
times[20][2] = 3.70000f;
angles[20][3] = 0.00000f;
times[20][3] = 4.40000f;
angles[20][4] = -0.03491f;
times[20][4] = 5.20000f;
angles[20][5] = -0.13090f;
times[20][5] = 6.20000f;
angles[20][6] = -0.17453f;
times[20][6] = 7.40000f;
angles[20][7] = -0.19199f;
times[20][7] = 8.40000f;

motion->angleInterpolation(jointCodes, angles, times, 1);


}



void MotionController::ALstandUpBack(){

AL::ALValue jointCodes, angles, times;
jointCodes.arraySetSize(22);
angles.arraySetSize(22);
times.arraySetSize(22);

jointCodes[0] = "HeadPitch";
angles[0].arraySetSize(11);
times[0].arraySetSize(11);
angles[0][0] = 0.00000f;
times[0][0] = 0.90000f;
angles[0][1] = 0.00000f;
times[0][1] = 1.90000f;
angles[0][2] = -0.78540f;
times[0][2] = 2.70000f;
angles[0][3] = 0.00000f;
times[0][3] = 3.40000f;
angles[0][4] = 0.34907f;
times[0][4] = 3.90000f;
angles[0][5] = 0.32823f;
times[0][5] = 4.90000f;
angles[0][6] = 0.33129f;
times[0][6] = 5.80000f;
angles[0][7] = 0.37886f;
times[0][7] = 6.80000f;
angles[0][8] = 0.37886f;
times[0][8] = 7.30000f;
angles[0][9] = 0.37886f;
times[0][9] = 8.40000f;
angles[0][10] = 0.27925f;
times[0][10] = 9.40000f;

jointCodes[1] = "HeadYaw";
angles[1].arraySetSize(10);
times[1].arraySetSize(10);
angles[1][0] = 0.00000f;
times[1][0] = 0.90000f;
angles[1][1] = -0.00000f;
times[1][1] = 1.90000f;
angles[1][2] = 0.00000f;
times[1][2] = 2.70000f;
angles[1][3] = 0.00000f;
times[1][3] = 3.40000f;
angles[1][4] = 0.00000f;
times[1][4] = 3.90000f;
angles[1][5] = -0.00744f;
times[1][5] = 4.90000f;
angles[1][6] = -0.01070f;
times[1][6] = 5.80000f;
angles[1][7] = -0.00940f;
times[1][7] = 6.80000f;
angles[1][8] = -0.00940f;
times[1][8] = 7.30000f;
angles[1][9] = -0.00940f;
times[1][9] = 8.40000f;

jointCodes[2] = "LAnklePitch";
angles[2].arraySetSize(11);
times[2].arraySetSize(11);
angles[2][0] = 0.00000f;
times[2][0] = 0.90000f;
angles[2][1] = 0.24435f;
times[2][1] = 1.90000f;
angles[2][2] = 0.24435f;
times[2][2] = 2.70000f;
angles[2][3] = 0.24435f;
times[2][3] = 3.40000f;
angles[2][4] = 0.78540f;
times[2][4] = 3.90000f;
angles[2][5] = -0.57065f;
times[2][5] = 4.90000f;
angles[2][6] = -1.22173f;
times[2][6] = 5.80000f;
angles[2][7] = -1.22173f;
times[2][7] = 6.80000f;
angles[2][8] = -1.22173f;
times[2][8] = 7.30000f;
angles[2][9] = -1.22173f;
times[2][9] = 8.40000f;
angles[2][10] = -0.17453f;
times[2][10] = 9.40000f;

jointCodes[3] = "LAnkleRoll";
angles[3].arraySetSize(11);
times[3].arraySetSize(11);
angles[3][0] = 0.00000f;
times[3][0] = 0.90000f;
angles[3][1] = 0.00000f;
times[3][1] = 1.90000f;
angles[3][2] = 0.00000f;
times[3][2] = 2.70000f;
angles[3][3] = 0.00000f;
times[3][3] = 3.40000f;
angles[3][4] = 0.00000f;
times[3][4] = 3.90000f;
angles[3][5] = -0.39573f;
times[3][5] = 4.90000f;
angles[3][6] = -0.10379f;
times[3][6] = 5.80000f;
angles[3][7] = 0.11810f;
times[3][7] = 6.80000f;
angles[3][8] = 0.08727f;
times[3][8] = 7.30000f;
angles[3][9] = 0.00000f;
times[3][9] = 8.40000f;
angles[3][10] = 0.00000f;
times[3][10] = 9.40000f;

jointCodes[4] = "LElbowRoll";
angles[4].arraySetSize(11);
times[4].arraySetSize(11);
angles[4][0] = 0.00000f;
times[4][0] = 0.90000f;
angles[4][1] = 0.00000f;
times[4][1] = 1.90000f;
angles[4][2] = -1.65806f;
times[4][2] = 2.70000f;
angles[4][3] = -0.69813f;
times[4][3] = 3.40000f;
angles[4][4] = 0.00000f;
times[4][4] = 3.90000f;
angles[4][5] = -0.48869f;
times[4][5] = 4.90000f;
angles[4][6] = -0.82372f;
times[4][6] = 5.80000f;
angles[4][7] = -0.80535f;
times[4][7] = 6.80000f;
angles[4][8] = -0.80535f;
times[4][8] = 7.30000f;
angles[4][9] = -1.13446f;
times[4][9] = 8.40000f;
angles[4][10] = -1.25664f;
times[4][10] = 9.40000f;

jointCodes[5] = "LElbowYaw";
angles[5].arraySetSize(11);
times[5].arraySetSize(11);
angles[5][0] = 0.00000f;
times[5][0] = 0.90000f;
angles[5][1] = 0.15708f;
times[5][1] = 1.90000f;
angles[5][2] = 0.08727f;
times[5][2] = 2.70000f;
angles[5][3] = 0.08727f;
times[5][3] = 3.40000f;
angles[5][4] = 0.08727f;
times[5][4] = 3.90000f;
angles[5][5] = 0.08295f;
times[5][5] = 4.90000f;
angles[5][6] = 0.09445f;
times[5][6] = 5.80000f;
angles[5][7] = 0.08308f;
times[5][7] = 6.80000f;
angles[5][8] = 0.08308f;
times[5][8] = 7.30000f;
angles[5][9] = -1.25664f;
times[5][9] = 8.40000f;
angles[5][10] = -1.23918f;
times[5][10] = 9.40000f;

jointCodes[6] = "LHipPitch";
angles[6].arraySetSize(11);
times[6].arraySetSize(11);
angles[6][0] = 0.00000f;
times[6][0] = 0.90000f;
angles[6][1] = -0.17453f;
times[6][1] = 1.90000f;
angles[6][2] = -0.17453f;
times[6][2] = 2.70000f;
angles[6][3] = -1.57080f;
times[6][3] = 3.40000f;
angles[6][4] = -1.57080f;
times[6][4] = 3.90000f;
angles[6][5] = -0.85706f;
times[6][5] = 4.90000f;
angles[6][6] = 0.38551f;
times[6][6] = 5.80000f;
angles[6][7] = -0.85521f;
times[6][7] = 6.80000f;
angles[6][8] = -0.83599f;
times[6][8] = 7.30000f;
angles[6][9] = -0.87266f;
times[6][9] = 8.40000f;
angles[6][10] = -0.17453f;
times[6][10] = 9.40000f;

jointCodes[7] = "LHipRoll";
angles[7].arraySetSize(11);
times[7].arraySetSize(11);
angles[7][0] = 0.00000f;
times[7][0] = 0.90000f;
angles[7][1] = 0.00000f;
times[7][1] = 1.90000f;
angles[7][2] = 0.00000f;
times[7][2] = 2.70000f;
angles[7][3] = 0.00000f;
times[7][3] = 3.40000f;
angles[7][4] = 0.54105f;
times[7][4] = 3.90000f;
angles[7][5] = 0.15498f;
times[7][5] = 4.90000f;
angles[7][6] = -0.29142f;
times[7][6] = 5.80000f;
angles[7][7] = 0.19199f;
times[7][7] = 6.80000f;
angles[7][8] = 0.36652f;
times[7][8] = 7.30000f;
angles[7][9] = 0.00000f;
times[7][9] = 8.40000f;
angles[7][10] = -0.01047f;
times[7][10] = 9.40000f;

jointCodes[8] = "LHipYawPitch";
angles[8].arraySetSize(10);
times[8].arraySetSize(10);
angles[8][0] = 0.00000f;
times[8][0] = 0.90000f;
angles[8][1] = 0.00000f;
times[8][1] = 1.90000f;
angles[8][2] = -0.00000f;
times[8][2] = 2.70000f;
angles[8][3] = -0.66323f;
times[8][3] = 3.40000f;
angles[8][4] = -0.49909f;
times[8][4] = 4.90000f;
angles[8][5] = -0.85897f;
times[8][5] = 5.80000f;
angles[8][6] = -0.40225f;
times[8][6] = 6.80000f;
angles[8][7] = -0.40225f;
times[8][7] = 7.30000f;
angles[8][8] = 0.00000f;
times[8][8] = 8.40000f;
angles[8][9] = 0.00000f;
times[8][9] = 9.40000f;

jointCodes[9] = "LKneePitch";
angles[9].arraySetSize(11);
times[9].arraySetSize(11);
angles[9][0] = 0.00000f;
times[9][0] = 0.90000f;
angles[9][1] = 1.67552f;
times[9][1] = 1.90000f;
angles[9][2] = 1.67552f;
times[9][2] = 2.70000f;
angles[9][3] = 1.67552f;
times[9][3] = 3.40000f;
angles[9][4] = 1.67552f;
times[9][4] = 3.90000f;
angles[9][5] = 2.20124f;
times[9][5] = 4.90000f;
angles[9][6] = 1.77479f;
times[9][6] = 5.80000f;
angles[9][7] = 2.20585f;
times[9][7] = 6.80000f;
angles[9][8] = 2.20585f;
times[9][8] = 7.30000f;
angles[9][9] = 2.09440f;
times[9][9] = 8.40000f;
angles[9][10] = 0.34907f;
times[9][10] = 9.40000f;

jointCodes[10] = "LShoulderPitch";
angles[10].arraySetSize(11);
times[10].arraySetSize(11);
angles[10][0] = 0.00000f;
times[10][0] = 0.90000f;
angles[10][1] = 2.09440f;
times[10][1] = 1.90000f;
angles[10][2] = 2.09440f;
times[10][2] = 2.70000f;
angles[10][3] = 2.09440f;
times[10][3] = 3.40000f;
angles[10][4] = 2.09440f;
times[10][4] = 3.90000f;
angles[10][5] = 0.69813f;
times[10][5] = 4.90000f;
angles[10][6] = 0.74074f;
times[10][6] = 5.80000f;
angles[10][7] = 0.73321f;
times[10][7] = 6.80000f;
angles[10][8] = 0.73321f;
times[10][8] = 7.30000f;
angles[10][9] = 1.71042f;
times[10][9] = 8.40000f;
angles[10][10] = 1.83260f;
times[10][10] = 9.40000f;

jointCodes[11] = "LShoulderRoll";
angles[11].arraySetSize(11);
times[11].arraySetSize(11);
angles[11][0] = 1.57080f;
times[11][0] = 0.90000f;
angles[11][1] = 0.80285f;
times[11][1] = 1.90000f;
angles[11][2] = 0.47124f;
times[11][2] = 2.70000f;
angles[11][3] = 0.36652f;
times[11][3] = 3.40000f;
angles[11][4] = 0.00000f;
times[11][4] = 3.90000f;
angles[11][5] = 1.04720f;
times[11][5] = 4.90000f;
angles[11][6] = 0.49851f;
times[11][6] = 5.80000f;
angles[11][7] = 0.49851f;
times[11][7] = 6.80000f;
angles[11][8] = 0.49851f;
times[11][8] = 7.30000f;
angles[11][9] = 0.03491f;
times[11][9] = 8.40000f;
angles[11][10] = 0.19199f;
times[11][10] = 9.40000f;

jointCodes[12] = "RAnklePitch";
angles[12].arraySetSize(11);
times[12].arraySetSize(11);
angles[12][0] = 0.00000f;
times[12][0] = 0.90000f;
angles[12][1] = 0.24435f;
times[12][1] = 1.90000f;
angles[12][2] = 0.24435f;
times[12][2] = 2.70000f;
angles[12][3] = 0.24435f;
times[12][3] = 3.40000f;
angles[12][4] = 0.78540f;
times[12][4] = 3.90000f;
angles[12][5] = 0.78540f;
times[12][5] = 4.90000f;
angles[12][6] = 0.69115f;
times[12][6] = 5.80000f;
angles[12][7] = 0.40317f;
times[12][7] = 6.80000f;
angles[12][8] = -0.57945f;
times[12][8] = 7.30000f;
angles[12][9] = -1.22173f;
times[12][9] = 8.40000f;
angles[12][10] = -0.17453f;
times[12][10] = 9.40000f;

jointCodes[13] = "RAnkleRoll";
angles[13].arraySetSize(11);
times[13].arraySetSize(11);
angles[13][0] = 0.00000f;
times[13][0] = 0.90000f;
angles[13][1] = 0.00000f;
times[13][1] = 1.90000f;
angles[13][2] = 0.00000f;
times[13][2] = 2.70000f;
angles[13][3] = 0.00000f;
times[13][3] = 3.40000f;
angles[13][4] = 0.00000f;
times[13][4] = 3.90000f;
angles[13][5] = 0.00929f;
times[13][5] = 4.90000f;
angles[13][6] = -0.12915f;
times[13][6] = 5.80000f;
angles[13][7] = 0.67960f;
times[13][7] = 6.80000f;
angles[13][8] = 0.27751f;
times[13][8] = 7.30000f;
angles[13][9] = 0.00000f;
times[13][9] = 8.40000f;
angles[13][10] = 0.00000f;
times[13][10] = 9.40000f;

jointCodes[14] = "RElbowRoll";
angles[14].arraySetSize(11);
times[14].arraySetSize(11);
angles[14][0] = 0.00000f;
times[14][0] = 0.90000f;
angles[14][1] = 0.00000f;
times[14][1] = 1.90000f;
angles[14][2] = 1.65806f;
times[14][2] = 2.70000f;
angles[14][3] = 0.69813f;
times[14][3] = 3.40000f;
angles[14][4] = 0.00000f;
times[14][4] = 3.90000f;
angles[14][5] = 0.07205f;
times[14][5] = 4.90000f;
angles[14][6] = 0.05819f;
times[14][6] = 5.80000f;
angles[14][7] = 0.45379f;
times[14][7] = 6.80000f;
angles[14][8] = 0.55995f;
times[14][8] = 7.30000f;
angles[14][9] = 1.13446f;
times[14][9] = 8.40000f;
angles[14][10] = 1.25664f;
times[14][10] = 9.40000f;

jointCodes[15] = "RElbowYaw";
angles[15].arraySetSize(11);
times[15].arraySetSize(11);
angles[15][0] = 0.00000f;
times[15][0] = 0.90000f;
angles[15][1] = -0.15708f;
times[15][1] = 1.90000f;
angles[15][2] = -0.08727f;
times[15][2] = 2.70000f;
angles[15][3] = -0.08727f;
times[15][3] = 3.40000f;
angles[15][4] = -0.08727f;
times[15][4] = 3.90000f;
angles[15][5] = -0.08080f;
times[15][5] = 4.90000f;
angles[15][6] = -0.08241f;
times[15][6] = 5.80000f;
angles[15][7] = 0.00062f;
times[15][7] = 6.80000f;
angles[15][8] = 0.00062f;
times[15][8] = 7.30000f;
angles[15][9] = 1.25664f;
times[15][9] = 8.40000f;
angles[15][10] = 1.23918f;
times[15][10] = 9.40000f;

jointCodes[16] = "RHipPitch";
angles[16].arraySetSize(11);
times[16].arraySetSize(11);
angles[16][0] = 0.00000f;
times[16][0] = 0.90000f;
angles[16][1] = -0.17453f;
times[16][1] = 1.90000f;
angles[16][2] = -0.17453f;
times[16][2] = 2.70000f;
angles[16][3] = -1.57080f;
times[16][3] = 3.40000f;
angles[16][4] = -1.57080f;
times[16][4] = 3.90000f;
angles[16][5] = -1.52484f;
times[16][5] = 4.90000f;
angles[16][6] = -1.55965f;
times[16][6] = 5.80000f;
angles[16][7] = -0.90583f;
times[16][7] = 6.80000f;
angles[16][8] = -0.90583f;
times[16][8] = 7.30000f;
angles[16][9] = -0.87266f;
times[16][9] = 8.40000f;
angles[16][10] = -0.17453f;
times[16][10] = 9.40000f;

jointCodes[17] = "RHipRoll";
angles[17].arraySetSize(11);
times[17].arraySetSize(11);
angles[17][0] = 0.00000f;
times[17][0] = 0.90000f;
angles[17][1] = 0.00000f;
times[17][1] = 1.90000f;
angles[17][2] = 0.00000f;
times[17][2] = 2.70000f;
angles[17][3] = 0.00000f;
times[17][3] = 3.40000f;
angles[17][4] = -0.54105f;
times[17][4] = 3.90000f;
angles[17][5] = -0.55842f;
times[17][5] = 4.90000f;
angles[17][6] = -0.56600f;
times[17][6] = 5.80000f;
angles[17][7] = -0.29671f;
times[17][7] = 6.80000f;
angles[17][8] = -0.01745f;
times[17][8] = 7.30000f;
angles[17][9] = 0.00000f;
times[17][9] = 8.40000f;
angles[17][10] = 0.01047f;
times[17][10] = 9.40000f;

jointCodes[18] = "RHipYawPitch";
angles[18].arraySetSize(5);
times[18].arraySetSize(5);
angles[18][0] = -0.49909f;
times[18][0] = 4.90000f;
angles[18][1] = -0.85897f;
times[18][1] = 5.80000f;
angles[18][2] = -0.40225f;
times[18][2] = 6.80000f;
angles[18][3] = -0.40225f;
times[18][3] = 7.30000f;
angles[18][4] = -0.40225f;
times[18][4] = 8.40000f;

jointCodes[19] = "RKneePitch";
angles[19].arraySetSize(11);
times[19].arraySetSize(11);
angles[19][0] = 0.00000f;
times[19][0] = 0.90000f;
angles[19][1] = 1.67552f;
times[19][1] = 1.90000f;
angles[19][2] = 1.67552f;
times[19][2] = 2.70000f;
angles[19][3] = 1.67552f;
times[19][3] = 3.40000f;
angles[19][4] = 1.67552f;
times[19][4] = 3.90000f;
angles[19][5] = 1.22173f;
times[19][5] = 4.90000f;
angles[19][6] = 1.08036f;
times[19][6] = 5.80000f;
angles[19][7] = 0.87616f;
times[19][7] = 6.80000f;
angles[19][8] = 1.76278f;
times[19][8] = 7.30000f;
angles[19][9] = 2.09440f;
times[19][9] = 8.40000f;
angles[19][10] = 0.34907f;
times[19][10] = 9.40000f;

jointCodes[20] = "RShoulderPitch";
angles[20].arraySetSize(11);
times[20].arraySetSize(11);
angles[20][0] = 0.00000f;
times[20][0] = 0.90000f;
angles[20][1] = 2.09440f;
times[20][1] = 1.90000f;
angles[20][2] = 2.09440f;
times[20][2] = 2.70000f;
angles[20][3] = 2.09440f;
times[20][3] = 3.40000f;
angles[20][4] = 2.09440f;
times[20][4] = 3.90000f;
angles[20][5] = 2.09440f;
times[20][5] = 4.90000f;
angles[20][6] = 1.77434f;
times[20][6] = 5.80000f;
angles[20][7] = 0.89131f;
times[20][7] = 6.80000f;
angles[20][8] = 0.89131f;
times[20][8] = 7.30000f;
angles[20][9] = 1.71042f;
times[20][9] = 8.40000f;
angles[20][10] = 1.83260f;
times[20][10] = 9.40000f;

jointCodes[21] = "RShoulderRoll";
angles[21].arraySetSize(11);
times[21].arraySetSize(11);
angles[21][0] = -1.57080f;
times[21][0] = 0.90000f;
angles[21][1] = -0.80285f;
times[21][1] = 1.90000f;
angles[21][2] = -0.47124f;
times[21][2] = 2.70000f;
angles[21][3] = -0.36652f;
times[21][3] = 3.40000f;
angles[21][4] = 0.00000f;
times[21][4] = 3.90000f;
angles[21][5] = -0.57596f;
times[21][5] = 4.90000f;
angles[21][6] = -0.27770f;
times[21][6] = 5.80000f;
angles[21][7] = -0.87266f;
times[21][7] = 6.80000f;
angles[21][8] = -0.68068f;
times[21][8] = 7.30000f;
angles[21][9] = -0.03491f;
times[21][9] = 8.40000f;
angles[21][10] = -0.19199f;
times[21][10] = 9.40000f;

motion->angleInterpolation(jointCodes, angles, times, 1);



}



void MotionController::ALstandUpCross(){

AL::ALValue jointCodes, angles, times;
jointCodes.arraySetSize(25);
angles.arraySetSize(25);
times.arraySetSize(25);

jointCodes[0] = "HeadPitch";
angles[0].arraySetSize(2);
times[0].arraySetSize(2);
angles[0][0] = 0.00000f;
times[0][0] = 1.90000f;
angles[0][1] = 0.00000f;
times[0][1] = 2.90000f;

jointCodes[1] = "HeadYaw";
angles[1].arraySetSize(2);
times[1].arraySetSize(2);
angles[1][0] = 0.00000f;
times[1][0] = 1.90000f;
angles[1][1] = 0.00000f;
times[1][1] = 2.90000f;

jointCodes[2] = "LAnklePitch";
angles[2].arraySetSize(2);
times[2].arraySetSize(2);
angles[2][0] = 0.52360f;
times[2][0] = 1.90000f;
angles[2][1] = 0.52360f;
times[2][1] = 2.90000f;

jointCodes[3] = "LAnkleRoll";
angles[3].arraySetSize(2);
times[3].arraySetSize(2);
angles[3][0] = 0.00000f;
times[3][0] = 1.90000f;
angles[3][1] = 0.00000f;
times[3][1] = 2.90000f;

jointCodes[4] = "LElbowRoll";
angles[4].arraySetSize(2);
times[4].arraySetSize(2);
angles[4][0] = 0.00000f;
times[4][0] = 1.90000f;
angles[4][1] = 0.00000f;
times[4][1] = 2.90000f;

jointCodes[5] = "LElbowYaw";
angles[5].arraySetSize(2);
times[5].arraySetSize(2);
angles[5][0] = 0.00000f;
times[5][0] = 1.90000f;
angles[5][1] = 0.00000f;
times[5][1] = 2.90000f;

jointCodes[6] = "LHand";
angles[6].arraySetSize(1);
times[6].arraySetSize(1);
angles[6][0] = 0.00000f;
times[6][0] = 1.90000f;

jointCodes[7] = "LHipPitch";
angles[7].arraySetSize(2);
times[7].arraySetSize(2);
angles[7][0] = 0.00000f;
times[7][0] = 1.90000f;
angles[7][1] = 0.00000f;
times[7][1] = 2.90000f;

jointCodes[8] = "LHipRoll";
angles[8].arraySetSize(2);
times[8].arraySetSize(2);
angles[8][0] = 0.00000f;
times[8][0] = 1.90000f;
angles[8][1] = 0.00000f;
times[8][1] = 2.90000f;

jointCodes[9] = "LHipYawPitch";
angles[9].arraySetSize(2);
times[9].arraySetSize(2);
angles[9][0] = 0.00000f;
times[9][0] = 1.90000f;
angles[9][1] = 0.00000f;
times[9][1] = 2.90000f;

jointCodes[10] = "LKneePitch";
angles[10].arraySetSize(2);
times[10].arraySetSize(2);
angles[10][0] = 0.00000f;
times[10][0] = 1.90000f;
angles[10][1] = 0.00000f;
times[10][1] = 2.90000f;

jointCodes[11] = "LShoulderPitch";
angles[11].arraySetSize(2);
times[11].arraySetSize(2);
angles[11][0] = 0.00000f;
times[11][0] = 1.90000f;
angles[11][1] = 0.00000f;
times[11][1] = 2.90000f;

jointCodes[12] = "LShoulderRoll";
angles[12].arraySetSize(2);
times[12].arraySetSize(2);
angles[12][0] = 1.57080f;
times[12][0] = 1.90000f;
angles[12][1] = 1.57080f;
times[12][1] = 2.90000f;

jointCodes[13] = "LWristYaw";
angles[13].arraySetSize(1);
times[13].arraySetSize(1);
angles[13][0] = -1.57080f;
times[13][0] = 1.90000f;

jointCodes[14] = "RAnklePitch";
angles[14].arraySetSize(2);
times[14].arraySetSize(2);
angles[14][0] = 0.52360f;
times[14][0] = 1.90000f;
angles[14][1] = 0.52360f;
times[14][1] = 2.90000f;

jointCodes[15] = "RAnkleRoll";
angles[15].arraySetSize(2);
times[15].arraySetSize(2);
angles[15][0] = 0.00000f;
times[15][0] = 1.90000f;
angles[15][1] = 0.00000f;
times[15][1] = 2.90000f;

jointCodes[16] = "RElbowRoll";
angles[16].arraySetSize(2);
times[16].arraySetSize(2);
angles[16][0] = 0.00000f;
times[16][0] = 1.90000f;
angles[16][1] = 0.00000f;
times[16][1] = 2.90000f;

jointCodes[17] = "RElbowYaw";
angles[17].arraySetSize(2);
times[17].arraySetSize(2);
angles[17][0] = 0.00000f;
times[17][0] = 1.90000f;
angles[17][1] = 0.00000f;
times[17][1] = 2.90000f;

jointCodes[18] = "RHand";
angles[18].arraySetSize(1);
times[18].arraySetSize(1);
angles[18][0] = 0.00000f;
times[18][0] = 1.90000f;

jointCodes[19] = "RHipPitch";
angles[19].arraySetSize(2);
times[19].arraySetSize(2);
angles[19][0] = 0.00000f;
times[19][0] = 1.90000f;
angles[19][1] = 0.00000f;
times[19][1] = 2.90000f;

jointCodes[20] = "RHipRoll";
angles[20].arraySetSize(2);
times[20].arraySetSize(2);
angles[20][0] = 0.00000f;
times[20][0] = 1.90000f;
angles[20][1] = 0.00000f;
times[20][1] = 2.90000f;

jointCodes[21] = "RKneePitch";
angles[21].arraySetSize(2);
times[21].arraySetSize(2);
angles[21][0] = 0.00000f;
times[21][0] = 1.90000f;
angles[21][1] = 0.00000f;
times[21][1] = 2.90000f;

jointCodes[22] = "RShoulderPitch";
angles[22].arraySetSize(2);
times[22].arraySetSize(2);
angles[22][0] = 0.00000f;
times[22][0] = 1.90000f;
angles[22][1] = 0.00000f;
times[22][1] = 2.90000f;

jointCodes[23] = "RShoulderRoll";
angles[23].arraySetSize(2);
times[23].arraySetSize(2);
angles[23][0] = -1.57080f;
times[23][0] = 1.90000f;
angles[23][1] = -1.57080f;
times[23][1] = 2.90000f;

jointCodes[24] = "RWristYaw";
angles[24].arraySetSize(1);
times[24].arraySetSize(1);
angles[24][0] = 1.57080f;
times[24][0] = 1.90000f;

motion->angleInterpolation(jointCodes, angles, times, 1);


}

