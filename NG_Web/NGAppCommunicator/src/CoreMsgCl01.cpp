/*
	NovaGenesis

	Name:		CoreMsgCl01
	Object:		CoreMsgCl01
	File:		CoreMsgCl01.cpp
	Author:		Antonio M. Alberti
	Date:		11/2012
	Version:	0.1
*/

#ifndef _COREMSGCL01_H
#include "CoreMsgCl01.h"
#endif

#ifndef _HT_H
#include "HT.h"
#endif

#ifndef _CORE_H
#include "Core.h"
#endif

CoreMsgCl01::CoreMsgCl01(string _LN, Block *_PB, MessageBuilder *_PMB):Action(_LN,_PB,_PMB)
{
}

CoreMsgCl01::~CoreMsgCl01()
{
}

// Run the actions behind a received command line
// ng -msg --cl _Version [ < _LimitersSize string S_1 ... S_LimitersSize > < _SourcesSize string S_1 ... S_SourcesSize > < _DestinationsSize string S_1 ... S_Destinations > ]
int CoreMsgCl01::Run(Message *_ReceivedMessage, CommandLine *_PCL, vector<Message*> &ScheduledMessages, Message *&InlineResponseMessage)
{
	int 			Status=ERROR;
	unsigned int 	NA=0;
	vector<string>	Limiters;
	vector<string>	Sources;
	vector<string>	Destinations;
	vector<string>	RunLimiters;
	vector<string>	RunSources;
	Message			*Run=0;
	CommandLine		*PCL;
	string			Offset = "                    ";
	Core			*PCore=0;

	PCore=(Core*)PB;

	PB->S << Offset <<  this->GetLegibleName() << endl;

	//PB->S << Offset << "(Messages container size = "<<PB->GetNumberOfMessages()<<")"<< endl;

	// Load the number of arguments
	if (_PCL->GetNumberofArguments(NA) == OK)
		{
			// Check the number of argument
			if (NA == 3)
				{
					// Get received command line arguments
					_PCL->GetArgument(0,Limiters);
					_PCL->GetArgument(1,Sources);
					_PCL->GetArgument(2,Destinations);

					if (Limiters.size() > 0 && Sources.size() > 0 && Destinations.size() > 0)
						{
							// **********************************************************************************
							// Prepare the first command line of the ng -run
							// **********************************************************************************

							// Setting up the process SCN as the space limiter
							RunLimiters.push_back(PB->PP->GetSelfCertifyingName());

							// Setting up the CLI block SCN as the source SCN
							RunSources.push_back(PB->GetSelfCertifyingName());

							// Creating a new message
							PB->PP->NewMessage(GetTime(),1,false,Run);

							// Creating the ng -cl -msg command line
							PMB->NewConnectionLessCommandLine("0.1",&RunLimiters,&RunSources,&RunSources,Run,PCL);

							// Store this message on the scheduled messages vector
							ScheduledMessages.push_back(Run);

							// Set the flag to generate the ng -run command line, if required.
							PCore->GenerateRunX01 = true;

							// Set the flag to generate the ng -scn --seq for the ng -run message
							PCore->GenerateRunXSCNSeq01=true;
						}
				}
		}


	//PB->S << Offset <<  "(Done)" << endl << endl << endl; 

	return Status;
}
