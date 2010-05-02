#ifndef LED_HANDLER_H
#define LED_HANDLER_H
#include "system/Thread.h"
#include "pub_sub/publisher.h"
#include "pub_sub/subscriber.h"
#include "alptr.h"
#include "albroker.h"
#include "alproxy.h"
#include <vector>
#include <string>
using std::string;
using std::vector;
class LedHandler :  public Thread, public Publisher,public Subscriber
{
public:
    LedHandler(AL::ALPtr<AL::ALBroker> pBroker,MessageQueue* mq);
    void updateLeds(int player_state,int kickoff,int team);
    void setLed(const std::string& device, const std::string& color);
    void setDcmChestColor(const string& color);
    void setDcmFootColor(const string& device,const string& color);
    void setDcmEyeColor(const string& device,const string& color);
    void setDcmEarColor(const string& device,const string& color);
    void setDcmIndividualEarColor(const string& device,const string& color);
    void setDcmIndividualEyeColor(const string& device,const string& color);
    void run();
    void process_messages();
private:
    void initializeColorMap();
    int getColor(string);
    AL::ALPtr<AL::ALProxy> leds; //proxy to handle led colors
    vector<string> chestleds;
    vector<vector<string>* > earleds_b;
    vector<vector<string>* > footleds;
    vector<vector<string>* > eyesleds_r;
    vector<vector<string>* > eyesleds_g;
    vector<vector<string>* > eyesleds_b;
    std::map<string, int > colors;
};

#endif // LED_HANDLER_H
