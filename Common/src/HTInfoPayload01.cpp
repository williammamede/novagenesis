/*
	NovaGenesis

	Name:		HTInfoPayload01
	Object:		HTInfoPayload01
	File:		HTInfoPayload01.cpp
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

#ifndef _HTINFOPAYLOAD01_H
#include "HTInfoPayload01.h"
#endif

#ifndef _HT_H
#include "HT.h"
#endif

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include <stdio.h>
#include <string.h>

using namespace rapidjson;

HTInfoPayload01::HTInfoPayload01 (string _LN, Block *_PB, MessageBuilder *_PMB) : Action (_LN, _PB, _PMB)
{
}

HTInfoPayload01::~HTInfoPayload01 ()
{
}

// Run the actions behind a received command line
// ng -info --_Alternative _Version [ < n string _ValuesSize string S_1 ... S_ValuesSize > ]
int
HTInfoPayload01::Run (Message *_ReceivedMessage, CommandLine *_PCL, vector<Message *> &ScheduledMessages, Message *&InlineResponseMessage)
{
  int Status = ERROR;
  string Offset = "                    ";
  unsigned int NA = 0;
  vector<string> Values;

  //PB->S << Offset <<  this->GetLegibleName() << endl;

  // Load the number of arguments
  if (_PCL->GetNumberofArguments (NA) == OK)
	{
	  // Check the number of arguments
	  if (NA == 1)
		{
		  // Get received command line argument
		  _PCL->GetArgument (0, Values);

		  if (Values.size () > 0)
			{
			  // The additional information is related to the message payload
			  if (_PCL->Alternative == "--payload")
				{
				  // Extract the payload from the received msg char array
				  //_ReceivedMessage->ExtractPayloadCharArrayFromMessageCharArray();

				  // Verifies the payload flag
				  if (_ReceivedMessage->GetHasPayloadFlag () == true)
					{
					  string PayloadPath = PB->GetPath ();

					  PB->S << endl << Offset << "(Received " << Values.at (0) << ")" << endl;
					  //PB->S << Offset <<  "(Saving the payload on the path "<<PayloadPath<<")" << endl;

					  _ReceivedMessage->SetPayloadFileName (Values.at (0));
					  _ReceivedMessage->SetPayloadFilePath (PayloadPath);
					  _ReceivedMessage->SetPayloadFileOption ("BINARY");
					  _ReceivedMessage->ConvertPayloadFromCharArrayToFile ();

					  generateReceivedMessageReport(Values.at (0), _ReceivedMessage->GetInstantiationTime());

					  // Note: The payload is going to be saved as a file on the specified path

					  /*
					  PB->S << Offset <<  "(Testing payload integrity)" << endl;

					  long long int PayloadSize=0;

					  _ReceivedMessage->GetPayloadSize(PayloadSize);

					  File *F1=new File;

					  cout << " Opening original payload file. The status is (0=success, 1=fail) = " << F1->OpenInputFile("SOA4All in the Future Internet of Services.mp4",PayloadPath,"BINARY") << endl;

					  char			*buffer;
					  long			length=0;

					  length = F1->tellg();

					  F1->seekg (0, ios::beg);

					  buffer = new char [length];

					  F1->read(buffer,length);

					  F1->close();

					  PB->S << "(Original            Received)" << endl;

					  for (unsigned int i=0;i<PayloadSize;i++)
						  {
							  printf("%i %d %c            ",i,(unsigned char)buffer[i],(unsigned char)buffer[i]);

							  printf("%i %d %c",i,(unsigned char)_ReceivedMessage->Payload[i],(unsigned char)_ReceivedMessage->Payload[i]);

							  if (buffer[i] != _ReceivedMessage->Payload[i])
								  {
									  printf("%s ","                Error detected");
								  }

							  printf("\n");
						  }
					  */
					}
				}
			}
		}
	}

  //PB->S << Offset <<  "(Done)" << endl << endl << endl;

  return Status;
}

void HTInfoPayload01::generateReceivedMessageReport(string _FileName, double _Time)
{
	PB->S << "(Adding file " << _FileName << " to the publish list report)" << endl;

	ifstream reportFile;
	reportFile.open (PB->GetPath () + "ReceivedMessagesReport.json");
	string reportContent;
	if(reportFile)
	{
		string line;
		while(getline(reportFile,line))
		{
			reportContent += line;
		}
		reportFile.close();	
	}
	else
	{
		reportContent = "{\"ReceivedMessages\":[]}";
	}

	string newReport = "{\"FileName\":\"" + _FileName + "\",\"Time\":" + PB->DoubleToString(_Time) + "}";

	Document document;
	document.Parse(reportContent.c_str());
	
	Value& publishedMessages = document["ReceivedMessages"];
	
	Document newReportDocument;
	newReportDocument.Parse(newReport.c_str());
	publishedMessages.PushBack(newReportDocument, document.GetAllocator());
	
	PB->S << "(Received messages report created)" << endl;

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	document.Accept(writer);
	ofstream newReportFile;
	newReportFile.open (PB->GetPath () + "ReceivedMessagesReport.json");
	newReportFile << buffer.GetString();
	newReportFile.close();
}
