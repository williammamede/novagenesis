/*
	NovaGenesis

	Name:		NovaGenesis Gateway API, executable for provide rest API fo interoperate WEB applications with NovaGenesis
	Object:		NGApi.cpp
	File:		NGApi.h
	Author:		William Mamede
	Date:		05/2022
	Version:	0.1

	Copyright (C) 2023  Antonio Marcos Alberti

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

#include "NGApi.h"

#ifndef _TIME_H
#include <time.h>
#endif

#ifndef _SYS_TIME_H
#include <sys/time.h>
#endif

int main(int argc, char *argv[])
{
	int R = 0;
	bool Problem = false;

	if (argc != 0)
	{
		if (argc == 3)
		{
			string Path = argv[1];

			cout << "*******************************************************************" << endl;
			cout << "*                                                                 *" << endl;
			cout << "*  NovaGenesis(NG) Content Distribution Application v0.2          *" << endl;
			cout << "*  Copyright Antonio Marcos Alberti - Inatel - May 2023         *" << endl;
			cout << "*                                                                 *" << endl;
			cout << "*                                                                 *" << endl;
			cout << "*******************************************************************" << endl
				 << endl;

			cout << "(The I/O path is " << Path << ")" << endl;

			string Temp1 = argv[2];

			if (Temp1 == "Source" || Temp1 == "Repository")
			{
				cout << "(This is a NG Web" << Temp1 << " process.)" << endl;

				long long Temp = (long long)&R;

				// Initialize the random generator
				srand((unsigned int)Temp * time(NULL));

				// Generates a random key
				R = 1 + (rand() % 2147483647);

				// Set the shm key
				key_t Key = R;

				// Create a process instance
				NGApi execApp("NGApi", Temp1, Key, Path);
			}
			else
			{
				Problem = true;

				cout << "(ERROR: Empty role)" << endl;
			}
		}
		else
		{
			Problem = true;

			cout << "(ERROR: Wrong number of main() arguments)" << endl;
		}
	}
	else
	{
		Problem = true;

		cout << "(ERROR: No argument supplied)" << endl;
	}

	if (Problem == true)
	{
		cout << "(Usage: ./NGApi Path Role)" << endl;
		cout << "(Path example: /home/myprofile/workspace/novagenesis/IO/Client1/)" << endl;
	}

	return 0;
}
