#ifndef BUTTON_CHECKER_H
#define BUTTON_CHECKER_H
#include "almemoryproxy.h"
#include "alproxy.h"
#include "alsentinelproxy.h"
#include <albroker.h>
#include <alproxy.h>
#include <alptr.h>
#include <alvalue.h>
#include <almodule.h>
#include "system/Mutex.h"
#include "system/Thread.h"
class ButtonListener : public AL::ALModule
{
public:
    ButtonListener(AL::ALPtr<AL::ALBroker> pbroker,const std::string& name) ;

    void buttonPressed(const std::string& pDataName, const AL::ALValue& pValue, const std::string& pMessage); //event handler for the button pressed
    void RBumperPressed(const std::string& pDataName, const AL::ALValue& pValue, const std::string& pMessage); //
    void LBumperPressed(const std::string& pDataName, const AL::ALValue& pValue, const std::string& pMessage); //
private:

    AL::ALPtr<AL::ALProxy> sentinel; //proxy to alsentinel for overriding default button press
    AL::ALPtr<AL::ALMemoryProxy> memory; //proxy to almemory module to exchange data
    bool* chest_button_pressed;
    bool* left_bumper_pressed;
    bool* right_bumper_pressed;
    Mutex* mx;

};

#endif // BUTTON_CHECKER_H
