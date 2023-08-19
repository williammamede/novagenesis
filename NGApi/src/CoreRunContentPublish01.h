/*
	NovaGenesis
	
	Name:		CoreRunContentPublish01
	Object:		CoreRunContentPublish01
	File:		CoreRunContentPublish01.h
	Author:		Antonio Marcos Alberti
	Date:		05/2021
	Version:	0.1

  	Copyright (C) 2021  Antonio Marcos Alberti

    This work is available under the GNU Lesser General Public License (See COPYING.txt).

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _CORERUNCONTENTPUBLISH01_H
#define _CORERUNCONTENTPUBLISH01_H

#ifndef _STRING_H
#include <string>
#endif

#ifndef _ACTION_H
#include "Action.h"
#endif

#ifndef _MESSAGE_H
#include "Message.h"
#endif

#ifndef _MESSAGEBUILDER_H
#include "MessageBuilder.h"
#endif

#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef _SYS_TYPES_H
#include <sys/types.h>
#endif

#ifndef _UNISTD_H
#include <unistd.h>
#endif

#ifndef _IOSTREAM_H
#include <iostream>
#endif

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

#ifndef _DIRENT_H
#include <dirent.h>
#endif

#define ERROR 1
#ifndef OK
#define OK 0
#endif

class Block;

class CoreRunContentPublish01 : public Action {
 public:

  // Constructor
  CoreRunContentPublish01 (string _LN, Block *_PB, MessageBuilder *_PMB);

  // Destructor
  virtual ~CoreRunContentPublish01 ();

  // Run the actions behind a received message
  virtual int
  Run (Message *_ReceivedMessage, CommandLine *_PCL, vector<Message *> &ScheduledMessages, Message *&InlineResponseMessage);

  // Publish a content to all the peer applications - notifies just one peer
  int PublishAPhotoToAllPeerApps01 (string _FileName, vector<Message *> &ScheduledMessages);

  // Publish a content to all the peer applications - notifies all peers
  int PublishAPhotoToAllPeerApps02 (string _FileName, vector<Message *> &ScheduledMessages);

  // Publish a content to all server applications - notifies all server peers
  int PublishAPhotoToAllPeerApps03 (string _FileName, vector<Message *> &ScheduledMessages);

  // Auxiliary to sort files in the directory
  std::vector<std::string> read_directory (const string _Path);

  // TODO: FIXP/Update - This function has been added to simply content publishing
  // Auxiliary function
  void CreatePublishMessage (double _Time, string _FileName, vector<Tuple *> &_PubNotify, vector<Tuple *> &_SubNotify);

  // Generates a report containing the content published
  void generatePublishMessageReport (string _FileName, double _Time);
};

#endif






