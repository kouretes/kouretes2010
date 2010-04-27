#ifndef NARUKOM_H
#define NARUKOM_H
#include "pub_sub/message_queue.h"
#include "network/udp_multicast_channel.h"
class Narukom{
private:
    MessageQueue* mq;
    UdpMulticastChannel* udp_multi;
public:
    Narukom();
    MessageQueue* get_message_queue();
    ~Narukom(){}//delete mq;}
};
#endif
