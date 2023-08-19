/*
	NovaGenesis

	Name:		CoreNotifyS01
	Object:		CoreNotifyS01
	File:		CoreNotifyS01.cpp
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

#ifndef _CORENOTIFYS01_H
#include "CoreNotifyS01.h"
#endif

#ifndef _CORE_H
#include "Core.h"
#endif

#ifndef _GW_H
#include "GW.h"
#endif

//#define DEBUG // To follow message processing
//#define DEBUG1 // To follow message processing for flow control

CoreNotifyS01::CoreNotifyS01 (string _LN, Block *_PB, MessageBuilder *_PMB) : Action (_LN, _PB, _PMB)
{
}

CoreNotifyS01::~CoreNotifyS01 ()
{
}

// Example of a real NOTIFY: ng -notify --s 0.1 [ < 1 s 18 > < 1 s 5277918A > < 4 s 92F3A5FD 4AF985A5 CAB3F4D6  > ]

// TODO: FIXP/Update - All this function have been modified to accelerate subscriptions in September 2021
// Run the actions behind a received command line
// ng -notify --s _Version [ < 1 string _Category > < _KeysSize string S_1 ... S_KeysSize > < _TupleSize string T_1 ... T_TupleSize > ]
int
CoreNotifyS01::Run (Message *_ReceivedMessage, CommandLine *_PCL, vector<Message *> &ScheduledMessages, Message *&InlineResponseMessage)
{
  int Status = OK;
  string Offset = "                    ";
  unsigned int NA = 0;
  vector<string> Limiters;
  vector<string> Sources;
  vector<string> Destinations;
  Message *RunSchedule = 0;
  CommandLine *PCL = 0;
  Core *PCore = 0;
  vector<string> ReceivedCategory;
  vector<string> ReceivedKeys;
  vector<string> SubscribingKeys;
  vector<string> ReceivedPublisher;
  Message *SubscriptionM = NULL;
  vector<string> *Values = new vector<string>;

  PCore = (Core *)PB;

#ifdef DEBUG
  PB->S << endl << Offset << this->GetLegibleName () << endl;
#endif

  // Load the number of arguments
  if (_PCL->GetNumberofArguments (NA) == OK)
	{
	  // Check the number of arguments
	  if (NA == 3)
		{
		  _PCL->GetArgument (0, ReceivedCategory);    		// Category
		  _PCL->GetArgument (1, ReceivedKeys);        // Keys
		  _PCL->GetArgument (2, ReceivedPublisher);  			// Publisher's tuples

		  bool Store[ReceivedKeys.size ()];

		  if (ReceivedCategory.size () > 0 && ReceivedKeys.size () > 0 && ReceivedPublisher.size () > 0)
			{
			  for (unsigned int b = 0; b < ReceivedKeys.size (); b++)
				{

#ifdef DEBUG1
				  PB->S << Offset << "(Checking a new notification with the key = " << ReceivedKeys.at(b) << ")" << endl;
#endif

				  Store[b] = true;

				  for (unsigned int a = 0; a < PCore->KeysOfReceivedPayloads.size (); a++)
					{
					  if (PCore->KeysOfReceivedPayloads.at (a) == ReceivedKeys.at (b))
						{
						  Store[b] = false;

						  break;
						}
					}
				}

			  // For each received key
			  for (unsigned int c = 0; c < ReceivedKeys.size (); c++)
				{
				  // Check whether if subscription is required
				  if (Store[c] == true)
					{
#ifdef DEBUG1
					  PB->S << Offset << "(Messages in memory = " << PB->PP->GetNumberOfMessages () << ")" << endl;
#endif

					  if (PB->PP->GetNumberOfMessages () < (MAX_MESSAGES_IN_MEMORY - 200))
						{
						  // Set for subscription
						  SubscribingKeys.push_back(ReceivedKeys.at(c));

						  // Allocates a new subscription
						  Subscription *PS;

						  PCore->NewSubscription (PS);

						  PS->Category = PB->StringToInt (ReceivedCategory.at (0));

						  PS->Status = "Waiting delivery";

#ifdef DEBUG
						  PB->S << Offset << "(Notification content category = " << PS->Category << ")" << endl;
#endif

						  // Copy the key to the Subscription container
						  PS->Key = ReceivedKeys.at (c);

#ifdef DEBUG
						  PB->S << Offset << "(Notification content key = " << PS->Key << ")" << endl;
#endif

						  // Copy the publisher's tuple to the Subscription container
						  for (unsigned int e = 0; e < ReceivedPublisher.size (); e++)
							{
							  PS->Publisher.Values.push_back (ReceivedPublisher.at (e));

#ifdef DEBUG
							  PB->S << Offset << "(Notification publisher ID [" << e << "] = "
									<< PS->Publisher.Values[e]
									<< ")" << endl;
#endif
							}

#ifdef DEBUG
						  PCore->Debug.OpenOutputFile ();

						  PCore->Debug << endl << "Received a notification of the key " << PS->Key
									   << ". Creating the subscription of index " << (PCore->Subscriptions.size () - 1)
									   << ". Status of the subscription is " << PS->Status << endl;

						  PCore->Debug.CloseFile ();
#endif

						  PS->HasContent = false;

						  // Storing a timestamp for performance evaluation
						  PS->Timestamp = GetTime ();
						}
					  else
						{
						  PB->S << Offset << "(Skipping this subscription, since the process is congested.)" << endl;
						}
					}
				  else
					{
					  PB->S << Offset << "(The content with key = " << ReceivedKeys.at (c)
							<< " has already been successfully received. Skipping new subscription.)" << endl;
					}
				}

				if (SubscribingKeys.size() > 0)
				  {
					// TODO: FIXP/Update - Directly schedule subscription of content. Altered in September 2021
					// April 2021, not scalable at all. Would be much better whether the keys were used to determine to which PSS/NRNCS the content has been submitted.
					for (unsigned int u = 0; u < PCore->PSTuples.size (); u++)
					  {
						SubscriptionM = NULL;
						Limiters.clear ();
						Sources.clear ();
						Destinations.clear ();

						// Setting up the OSID as the space limiter
						Limiters.push_back (PB->PP->Intra_Domain);

						// Setting up this OS as the 1st source SCN
						Sources.push_back (PB->PP->GetHostSelfCertifyingName ());

						// Setting up this OS as the 2nd source SCN
						Sources.push_back (PB->PP->GetOperatingSystemSelfCertifyingName ());

						// Setting up this process as the 3rd source SCN
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
						PB->PP->NewMessage (GetTime (), 1, false, SubscriptionM);

						// Creating the ng -cl -m command line
						PMB->NewConnectionLessCommandLine ("0.1", &Limiters, &Sources, &Destinations, SubscriptionM, PCL);

						// ***************************************************
						// Generate the ng -s -bind
						// ***************************************************

						// Subscribe the key on the Category 18
						PMB->NewSubscriptionCommandLine ("0.1", PB
							->StringToInt (ReceivedCategory.at(0)), &SubscribingKeys, SubscriptionM, PCL);

						// Subscribe the key on the Category 2
						//PMB->NewSubscriptionCommandLine ("0.1", 2, &SubscribingKeys, SubscriptionM, PCL);

						// Subscribe the key on the Category 9
						//PMB->NewSubscriptionCommandLine ("0.1", 9, &SubscribingKeys, SubscriptionM, PCL);

						// ***************************************************
						// Generate the ng -message --type [ < 1 string 1 > ]
						// ***************************************************

						//SubscriptionM->NewCommandLine("-message","--type","0.1",PCL);

						//PCL->NewArgument(1);

						//PCL->SetArgumentElement(0,0,PB->IntToString(SubscriptionM->GetType()));

						// ***************************************************
						// Generate the ng -message --seq [ < 1 string 1 > ]
						// ***************************************************

						//SubscriptionM->NewCommandLine("-message","--seq","0.1",PCL);

						//PCL->NewArgument(1);

						//PCL->SetArgumentElement(0,0,PB->IntToString(PCore->GetSequenceNumber()));

						// ******************************************************
						// Generate the SCN
						// ******************************************************

						PB->GenerateSCNFromMessageBinaryPatterns (SubscriptionM, SCN);

						// Creating the ng -scn --s command line
						PMB->NewSCNCommandLine ("0.1", SCN, SubscriptionM, PCL);

						//PB->S << Offset << "(The following message contains a subscription to the peer)" << endl;

						//PB->S << "(" << endl << *SubscriptionM << ")" << endl;

						// ******************************************************
						// Finish
						// ******************************************************

						// Push the message to the GW input queue
						PCore->PGW->PushToInputQueue (SubscriptionM);
					  }
				  }
				else
				  {
#ifdef DEBUG
					  PB->S << Offset << "(Warning: All content was previously subscribed. No need to new subscriptions" << endl;
#endif
				  }

			  PCore->GenerateRunX01 = false;

			  if (ScheduledMessages.size () > 0)
				{
				  Message *Temp = ScheduledMessages.at (0);

				  Temp->MarkToDelete ();

				  // Make the scheduled messages vector empty
				  ScheduledMessages.clear ();
				}
			}
		  else
			{
			  PB->S << Offset << "(ERROR: Notification arguments are missing)" << endl;
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

#ifdef DEBUG
  PB->S << Offset << "(Done)" << endl << endl << endl;
#endif

  return Status;
}
