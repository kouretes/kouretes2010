#include <iostream>
#include "pub_sub/message_queue.h"
#include "pub_sub/publisher.h"
#include "pub_sub/subscriber.h"
#include <string>
#include <vector>
#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "pinger.h"
#include "ponger.h"
#include "echo.h"
#include "scorekeeper.h"
#include "pingpong.pb.h"
#include "../narukom.h"
using namespace std;
int main(int argc, char **argv) {
   
//	    message queue code
     Narukom n;
    MessageQueue* mq = n.get_message_queue();
    std::cout << "Pinger Application" << std::endl;

  Pinger* ping = new Pinger(false);
  Ponger* pong = new Ponger(false);
  ScoreKeeper* sc = new ScoreKeeper(false);
  Echo* echo = new Echo;
// ping pong code
cout << "Adding publishers to message queue 2 " << endl;
  mq->add_publisher(ping);
  mq->add_publisher(pong);

  cout << "Adding subscribers to message queue" << endl;
  mq->add_subscriber(sc);
  mq->add_subscriber ( echo );
  mq->add_subscriber ( pong );
  mq->add_subscriber ( ping );
  
	cout  << "Subscibing to topics" << endl;
	mq->subscribe ( "score",sc,1 );
  mq->subscribe ( "global",echo,2 );
  mq->subscribe ( "motion",ping,0 );
  mq->subscribe ( "motion",pong,0 );
    
	cout << "Start Threads for all publishers and Subscribers" << endl;
	ping->start();
	pong->start();
	echo->start();
	sc->start();	
	ping->start_service();

ping->join();
pong->join();


		
    
  
    
    
//     boost::posix_time::to_iso_string(boost::posix_time::microsec_clock::local_time());
    cout << "After join " << endl;
    return 0;
}
