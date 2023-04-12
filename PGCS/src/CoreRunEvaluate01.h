/*
	NovaGenesis
	
	Name:		CoreRunEvaluate01
	Object:		CoreRunEvaluate01
	File:		CoreRunEvaluate01.h
	Author:		Antonio Marcos Alberti
	Date:		05/2021
	Version:	0.1

 	Copyright (C) 2021  Antonio Marcos Alberti

    This work is available under the GNU General Public License (See COPYING.txt).

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _CORERUNEVALUATE01_H
#define _CORERUNEVALUATE1_H

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

#ifndef _TUPLE_H
#include "Tuple.h"
#endif

#ifndef _CORE_H
#include "Core.h"
#endif

#define ERROR 1
#define OK 0

class Block;

class CoreRunEvaluate01 : public Action {
 public:

  // Auxiliary
  double TimeToExpose;

  // Constructor
  CoreRunEvaluate01 (string _LN, Block *_PB, MessageBuilder *_PMB);

  // Destructor
  virtual ~CoreRunEvaluate01 ();

  // Run the actions behind a received message
  virtual int
  Run (Message *_ReceivedMessage, CommandLine *_PCL, vector<Message *> &ScheduledMessages, Message *&InlineResponseMessage);

  int CheckForPSAwareness (vector<Message *> &_ScheduledMessages, bool &_ClearScheduledMessage);

  int CheckForNewPeerApplication (vector<Message *> &_ScheduledMessages, bool &_ClearScheduledMessage);

  int ShowTheDiscoveredPeers ();

  int CheckSubscriptions (vector<Message *> &_ScheduledMessages, bool _ClearScheduledMessage);

  int ScheduleASubscription (Subscription *_PS, vector<Message *> &_ScheduledMessages, bool _ClearScheduledMessage);

  int
  ProcessASubscribedPayload (string _Publisher_LN, unsigned int _Index, Subscription *_PS, vector<Message *> &_ScheduledMessages, bool _ClearScheduledMessage);

  int ProcessTXTFile (string _Publisher_LN, unsigned int _Index, Subscription *_PS, string _FileName);

  bool Run (string _Command, FILE *&_f);

  void generateApplicationLifecycleReport(double _Time);
};

#endif






