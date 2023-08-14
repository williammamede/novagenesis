/*
	NovaGenesis

	Name:		ServerNgApiEndpoints
	Object:		ServerNgApiEndpoints
	File:		ServerNgApiEndpoints.cpp
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

#ifndef _LISTEN_H
#include "ServeNgApiEndpoints.h"
#endif

// cpprest provides macros for all streams but std::clog in basic_types.h
#ifdef _UTF16_STRINGS
// On Windows, all strings are wide
#define uclog std::wclog
#else
// On POSIX platforms, all strings are narrow
#define uclog std::clog
#endif // endif _UTF16_STRINGS

#ifndef BASE
#define BASE std::getenv("BASE")
#endif

// Constant for status codes ok
const auto OKAY = status_codes::OK;

//contructor
ServeNgApiEndpoints::ServeNgApiEndpoints()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);

    //execute the server start
    start();
}

//destructor
ServeNgApiEndpoints::~ServeNgApiEndpoints()
{
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}

// Methor to serve the API endpoints
void ServeNgApiEndpoints::start()
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
    listener.support(methods::GET, [this](http_request req)
        {
            req.headers().add(U("Access-Control-Allow-Origin"), U("*"));
            req.headers().add(U("Allow"), U("GET, POST, OPTIONS"));
            req.headers().add(U("Access-Control-Allow-Origin"), U("*"));
            req.headers().add(U("Access-Control-Allow-Methods"), U("GET, POST, OPTIONS"));

            auto http_uri_sub_dir = req.request_uri().path();
            uclog << U("Received request for: ") << http_uri_sub_dir << endl;

            if (http_uri_sub_dir == U("/serviceOffers")) {
                respond(req, OKAY, getServiceOffers());
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
                respond(req, OKAY, getBindingsReport(blockName));
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
                respondImage(req, OKAY, std::string(BASE) + "/IO/Source1/" + imageName);
            } else if (http_uri_sub_dir == U("/publishedMessages")) {
                respond(req, OKAY, getPublishedMessages());
            } else if (http_uri_sub_dir == U("/receivedMessages")) {
                respond(req, OKAY, getReceivedMessages());
            } else if (http_uri_sub_dir == U("/lifecycle")) {
                respond(req, OKAY, getLifecycle());
            } 
            // Else it must be a web page request
            else {
                auto path = http_uri_sub_dir.substr(1);
                uclog << U("Received request for WEB Page: ") << path << endl;
                handleWebRequest(req, path);
            }
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
}

void ServeNgApiEndpoints::respond(const http_request &request, const status_code &status, const json::value &response)
{
    request.reply(status, response);
}

void ServeNgApiEndpoints::respondImage(http_request &request, const status_code &status, const string &imagePath)
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

void ServeNgApiEndpoints::replyImage(const http_request &request, const status_code &status, const concurrency::streams::istream &imageStream)
{
    request.reply(status, imageStream, U("image/jpeg"));
}

/**
 * @brief Handle the web page request
 * 
 * @param request The request
 * @param path The path of the web page
 */
