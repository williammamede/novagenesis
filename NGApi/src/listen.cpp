/*
	NovaGenesis

	Name:		Listen
	Object:		Listen
	File:		Listen.cpp
	Author:		William Mamede
	Date:		05/2022
	Version:	0.1

   	Copyright (C) 2022  Antonio Marcos Alberti

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
#include <cpprest/http_listener.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <thread>
#include <chrono>
#include <ctime>

// cpprest provides macros for all streams but std::clog in basic_types.h
#ifdef _UTF16_STRINGS
// On Windows, all strings are wide
#define uclog std::wclog
#else
// On POSIX platforms, all strings are narrow
#define uclog std::clog
#endif // endif _UTF16_STRINGS

using namespace std;
using namespace web::http::experimental::listener;
using namespace web::http;
using namespace web;

void respond(const http_request &request, const status_code &status, const json::value &response)
{
    json::value resp;
    resp[U("status")] = json::value::number(status);
    resp[U("response")] = response;

    request.reply(status, resp);
}

// Returns the Bindings for the given request
web::json::value getBindingsReport(string requestedBindings)//rename to block
{
    web::json::value output;
    // TODO: improve the way path is handled
    string path = "/mnt/c/Users/williamsm/Documents/personal_workspace/novagenesis/IO/NRNCS/" + requestedBindings + "bindings.json";
    uclog << "Reading bindings from " << requestedBindings << endl;
    try
    {
        std::ifstream file(path);
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        output = json::value::parse(buffer.str());
    }
    catch (const json::json_exception &e)
    {
        output = json::value::string("Error reading bindings from " + requestedBindings);
        uclog << "Error reading bindings from " << requestedBindings << endl;
        uclog << e.what() << endl;
    }

    return output;;
}

// main function responsible for handling requests
int main()
{
    // Synchronously bind the listener to all nics.
    uclog << U("Starting NGApi.") << endl;
    const auto uri = uri_builder{}
                         .set_scheme("http")
                         .set_host("0.0.0.0")
                         .set_port(10102)
                         .set_path("/")
                         .to_uri();
    http_listener listener(uri);
    listener.open().wait();

    // Handle incoming requests.
    uclog << U("Setting up.") << endl;
    listener.support(methods::GET, [](http_request req)
        {
            auto http_get_vars = uri::split_query(req.request_uri().query());

            auto requestedBinding = http_get_vars.find(U("requestedBinding"));

            if (requestedBinding == end(http_get_vars)) {
                auto err = U("No requestedBinding found in the query string. [e.g ?requestedBinding=<HT|NR|GW>]");
                uclog << err << endl;
                respond(req, status_codes::BadRequest, json::value::string(err));
                return;
            }

            auto bindingName = requestedBinding->second;
            uclog << U("Received requestedBinding: ") << bindingName << endl;
            respond(req, status_codes::OK, getBindingsReport(bindingName)); 
        });

    // Wait while the listener does the heavy lifting.
    uclog << U("Waiting for incoming connection...") << endl;
    while (true)
    {
        this_thread::sleep_for(chrono::milliseconds(2000));
    }

    // Nothing left to do but commit suicide.
    uclog << U("Terminating NGApi.") << endl;
    listener.close();
    return 0;
}