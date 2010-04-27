/*
 * Catalog.h
 *
 *  Created on: Mar 29, 2010
 *      Author: vagvaz
 */

#ifndef CATALOG_H_
#define CATALOG_H_

#include "system/Thread.h"
#include "pub_sub/publisher.h"
#include "pub_sub/subscriber.h"
#include "network/channel_info.h"
#include <map>


class Catalog: public Thread, public Publisher, public Subscriber {
public:
	Catalog();
	virtual ~Catalog();
	virtual void run();
	virtual void publish(google::protobuf::Message* msg);
	virtual void protcess_messages();
private:
	std::map<ChannelInfo> channels;

};

#endif /* CATALOG_H_ */
