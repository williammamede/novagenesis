/*
    NovaGenesis

    Name:       AiConnector
    Object:     AiConnector
    File:       AiConnector.h
    Author:     William Mamede
    Date:       05/2024
    Version:    0.1

    Copyright (C) 2023 Antonio Marcos Alberti

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

#ifndef IO_CONNECTOR_H
#define IO_CONNECTOR_H

#ifndef _IOSTREAM_H
#include <iostream>
#endif

#include <string>
#include <curl/curl.h>

#include <fstream>
#include <filesystem>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

using namespace std;
using namespace rapidjson;

class AiConnector
{
public:
    AiConnector();
    ~AiConnector();
    string analyzeSystemTuples();
private:
    string openAndParseFile(string filePath);
    string sendJsonToAi(string request);
    static size_t WriteCallback(char* contents, size_t size, size_t nmemb, std::string* response);
    string extractContentFromResponse(string response);
};
#endif