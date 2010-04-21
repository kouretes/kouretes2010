#include "narukom.h"
#include "topic_tree.h"

Narukom::Narukom()
{
    mq = new MessageQueue("/home/vagvaz/topic_tree.xml");
    udp_multi = new UdpMulticastChannel();
    mq->add_subscriber(udp_multi);
    mq->subscribe("global",udp_multi,0);
    mq->start();
    udp_multi->start();
}
MessageQueue* Narukom::get_message_queue()
{
    if(mq == 0)
        mq = new MessageQueue();
    return mq;

}
