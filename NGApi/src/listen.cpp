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
#include <cpprest/filestream.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <thread>
#include <chrono>
#include <ctime>
#include <fstream>

// cpprest provides macros for all streams but std::clog in basic_types.h
#ifdef _UTF16_STRINGS
// On Windows, all strings are wide
#define uclog std::wclog
#else
// On POSIX platforms, all strings are narrow
#define uclog std::clog
#endif // endif _UTF16_STRINGS
#define BASE std::getenv("BASE")

using namespace std;
using namespace web::http::experimental::listener;
using namespace web::http;
using namespace web;
using namespace utility;
using namespace concurrency::streams;

void respond(const http_request &request, const status_code &status, const json::value &response)
{
    json::value resp;
    resp[U("status")] = json::value::number(status);
    resp[U("response")] = response;
    // add header access control allow origin
    resp[U("headers")][U("Access-Control-Allow-Origin")] = json::value::string(U("http://127.0.0.1:10102"));

    request.reply(status, resp);
}

void respondImage(const http_request &request, const status_code &status, const string &imagePath)
{
    auto fileStream = std::make_shared<Concurrency::streams::ostream>();

    utility::string_t file = imagePath;
    *fileStream = Concurrency::streams::fstream::open_ostream(file, std::ios::out | std::ios::trunc).get();
    request.body().read_to_end(fileStream->streambuf()).get();
    request.reply(status);
}

void replyImage(const http_request &request, const status_code &status, const concurrency::streams::istream &imageStream)
{
    request.reply(status, imageStream, U("image/jpeg"));
}

/**
 * @brief Get the Bindings Report object
 * 
 * @param requestedBlock The block to get the bindings from
 * @return web::json::value The bindings report
 */
web::json::value getBindingsReport(string requestedBlock)
{
    web::json::value output;
    string path = std::string(BASE) + "/IO/" + requestedBlock + "/HTbindings.json";
    uclog << "Reading bindings from " << requestedBlock << endl;
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
        output = json::value::string("Error reading bindings from " + requestedBlock);
        uclog << "Error reading bindings from " << requestedBlock << endl;
        uclog << e.what() << endl;
    }

    return output;
}

/**
 * @brief Get the Service Offers object
 * 
 * @return web::json::value The service offers
 */
web::json::value getServiceOffers()
{
    web::json::value output;
    string path = std::string(BASE) + "/IO/Source1/ServiceOfferReport.json";
    uclog << "Reading service offers" << path << endl;
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
        output = json::value::string("Error reading service offers");
        uclog << "Error reading service offers" << endl;
        uclog << e.what() << endl;
    }

    return output;
}

/**
 * @brief Get the Published messages report
 * 
 * @return web::json::value The published messages report
 */
web::json::value getPublishedMessages()
{
    web::json::value output;
    string path = std::string(BASE) + "/IO/Source1/PublishedMessagesReport.json";
    uclog << "Reading published messages" << path << endl;
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
        output = json::value::string("Error reading published messages");
        uclog << "Error reading published messages" << endl;
        uclog << e.what() << endl;
    }

    return output;
}


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
            req.headers().add(U("Access-Control-Allow-Origin"), U("*"));
            req.headers().add(U("Allow"), U("GET, POST, OPTIONS"));
            req.headers().add(U("Access-Control-Allow-Origin"), U("*"));
            req.headers().add(U("Access-Control-Allow-Methods"), U("GET, POST, OPTIONS"));

            auto http_uri_sub_dir = req.request_uri().path();
            uclog << U("Received request for: ") << http_uri_sub_dir << endl;

            if (http_uri_sub_dir == U("/serviceOffers")) {
                respond(req, status_codes::OK, getServiceOffers());
                return;
            } else if (http_uri_sub_dir == U("/bindings")) {
                auto http_get_vars = uri::split_query(req.request_uri().query());

                auto requestedBinding = http_get_vars.find(U("requestedBinding"));

                if (requestedBinding == end(http_get_vars)) {
                    auto err = U("No requestedBinding found in the query string. [e.g ?requestedBinding=<NRNCS>]");
                    uclog << err << endl;
                    respond(req, status_codes::BadRequest, json::value::string(err));
                    return;
                }

                auto blockName = requestedBinding->second;
                uclog << U("Received requestedBinding: ") << blockName << endl;
                respond(req, status_codes::OK, getBindingsReport(blockName));
            } else if (http_uri_sub_dir == U("/transmitedImage")) {
                auto http_get_vars = uri::split_query(req.request_uri().query());
                auto requestedImage = http_get_vars.find(U("requestedImage"));

                if (requestedImage == end(http_get_vars)) {
                    auto err = U("No requestedImage found in the query string. [e.g ?requestedImage=<Image name>]");
                    uclog << err << endl;
                    respond(req, status_codes::BadRequest, json::value::string(err));
                    return;
                }

                auto imageName = requestedImage->second;
                uclog << U("Received requestedImage: ") << imageName << endl;
                respondImage(req, status_codes::OK, std::string(BASE) + "/IO/Source1/" + imageName);
            } else if (http_uri_sub_dir == U("/publishedMessages")) {
                respond(req, status_codes::OK, getPublishedMessages());
            }

            respond(req, status_codes::NotFound, json::value::string(U("URI not found. Try /serviceOffers or /bindings")));
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