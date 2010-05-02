#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include "pub_sub/message_queue.h"
#include "robotcontroller/robotcontroller.h"
#include "led_handler.h"
#include "albroker.h"
#include "alptr.h"
class Communication  {
public:
        Communication(AL::ALPtr<AL::ALBroker> pBroker,MessageQueue *mq, const std::string& configuration_file);
private:
        RobotController* gc;
        LedHandler* lh;
};
#endif 
