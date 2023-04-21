/*
	NovaGenesis

	Name:		Name Resolution and Network Cache Service
	Object:		execNRNCS
	File:		execNRNCS.cpp
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

#include "NRNCS.h"

#ifndef _TIME_H
#include <time.h>
#endif

#ifndef _SYS_TIME_H
#include <sys/time.h>
#endif

#include <chrono>
#include <ctime>
#include <sstream>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include <stdio.h>
#include <string.h>

using namespace rapidjson;

void generateApplicationLifecycleReport(string Path, string _Time)
{
	cout << "(Generating the application lifecycle report)" << endl;

	// Read json file that stores the system state
	cout << "(Reading the json file that will store the system state)" << endl;

	ifstream reportFile;
	reportFile.open (Path + "ApplicationLifecycleReport.json");
	string reportContent = "";
	if (reportFile.is_open())
	{
		cout << "(The json file that will store the system state was successfully opened)" << endl;
		string line;
		while (getline(reportFile, line))
		{
			reportContent += line;
		}

		cout << "(The json file that will store the system state was successfully read)" << endl;
	}
	reportFile.close();

	if (reportContent == "")
	{
		cout << "(No information was found in the json file that will store the system state)" << endl;
		return;
	}

	Document document;
	document.Parse(reportContent.c_str());

	Value& serviceStates = document["serviceStates"];

	for (SizeType i = 0; i < serviceStates.Size(); i++)
	{
		Value& serviceState = serviceStates[i];
		if (std::string(serviceState["serviceName"].GetString()) == "NRNCS")
		{
			cout << "(Updating the serviceState with running)" << endl;
			serviceState["serviceState"].SetString("running");

			cout << "(Updating the serviceState with time)" << endl;
			serviceState["time"].SetString(_Time.c_str(), _Time.length(), document.GetAllocator());
		}
	}

	cout << "(Writing the json file that will store the system state)" << endl;
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	document.Accept(writer);
	ofstream reportFile2;
	reportFile2.open (Path + "ApplicationLifecycleReport.json");
	reportFile2 << buffer.GetString();
	if (reportFile2.good())
	{
		cout << "(The json file that will store the system state was successfully written)" << endl;
	}
	else
	{
		cout << "(The json file that will store the system state was not successfully written)" << endl;
	}
	reportFile2.close();
}

string getTime() {
    // Get the current system time
    auto now = std::chrono::system_clock::now();
    
    // Convert the system time to a time_t object
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    
    // Convert the time_t object to a string
    std::stringstream ss;
    ss << std::ctime(&time);
    std::string timeStr = ss.str();
    
    // Remove the newline character from the end of the string
    timeStr = timeStr.substr(0, timeStr.length() - 1);
    
    return timeStr;
}

int main (int argc, char *argv[])
{
  int R = 0;
  bool Problem = false;

  if (argc != 0)
	{
	  if (argc == 2)
		{
		  string Path = argv[1];

		  cout << "*******************************************************************" << endl;
		  cout << "*                                                                 *" << endl;
		  cout << "*  NovaGenesis(NG) Name Resolution and Network Cache Service v0.1 *" << endl;
		  cout << "*  Copyright Antonio Marcos Alberti - Inatel - April 2021         *" << endl;
		  cout << "*                                                                 *" << endl;
		  cout << "*                                                                 *" << endl;
		  cout << "*******************************************************************" << endl << endl;

		  cout << "(The I/O path is " << Path << ")" << endl;

		  long long Temp = (long long)&R;

		  // Initialize the random generator
		  srand ((unsigned int)Temp * time (NULL));

		  // Generates a random key
		  R = 1 + (rand () % 2147483647);

		  // Set the shm key
		  key_t Key = R;

		  // Update lifecycle report
		  string _Time = getTime();
		  generateApplicationLifecycleReport("/mnt/c/Users/williamsm/Documents/personal_workspace/novagenesis/IO/PGCS/", _Time);

		  // Create a process instance
		  NRNCS execNRS ("NRNCS", Key, Path);
		}
	  else
		{
		  cout << "(ERROR: Wrong number of main() arguments)" << endl;

		  Problem = true;
		}
	}
  else
	{
	  cout << "(ERROR: No argument supplied)" << endl;

	  Problem = true;
	}

  if (Problem == true)
	{
	  cout << "(Usage: ./NRNCS Path)" << endl;
	  cout << "(Path example: /home/myprofile/workspace/novagenesis/IO/NRNCS/)" << endl;
	}

  return 0;
}




