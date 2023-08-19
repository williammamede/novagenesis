/*
    NovaGenesis

    Name:		Simple application process
    Object:		App
    File:		App.h
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

#ifndef _NGAPI_H
#define _APP_H

#include "Process.h"

#define ERROR 1
#ifndef OKAY
#define OKAY 0
#endif

class NGApi : public Process
{
public:
    // Application role ("Source" or "Repository")
    string Role;

    // Constructor
    NGApi(string _LN, string _Role, key_t _Key, string _Path);

    // Destructor
    ~NGApi();

    // Allocate a new block based on a name and add a Block on Blocks container
    int NewBlock(string _LN, Block *&_PB);

    // Execute NG process
    void executeNGProcess(string _Role);

    // Execute NGApi endpoint provided by serveNGApiEndpoints
    void executeNGApiEndpoint();
};

#endif
