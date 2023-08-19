/*
	NovaGenesis

	Name:		CoreRunSubscribe01
	Object:		CoreRunSubscribe01
	File:		CoreRunSubscribe01.cpp
	Author:		Antonio Marcos Alberti
	Date:		05/2021
	Version:	0.2

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

#ifndef _CORERUNSUBSCRIBE01_H
#include "CoreRunSubscribe01.h"
#endif

#ifndef _CORE_H
#include "Core.h"
#endif

#ifndef _GW_H
#include "GW.h"
#endif

CoreRunSubscribe01::CoreRunSubscribe01 (string _LN, Block *_PB, MessageBuilder *_PMB) : Action (_LN, _PB, _PMB)
{
}

CoreRunSubscribe01::~CoreRunSubscribe01 ()
{
}

// Run the actions behind a received command line
// ng -run --subscribe 0.1 [ < 1 string Category > < N string Key1 .. KeyN > ]
int
CoreRunSubscribe01::Run (Message *_ReceivedMessage, CommandLine *_PCL, vector<Message *> &ScheduledMessages, Message *&InlineResponseMessage)
{
  int Status = OK;
  string Offset = "                    ";
  unsigned int NA = 0;
  vector<string> Category;
  vector<string> Keys;
  vector<string> Limiters;
  vector<string> Sources;
  vector<string> Destinations;
  Core *PCore = 0;
  Message *Subcription = 0;
  CommandLine *PCL = 0;
  vector<string> *Values = new vector<string>;

  PCore = (Core *)PB;

  //PB->S << Offset <<  this->GetLegibleName() << endl;

  // Load the number of arguments
  if (_PCL->GetNumberofArguments (NA) == OK)
	{
	  // Check the number of arguments
	  if (NA == 2)
		{
		  // Get received command line arguments
		  _PCL->GetArgument (0, Category);
		  _PCL->GetArgument (1, Keys);

		  // Pairs=_PCL->Arguments.at(1);
		  if (Category.size () > 0 && Keys.size ())
			{

			  // April 2021, not scalable at all. Would be much better whether the keys were used to determine to which PSS/NRNCS the content has been submitted.
			  for (unsigned int u = 0; u < PCore->PSTuples.size (); u++)
				{
				  Subcription = NULL;
				  Limiters.clear ();
				  Sources.clear ();
				  Destinations.clear ();

				  //PB->S << Offset <<  "(Generating a message to subscribe some keys at domain level)"<<endl;

				  // Setting up the OSID as the space limiter
				  Limiters.push_back (PB->PP->Intra_Domain);

				  // Setting up the this OS as the 1st source SCN
				  Sources.push_back (PB->PP->GetHostSelfCertifyingName ());

				  // Setting up the this OS as the 2nd source SCN
				  Sources.push_back (PB->PP->GetOperatingSystemSelfCertifyingName ());

				  // Setting up the this process as the 3rd source SCN
				  Sources.push_back (PB->PP->GetSelfCertifyingName ());

				  // Setting up the PS block SCN as the 4th source SCN
				  Sources.push_back (PB->GetSelfCertifyingName ());

				  // Setting up the destination
				  Destinations.push_back (PCore->PSTuples[u]->Values[0]);

				  // Setting up the destination 2nd source
				  Destinations.push_back (PCore->PSTuples[u]->Values[1]);

				  // Setting up the destination 3rd source
				  Destinations.push_back (PCore->PSTuples[u]->Values[2]);

				  // Setting up the destination 4th source
				  Destinations.push_back (PCore->PSTuples[u]->Values[3]);

				  // Creating a new message
				  PB->PP->NewMessage (GetTime (), 1, false, Subcription);

				  // Creating the ng -cl -m command line
				  PMB->NewConnectionLessCommandLine ("0.1", &Limiters, &Sources, &Destinations, Subcription, PCL);

				  // ***************************************************
				  // Generate the ng -s -bind
				  // ***************************************************

				  // Subscribe the key on the Category 18
				  PMB->NewSubscriptionCommandLine ("0.1", PB->StringToInt (Category.at (0)), &Keys, Subcription, PCL);

				  // Subscribe the key on the Category 2
				  PMB->NewSubscriptionCommandLine ("0.1", 2, &Keys, Subcription, PCL);

				  // Subscribe the key on the Category 9
				  PMB->NewSubscriptionCommandLine ("0.1", 9, &Keys, Subcription, PCL);

				  // ***************************************************
				  // Generate the ng -message --type [ < 1 string 1 > ]
				  // ***************************************************

				  //Subcription->NewCommandLine("-message","--type","0.1",PCL);

				  //PCL->NewArgument(1);

				  //PCL->SetArgumentElement(0,0,PB->IntToString(Subcription->GetType()));

				  // ***************************************************
				  // Generate the ng -message --seq [ < 1 string 1 > ]
				  // ***************************************************

				  //Subcription->NewCommandLine("-message","--seq","0.1",PCL);

				  //PCL->NewArgument(1);

				  //PCL->SetArgumentElement(0,0,PB->IntToString(PCore->GetSequenceNumber()));

				  // ******************************************************
				  // Generate the SCN
				  // ******************************************************

				  PB->GenerateSCNFromMessageBinaryPatterns (Subcription, SCN);

				  // Creating the ng -scn --s command line
				  PMB->NewSCNCommandLine ("0.1", SCN, Subcription, PCL);

				  //PB->S << Offset <<"(The following message contains a subscription to the peer)"<< endl;

				  //PB->S << "(" << endl << *Subscription << ")"<< endl;

				  // ******************************************************
				  // Finish
				  // ******************************************************

				  // Push the message to the GW input queue
				  PCore->PGW->PushToInputQueue (Subcription);
				}

			  if (ScheduledMessages.size () > 0)
				{
				  Message *Temp = ScheduledMessages.at (0);

				  Temp->MarkToDelete ();

				  // Make the scheduled messages vector empty
				  ScheduledMessages.clear ();
				}

			  Status = OK;
			}
		  else
			{
			  PB->S << Offset << "(ERROR: One or more argument is empty)" << endl;
			}

		}
	  else
		{
		  PB->S << Offset << "(ERROR: Wrong number of arguments)" << endl;
		}
	}
  else
	{
	  PB->S << Offset << "(ERROR: Unable to read the number of arguments)" << endl;
	}

  delete Values;

  //PB->S << Offset <<  "(Done)" << endl << endl << endl;

  return Status;
}

