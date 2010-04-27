/**
 * @file krobot.cpp
 * */

#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/resource.h>

#include "alxplatform.h"
#include <albroker.h>
#include <alproxy.h>
#include <almemoryfastaccess.h>
#include "almotionproxy.h"

#include "albrokermanager.h"
#include "alptr.h"
#include "motion/motion_controller.h"
#include "vision/vision.h"

using namespace AL;
using namespace std;

void _terminationHandler(int signum) {
	std::cout << "Exiting Kouretes." << std::endl;
	AL::ALBrokerManager::getInstance()->killAllBroker();
	exit(0);
}

int main(int argc, char *argv[]) {
	std::cout << "..::: Kouretes Robot :::.." << std::endl;

	int i = 1;
	std::string brokerName = "Robot";
	std::string brokerIP = "";
	int brokerPort = 0;
	// Default parent broker IP
	std::string parentBrokerIP = "127.0.0.1";
	// Default parent broker port
	int parentBrokerPort = 9561;

	// checking options
	while (i < argc) {
		//    if ( argv[i][0] != '-' ) return usage( argv[0] );
		if (std::string(argv[i]) == "-b")
			brokerIP = std::string(argv[++i]);
		else if (std::string(argv[i]) == "-p")
			brokerPort = atoi(argv[++i]);
		else if (std::string(argv[i]) == "-pip")
			parentBrokerIP = std::string(argv[++i]);
		else if (std::string(argv[i]) == "-pport")
			parentBrokerPort = atoi(argv[++i]);
		//    else if ( std::string( argv[i] ) == "-h" )        return usage( argv[0] );
		i++;
	}

	std::cout << "Try to connect to parent Broker at ip :" << parentBrokerIP << " and port : " << parentBrokerPort << std::endl;
	std::cout << "Start the server bind on this ip :  " << brokerIP << " and port : " << brokerPort << std::endl;

	try {

		AL::ALBroker::Ptr broker = AL::ALBroker::createBroker(brokerName, brokerIP, brokerPort, parentBrokerIP, parentBrokerPort);

		SleepMs(1000);
		Vision testV(broker);
		MotionController mc(broker);

		testV.start();
		mc.start();
		SleepMs(1000);
		testV.join();
		mc.join();

		cout << "EXITING TEST" << endl;
		exit(0);

	} catch (AL::ALError &e) {
		std::cout << "-----------------------------------------------------" << std::endl;
		std::cout << "Creation of broker failed. Application will exit now." << std::endl;
		std::cout << "-----------------------------------------------------" << std::endl;
		std::cout << e.toString() << std::endl;
		exit(0);
	}

# ifndef _WIN32
	struct sigaction new_action;
	/* Set up the structure to specify the new action. */
	new_action.sa_handler = _terminationHandler;
	sigemptyset(&new_action.sa_mask);
	new_action.sa_flags = 0;

	sigaction(SIGINT, &new_action, NULL);
# endif

	while (1) {
		SleepMs(100);
	}

# ifdef _WIN32
	_terminationHandler( 0 );
# endif

	exit(0);
}
