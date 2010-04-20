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



class MotionController : public Thread, public Publisher, public Subscriber{
  public:
    MotionController(AL::ALPtr<AL::ALBroker> pbroker){
     

    try {
			AL::ALPtr<AL::ALMotionProxy> motion = pbroker->getMotionProxy();
		} catch (AL::ALError& e) {
		  ;
		}
    }
    void run(){
      std::cout << " MotionController Runs test" << std::endl;
      sleep(1);
      }
  private:


};

#endif