void ServeNgApiEndpoints::handleWebRequest(const http_request &request, const string &path)
{
    std::string hashUrl = getUrlAsHash(path);
    std::string response;
    std::string headers;
    if (isPagePublished(hashUrl)) {
        // The page is already published, this uncompress the page and headers and send it back
        std::string compressedFilePath = std::string(BASE) + "/IO/Repository1/" + hashUrl + ".zip";
        // Uncompress the file to a temporary directory
        std::string tempDir = std::string(BASE) + "/IO/Repository1/" + hashUrl;

        // Verify if the temporary directory does not exists
        if (!std::filesystem::exists(tempDir)) {
            std::string command = "unzip -o " + compressedFilePath + " -d " + tempDir;
            std::system(command.c_str());

            // Waint until the directory is created
            while (!std::filesystem::exists(tempDir)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }

        // Read the index.html file
        std::ifstream file(tempDir + "/" + hashUrl + ".html");
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        response = buffer.str();
        // Read the headers file
        std::ifstream fileHeaders(tempDir + "/headers.txt");

        // The header file should have the following pattern:
        // HTTP/1.1 200 OK
        // Date: Mon, 27 Jul 2009 12:28:53 GMT
        // Server: Apache/2.2.14 (Win32)
        // Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT
        // Content-Length: 88
        // Content-Type: text/html
        // Connection: Closed

        std::stringstream bufferHeaders;
        bufferHeaders << fileHeaders.rdbuf();
        fileHeaders.close();
        headers = bufferHeaders.str();
        
        http_response responseHttp(OKAY);

        std::istringstream iss(headers);
        std::string line;

        while (std::getline(iss, line)) {
            auto pos = line.find(':');
            if (pos != std::string::npos) {
                auto headerName = line.substr(0, pos);
                auto headerValue = line.substr(pos + 1);
                // Remove the \r at the end of the string
                headerValue.erase(std::remove(headerValue.begin(), headerValue.end(), '\r'), headerValue.end());
                // Remove the space at the beginning of the string
                headerValue.erase(headerValue.begin(), std::find_if(headerValue.begin(), headerValue.end(), [](int ch) {
                    return !std::isspace(ch);
                }));
                responseHttp.headers().add(U(headerName.c_str()), U(headerValue.c_str()));
            }
        }

        responseHttp.set_body(response);
        request.reply(responseHttp);
    } else {
        // The page is not published yet, so request it from the NG network if path diferent from favicon.ico
        if(strcmp(path.c_str(), "favicon.ico") != 0) {
            createNGPageRequest(path);
        }

        // Respond with the errorPage.html from Repository1
        std::string errorPagePath = std::string(BASE) + "/IO/Repository1/errorPage.html";
        std::ifstream file(errorPagePath);
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        response = buffer.str();
        
        http_response responseHttp(OKAY);
        responseHttp.headers().add(U("Content-Type"), U("text/html"));
        responseHttp.set_body(response);
        request.reply(responseHttp);
    }
}

/**
 * @brief Create a NG page request
 * 
 * @param path The path of the web page
 */
void ServeNgApiEndpoints::createNGPageRequest(const string &path)
{
    // Create a file in source1 with the path of the web page
    std::string filePath = std::string(BASE) + "/IO/Repository1/webRequest" + getUrlAsHash(path) + ".json";
    std::ofstream file(filePath);
    file << "{\"path\": \"" << path << "\"}";
    file.close();
}

/**
 * @brief Get the Bindings Report object
 * 
 * @param requestedBlock The block to get the bindings from
 * @return web::json::value The bindings report
 */
web::json::value ServeNgApiEndpoints::getBindingsReport(string requestedBlock)
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
web::json::value ServeNgApiEndpoints::getServiceOffers()
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
web::json::value ServeNgApiEndpoints::getPublishedMessages()
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
web::json::value ServeNgApiEndpoints::getReceivedMessages()
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
web::json::value ServeNgApiEndpoints::getLifecycle()
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
 * @brief Header callback for web page requests, that will be included in restcppsdk response
 * 
 * @return size_t The size of the header
 */
size_t ServeNgApiEndpoints::HeaderCallback(char* contents, size_t size, size_t nmemb, std::string* response)
{
    size_t totalSize = size * nmemb;
    response->append(contents, totalSize);
    
    return totalSize;
}

/**
 * @brief Write callback for web page requests
 * 
 * @return size_t The size of the response
 */ 
size_t ServeNgApiEndpoints::WriteCallback(char* contents, size_t size, size_t nmemb, std::string* response)
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
                    // Set the same callback function for the redirect response
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

/**
 * @brief Get the URL name as hash
 * 
 * @param url The URL to hash
 * 
 * @return string The hash of the URL
 */
 string ServeNgApiEndpoints::getUrlAsHash(string url)
 {
     std::hash<std::string> hasher;
     return std::to_string(hasher(url));
 }

/**
 * @brief Check if the page is already published
 * 
 * @param hashUrl The hash of the URL
 * 
 * @return true The page is already published
 */
bool ServeNgApiEndpoints::isPagePublished(string hashUrl)
{
    return std::filesystem::exists(std::string(BASE) + "/IO/Repository1/" + hashUrl + ".zip");
}

