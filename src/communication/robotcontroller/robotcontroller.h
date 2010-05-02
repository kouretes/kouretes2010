#ifndef ROBOTCONTROLLER_H
#define ROBOTCONTROLLER_H
#include "system/Thread.h"
#include "pub_sub/publisher.h"
#include "gamecontroller.h"
#include "RoboCupGameControlData.h"
#include "messages/Gamecontroller.pb.h"
#include "button_checker.h"
#include "alptr.h"
#include "albroker.h"
#include "alproxy.h"
#include "almemoryproxy.h"
class ButtonClient;
class RobotController : public Thread, public Publisher
{
public:
    RobotController(AL::ALPtr<AL::ALBroker> pBroker,MessageQueue* mq, const std::string& configuration_file);
    void run();
private:
    void sendLedUpdate();
    bool readConfiguration(const std::string& file_name);
    int  extract_player_state(RoboCupGameControlData* data);
    GameController* gm;
    ButtonListener* bc;
    RoboCupGameControlData game_data;
    AL::ALMemoryProxy* memory;
    bool received_data;
    int chest_button_pressed;
    int left_bumper_pressed;
    int right_bumper_pressed;
    LedChangeMessage leds;
    GameStateMessage gm_state;
    ConfigMessage conf;
    Mutex mx;
    Mutex button_mx;
};

#endif // ROBOTCONTROLLER_H
