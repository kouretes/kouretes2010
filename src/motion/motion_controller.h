#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H
#include "narukom/system/Thread.h"
#include "narukom/pub_sub/publisher.h"
#include "narukom/pub_sub/subscriber.h"
#include "alxplatform.h"
#include <albroker.h>
#include <alproxy.h>
#include <almemoryfastaccess.h>
#include "almotionproxy.h"
#include "albrokermanager.h"
#include "alptr.h"
#include "messages/motion.pb.h"


class MotionController : public Thread, public Publisher, public Subscriber{
  public:

	AL::ALPtr<AL::ALMotionProxy> motion;
    MotionController(AL::ALPtr<AL::ALBroker> pbroker){
     

    try {
			 motion = pbroker->getMotionProxy();
		} catch (AL::ALError& e) {
		  ;
		}
    }
    void run(){
      std::cout << " MotionController Runs test" << std::endl;
	motion->setWalkTargetVelocity(0.5, 0.5, 0.5, 0.5);
      sleep(10);
	motion->killWalk();
	      sleep(10);
      }
  private:
  MotionCommand command;

};

#endif
