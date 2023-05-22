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
#include <cpprest/rawptrstream.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <thread>
#include <chrono>
#include <ctime>
#include <fstream>
#include <curl/curl.h>


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
    request.reply(status, response);
}

void respondImage(http_request &request, const status_code &status, const string &imagePath)
{
    /* auto fileStream = std::make_shared<Concurrency::streams::ostream>();

    utility::string_t file = imagePath;
    *fileStream = Concurrency::streams::fstream::open_ostream(file, std::ios::out | std::ios::trunc).get();
    request.set_body(*fileStream,  */
    http_response response(status);
    response.headers().add(U("Content-Type"), U("image/png"));
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    
    auto fileStream = std::make_shared<Concurrency::streams::istream>(Concurrency::streams::file_stream<uint8_t>::open_istream(imagePath).get());
    response.set_body(*fileStream);
    
    request.reply(response);
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

/**
 * @brief Get the Subscribed messages report
 * 
 * @return web::json::value The subscribed messages report
 */
web::json::value getReceivedMessages()
{
    web::json::value output;
    string path = std::string(BASE) + "/IO/NRNCS/ReceivedMessagesReport.json";
    uclog << "Reading received messages" << path << endl;
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
        output = json::value::string("Error reading received messages");
        uclog << "Error reading received messages" << endl;
        uclog << e.what() << endl;
    }

    return output;
}

/**
 * @brief Get the lifecycle, the service status
 * 
 * @return web::json::value The services status
 */
web::json::value getLifecycle()
{
    web::json::value output;
    string path = std::string(BASE) + "/IO/PGCS/ApplicationLifecycleReport.json";
    uclog << "Reading lifecycle" << path << endl;
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
        output = json::value::string("Error reading service lifecycle");
        uclog << "Error reading service lifecycle" << endl;
        uclog << e.what() << endl;
    }

    return output;
}

/**
 * @brief Write callback for web page requests
 * 
 * @return size_t The size of the response
 */ 
size_t WriteCallback(char* contents, size_t size, size_t nmemb, std::string* response)
{
    size_t totalSize = size * nmemb;
    response->append(contents, totalSize);
    
    // Check if the response indicates a redirect
    if (totalSize >= 17 && response->substr(9, 8) == "301 Moved") {
        size_t redirectStartPos = response->find("Location: ");
        if (redirectStartPos != std::string::npos) {
            size_t redirectEndPos = response->find("\r\n", redirectStartPos);
            if (redirectEndPos != std::string::npos) {
                std::string redirectUrl = response->substr(redirectStartPos + 10, redirectEndPos - redirectStartPos - 10);
                
                // Perform a new request to the redirect URL
                CURL* curlRedirect = curl_easy_init();
                std::string redirectResponse;
                
                if (curlRedirect) {
                    curl_easy_setopt(curlRedirect, CURLOPT_URL, redirectUrl.c_str());
                    curl_easy_setopt(curlRedirect, CURLOPT_WRITEFUNCTION, WriteCallback);
                    curl_easy_setopt(curlRedirect, CURLOPT_WRITEDATA, &redirectResponse);
                    CURLcode resRedirect = curl_easy_perform(curlRedirect);
                    
                    if (resRedirect != CURLE_OK) {
                        std::cerr << "Error: " << curl_easy_strerror(resRedirect) << std::endl;
                    }
                    
                    curl_easy_cleanup(curlRedirect);
                }
                
                // Replace the original response with the redirect response
                *response = redirectResponse;
            }
        }
    }
    
    return totalSize;
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
            } else if (http_uri_sub_dir == U("/receivedMessages")) {
                respond(req, status_codes::OK, getReceivedMessages());
            } else if (http_uri_sub_dir == U("/lifecycle")) {
                respond(req, status_codes::OK, getLifecycle());
            } 
            // Else if contains www on it
            else if (http_uri_sub_dir.find(U("www")) != std::string::npos) {
                auto path = http_uri_sub_dir.substr(1);
                uclog << U("Received request for WEB Page: ") << path << endl;

                // Get with CURL the web page from the given path
                CURL* curl = curl_easy_init();
                std::string response;

                if (curl) {
                    // Set the URL to the desired web page
                    curl_easy_setopt(curl, CURLOPT_URL, path.c_str());

                    // Set the callback function to handle the response
                    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);  // Enable automatic redirect following
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

                    // Perform the request
                    CURLcode res = curl_easy_perform(curl);

                    // Check for errors
                    if (res != CURLE_OK) {
                        std::cerr << "Error: " << curl_easy_strerror(res) << std::endl;
                    }

                    // Clean up
                    curl_easy_cleanup(curl);
                }

                http_response responseHttp(status_codes::OK);
                responseHttp.headers().add(U("Content-Type"), U("text/html"));
                responseHttp.set_body(response);
                req.reply(responseHttp);
            }
            else

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