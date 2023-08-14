/*
	NovaGenesis

	Name:		Simple application process
	Object:		App
	File:		App.h
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

#ifndef _CONTENTAPP_H
#include "ContentApp.h"
#endif

#ifndef _CORE_H
#include "Core.h"
#endif

#ifndef LISTEN_H
#include "ServeNgApiEndpoints.h"
#endif

#define OK 0

#include <thread>

ContentApp::ContentApp (string _LN, string _Role, key_t _Key, string _Path) : Process (_LN, _Key, _Path)
{
	Block *PCoreB = 0;
    string CoreLN = "Core";
    Role = _Role;

    NewBlock (CoreLN, PCoreB);
	// If the role is Source executeNGApiEndpoint will not be called
	if (_Role == "Repository")
	{
		std::thread ngThread(&ContentApp::executeNGProcess, this);
		std::thread apiThread(&ContentApp::executeNGApiEndpoint, this);

		ngThread.join();
		apiThread.join();
	} else {
		std::thread ngThread(&ContentApp::executeNGProcess, this);
		ngThread.join();
	}
}

ContentApp::~ContentApp ()
{
}

void ContentApp::executeNGProcess()
{
    // Run the base class GW
    RunGateway();
}

void ContentApp::executeNGApiEndpoint()
{
    ServeNgApiEndpoints serveNgApiEndpoints;
}

// Allocate a new block based on a name and add a Block on Blocks container
int ContentApp::NewBlock (string _LN, Block *&_PB)
{
  if (_LN == "Core")
	{
	  Block *PGWB = 0;
	  Block *PHTB = 0;
	  GW *PGW;
	  HT *PHT;
	  string GWLN = "GW";
	  string HTLN = "HT";

	  GetBlock (GWLN, PGWB);

	  PGW = (GW *)PGWB;

	  GetBlock (HTLN, PHTB);

	  PHT = (HT *)PHTB;

	  unsigned int Index;

	  Index = GetBlocksSize ();

	  Core *PCore = new Core (_LN, this, Index, PGW, PHT, GetPath ());

	  _PB = (Block *)PCore;

	  InsertBlock (_PB);

	  return OK;
	}

  return ERROR;
}



