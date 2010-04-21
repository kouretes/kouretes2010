#include <iostream>
#include "pub_sub/message_queue.h"
#include "ponger.h"
#include "network/udp_multicast_channel.h"

int main(int argc, char **argv) {
	
	MessageQueue* mq = new MessageQueue();
    std::cout << "Pinger Application" << std::endl;
    std::vector<Publisher*> pubs;
    std::vector<Subscriber*> subs;
    std::vector<std::string*> names;
    
    mq->start();
		
  Ponger* pong = new Ponger(false);
	UdpMulticastChannel* udp = new UdpMulticastChannel;
cout << "Adding publishers to message queue " << endl;
  mq->add_publisher(pong);
	
   mq->add_publisher(udp);

  cout << "Adding subscribers to message queue" << endl;
   mq->add_subscriber ( udp );
  mq->add_subscriber ( pong );

	cout  << "Subscibing to topics" << endl;

   mq->subscribe ( "global",udp,2 );
  mq->subscribe ( "motion",pong,0 );

    
	cout << "Start Threads for all publishers and Subscribers" << endl;
	udp->start();
	pong->start();
pong->join();
    cout << "After join " << endl;
    return 0;
}
