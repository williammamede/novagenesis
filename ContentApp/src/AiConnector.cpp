/*
    NovaGenesis

    Name:       IoConnector
    Object:     IoConnector
    File:       IoConnector.cpp
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

#ifndef AI_CONNECTOR_CPP
#include "AiConnector.h"
#endif

#ifndef BASE
#define BASE std::getenv("BASE")
#endif

#define GEMINE_AI_API_KEY std::getenv("GOOGLE_API_KEY")
#define GEMINE_AI_URL_1_5 "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-pro:generateContent"
#define GEMINE_AI_URL "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent"

#include <string>
#include <curl/curl.h>

#include <fstream>
#include <filesystem>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

using namespace std;
using namespace rapidjson;


AiConnector::AiConnector()
{
}

AiConnector::~AiConnector()
{
}

/**
 * @brief Analyze system tuples with Gemine AI
 * 
 */
string AiConnector::analyzeSystemTuples()
{
    cout << "Analyzing system tuples with Gemine AI..." << endl;

    // Define the Json message to be sent to the AI
    string jsonMessageString = "{\"contents\": [{\"parts\": [{\"text\": \"" 
        + openAndParseFile(std::string(BASE) + "/IO/Repository1/Part1.txt") 
        + "\"}, {\"text\": \"" 
        + openAndParseFile(std::string(BASE) + "/IO/NRNCS/HTBindings.json") 
        + "\"}, {\"text\": \"" 
        + openAndParseFile(std::string(BASE) + "/IO/Repository1/SystemProcessTuplesContentApp.json") 
        + "\"}]}],\"generationConfig\":{\"response_mime_type\":\"application/json\"}}";

    // Save the request in a file
    ofstream requestFile(std::string(BASE) + "/IO/Repository1/Request.json");
    requestFile << jsonMessageString;
    requestFile.close();

    string response = sendJsonToAi(jsonMessageString);

    // Extract the content from the response
    string content = extractContentFromResponse(response);

    // Save the response in a file
    ofstream responseFile(std::string(BASE) + "/IO/Repository1/AiResponse.json");
    responseFile << content;
    responseFile.close();

    return content;
}

/**
 * @brief Open and parse a file
 * 
 * @param filePath 
 * @return string 
 */
string AiConnector::openAndParseFile(string filePath)
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        cout << "Error opening file " << filePath << endl;
        return "";
    }

    string fileContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    // Replace " with \" to escape the quotes
    size_t pos = 0;
    while ((pos = fileContent.find("\"", pos)) != std::string::npos)
    {
        fileContent.replace(pos, 1, "\\\"");
        pos += 2; // Skip past the newly inserted characters
    }

    // store a copy of the file content in a file for testing
    ofstream fileCopy(filePath + ".copy");
    fileCopy << fileContent;

    return fileContent;
}


/**
 * @brief Send a Json message to the AI
 * 
 * @param jsonMessageString 
 * @return string 
 */
string AiConnector::sendJsonToAi(string jsonMessageString)
{
    // Create a CURL object
    CURL *curl;
    CURLcode res;

    // Initialize the CURL object
    curl = curl_easy_init();

    // Check if the CURL object was initialized
    if (curl)
    {
        // Set the URL
        std::string urlWithApiKey = std::string(GEMINE_AI_URL) + "?key=" + std::string(GEMINE_AI_API_KEY);
        curl_easy_setopt(curl, CURLOPT_URL, urlWithApiKey.c_str());

        // Set the API key
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonMessageString.c_str());

        // Set the write callback
        string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check if the request was successful
        if (res != CURLE_OK)
        {
            cout << "Error sending Json message to the AI: " << curl_easy_strerror(res) << endl;
            return "";
        }

        // Cleanup
        curl_easy_cleanup(curl);

        return response;
    }
    else
    {
        cout << "Error initializing CURL object" << endl;
        return "";
    }
}

/**
 * @brief Extract the content from the response
 * 
 * @param response
 * @return string 
 */
string AiConnector::extractContentFromResponse(string response)
{
    // Gemini AI responds if a json that follows the patter {"candidates": [{"content": {"parts": [{"text": "json response content"}]}}]}
    // Parse the response to extract the content from text

    Document document;
    document.Parse(response.c_str());

    if (document.HasMember("candidates"))
    {
        Value &candidates = document["candidates"];
        if (candidates.IsArray())
        {
            for (SizeType i = 0; i < candidates.Size(); i++)
            {
                Value &content = candidates[i]["content"];
                if (content.HasMember("parts"))
                {
                    Value &parts = content["parts"];
                    if (parts.IsArray())
                    {
                        for (SizeType j = 0; j < parts.Size(); j++)
                        {
                            Value &part = parts[j];
                            if (part.HasMember("text"))
                            {
                                return part["text"].GetString();
                            }
                        }
                    }
                }
            }
        }
    }

    return "Error extracting content from response";

}

/**
 * @brief Write callback for web page requests
 * 
 * @return size_t The size of the response
 */ 
size_t AiConnector::WriteCallback(char* contents, size_t size, size_t nmemb, std::string* response)
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