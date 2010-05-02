#include "button_checker.h"
#include <string>
using std::string;
using AL::ALValue;
using std::cout;
using std::endl;
ButtonListener::ButtonListener(AL::ALPtr<AL::ALBroker> pbroker, const std::string& name) : AL::ALModule(pbroker,name)/* bool* button_pressed, bool* left_bumper_pressed, bool* right_bumper_pressed, Mutex* mx) :AL::ALModule(pbroker,string("ButtonListener")),
                             chest_button_pressed(button_pressed),left_bumper_pressed(left_bumper_pressed),
                             right_bumper_pressed(right_bumper_pressed),mx(mx)*/
{
    setModuleDescription( "Module that listens button events" );
    functionName("buttonPressed","ButtonListener", "button pressed");
    BIND_METHOD(ButtonListener::buttonPressed);
    functionName("LBumperPressed","ButtonListener", "left bumber pressed");
    BIND_METHOD(ButtonListener::LBumperPressed);
    functionName("RBumperPressed","ButtonListener", "right bumber pressed");
    BIND_METHOD(ButtonListener::RBumperPressed);
    cout << "Button Listener start of constructor" << endl;
    try {
        sentinel = getParentBroker()->getProxy("ALSentinel");
        memory  = getParentBroker()->getMemoryProxy();
      }
    catch( AL::ALError& e) {
           cout << "Could not create sentiner or memory in ButtonListener Module " << endl;
      }
      //disable default chest button function
        sentinel->pCall<bool>("enableDefaultActionSimpleClick",false);
        //subscribe on chest button pressed
        memory->subscribeToEvent(string("ChestButtonPressed"),getName(),string("buttonPressed"));
        memory->subscribeToEvent (
                    "LeftBumperPressed",
                    getName(),

                    "LBumperPressed" );

          memory->subscribeToEvent(
                    "RightBumperPressed",
                    getName(),

                    "RBumperPressed" );
            cout << "Button Listener end of constructor" << endl;
}
void ButtonListener::buttonPressed(const std::string& pDataName, const ALValue& pValue, const std::string& pMessage)
{
    cout << "Button Pressed" << endl;
   // mx->Lock();
    memory->insertData("button_pressed",1);
   // mx->Unlock();
    //memory->raiseEvent("LBumperPressed",1.0f);
}
void ButtonListener::LBumperPressed(const std::string& pDataName, const ALValue& pValue, const std::string& pMessage)
{
    cout << "Left Pressed" << endl;
    memory->insertData("lbumper_pressed",1);
}
void ButtonListener::RBumperPressed(const std::string& pDataName, const ALValue& pValue, const std::string& pMessage)
{
    cout << "Right Pressed" << endl;
    memory->insertData("rbumper_pressed",1);
}

