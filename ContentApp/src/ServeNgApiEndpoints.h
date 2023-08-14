/*
    NovaGenesis

    Name:       Listen
    Object:     Listen
    File:       Listen.h
    Author:     William Mamede
    Date:       05/2022
    Version:    0.1

    Copyright (C) 2022 Antonio Marcos Alberti

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

#ifdef OK
#undef OK
#endif

#ifndef LISTEN_H
#define LISTEN_H

#include <cpprest/http_listener.h>
#include <cpprest/filestream.h>
#include <cpprest/rawptrstream.h>

#ifndef _IOSTREAM_H
#include <iostream>
#endif

#ifndef _GLIBCXX_IOMANIP
#include <iomanip>
#endif

#ifndef _SSTREAM_H
#include <sstream>
#endif

#include <thread>
#include <chrono>
#include <ctime>

#ifndef _FSTREAM_H
#include <fstream>
#endif

#include <curl/curl.h>

#include <filesystem>

using namespace std;
using namespace web::http::experimental::listener;
using namespace web::http;
using namespace web;
using namespace utility;
using namespace concurrency::streams;

class ServeNgApiEndpoints
{
public:
    ServeNgApiEndpoints();
    ~ServeNgApiEndpoints();
    void start();

private:
    void respond(const http_request &request, const status_code &status, const json::value &response);
    void respondImage(http_request &request, const status_code &status, const string &imagePath);
    void replyImage(const http_request &request, const status_code &status, const concurrency::streams::istream &imageStream);
    void handleWebRequest(const http_request &request, const string &url);
    void createNGPageRequest(const string &url);
    web::json::value getBindingsReport(string requestedBlock);
    web::json::value getServiceOffers();
    web::json::value getPublishedMessages();
    web::json::value getReceivedMessages();
    web::json::value getLifecycle();
    string getUrlAsHash(string url);
    // static write and header callbacks
    static size_t WriteCallback(char *contents, size_t size, size_t nmemb, std::string *response);
    static size_t HeaderCallback(char *contents, size_t size, size_t nmemb, std::string *response);
    bool isPagePublished(string url);
    CURL* curl;
};

#endif // LISTEN_H
#ifdef OK
#undef OK
#define OK 0
#endif