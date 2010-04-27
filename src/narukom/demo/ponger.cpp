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

#include "ponger.h"
#include "pingpong.pb.h"
#include <cstdlib>
#include <time.h>
using std::cerr;
void Ponger::printGame(int other,int myside)
{
  //      "         []         "
  cout << "--------------------" << endl;
// 	"--------------------"
// 	"|        []        O\n"
// 	"|        []         \n"
// 	"         []        |\n"
// 	"         []        |" 
// 	"|        []       O|"
// 	"|        []        |\n"
// 	"         []         \n"
// 	"         []         "
  if( other)
  {
    if(other == myside)
    {
      cout << "|        []        O\n" <<    "|        []         \n" ;//<<endl;
      cout << "         []        |\n" <<    "         []         |"  << endl;}
    else
    {
      cout << "|        []       O|\n" <<    "|        []        |\n" ;//<< endl;
      cout << "         []         \n" <<    "         []          "  << endl;
    }
  }
  else
  {
    if(other == myside)
    {
      cout << "         []        |\n" <<    "         []        |\n" ;//<<endl;
      cout << "|        []        O\n" <<    "|        []         "  << endl;}
    else
    {
      cout << "         []         \n" <<    "         []          \n";// << endl;
      cout << "|        []       O|\n" <<    "|        []        |"  << endl;
    }
  cout << "--------------------" << endl;
  }

}
PongMessage* Ponger::play(PingMessage* msg)
{
  PongMessage* result = new  PongMessage;
  srand(time(0));
//   int myside = rand() % 2;
//   if(myside ==msg->side())
//     result->set_successful(true);
// 	else
// 		result->set_successful(false);
	  int myside = rand() % 2;
	std::cout << "msgside " << msg->side() << " myside " << myside << endl;
  if(myside ==msg->side())
    result->set_successful(true);
	else
		result->set_successful(false);
	cout << "successful " << result->successful() << endl;
  result->set_topic("motion");
  result->set_side(myside);
	result->set_timeout(0);
  printGame(msg->side(),myside);
  
  return result;
}
void Ponger::run()
{
//   cout << "Ponger ---> RUN " << endl;
  if(Subscriber::getBuffer()->size() > 0)
    process_messages();
  
}

void Ponger::process_messages()
{

        MessageBuffer* sub_buf = Subscriber::getBuffer();
    cout << "Ponger " << endl;
    if(sub_buf == NULL)
      cout << "None Unprocessed Buffers" << endl;
    google::protobuf::Message*  cur = sub_buf->remove_head();
  
  
    while(cur != NULL )
    {
      if(cur->GetTypeName() ==  "PingMessage"  )
      {
	PingMessage* msg = (PingMessage*)cur;
	PongMessage* pong_msg = play(msg);
	publish(pong_msg);
      }
       else
      {
	cout << "Unknown Message Type: " << cur->GetTypeName() << endl;
      }
      delete cur; 
      cur = sub_buf->remove_head();
      
    }

  //  sleep(2);
}

void Ponger::publish ( google::protobuf::Message* msg )
{
	  static int delivered = 0;
    Publisher::publish ( msg );
    if(++delivered == 5)
      Thread::stop();
}

