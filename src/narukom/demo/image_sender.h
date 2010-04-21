/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#ifndef IMAGE_SENDER_H
#define IMAGE_SENDER_H

#include "system/Thread.h"
#include "pub_sub/publisher.h"


class ImageSender : public Thread, public Publisher
{
    virtual void run();
		virtual void publish(google::protobuf::Message* msg);
//    virtual ~Publisher();
};

#endif // IMAGE_SENDER_H
