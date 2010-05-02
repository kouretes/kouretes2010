#include "communication.h"

Communication::Communication(AL::ALPtr<AL::ALBroker> pBroker,MessageQueue *mq, const std::string& configuration_file)
{
    gc = new RobotController(pBroker,mq,configuration_file);
    lh = new LedHandler(pBroker,mq);
    gc->start();
    lh->start();
    //gc->join();
    //lh->join();

}
