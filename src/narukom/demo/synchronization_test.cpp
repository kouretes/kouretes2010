#include <iostream>
#include "pub_sub/message_queue.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include "ratio.h"
#include "slow.h"
#include "fast.h"

using namespace std;
using namespace google::protobuf;
int main(int argc, char **argv) {
   MessageQueue mq;
	 Slow s;
	 Fast f;
	 Ratio r;
	 cout << "Add Publishers " << endl;
	 mq.add_publisher(&s);
	 mq.add_publisher(&f);
	 cout << "Add Subscribers" << endl;
	 mq.add_subscriber((r.get_blackboard()));
	 cout <<  "Subscibing.." << endl;
	 mq.subscribe("global",r.get_blackboard(),2);
	 cout << "start threads" << endl;
	 mq.start();
	 s.start();
	 f.start();
	 r.start();
	 cout << "Wait for joing" << endl;
	 s.join();
	 f.join();
	 r.join();
	 
	 mq.join();
	 cout << "Exiting.. " << endl;
	 return 0;
return 0;
}
