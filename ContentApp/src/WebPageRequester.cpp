/*
	NovaGenesis

	Name:		WebPageRequester
	Object:		WebPageRequester
	File:		WebPageRequester.cpp
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

#ifndef WEB_PAGE_REQUESTER_H
#include "WebPageRequester.h"
#endif

#ifndef BASE
#define BASE std::getenv("BASE")
#endif

#include <thread>
#include <chrono>
#include <regex>

WebPageRequester::WebPageRequester()
{
}

WebPageRequester::~WebPageRequester()
{
}

/**
 * @brief Request a web content and store it in the source folder
 * 
 * @param url 
 */
void WebPageRequester::requestWebContent(const string &url)
{
    requestContentFromUrl(url, true);
}

/**
 * @brief Request a web content and store it in the source folder
 * 
 * @param url 
 * @param isRoot
 */
/* void WebPageRequester::requestContentFromUrl(string url, bool isRoot)
{
    // Create a hash of the URL to use as the folder name
    string urlHash = getUrlAsHash(url);

    // Create the folder to store the web page in the source folder
    string folderPath = string(BASE) + "/IO/Source1/" + urlHash;

    // Check if there is a folder and a zip file with the same name
    if (std::filesystem::exists(folderPath) || std::filesystem::exists(folderPath + ".zip")) {
        std::cout << "Folder or zip file already exists for URL: " << url << std::endl;
        return;
    }

    std::filesystem::create_directories(folderPath);

    curl_global_init(CURL_GLOBAL_ALL);

    CURL* curl = curl_easy_init();

    if(!curl) {
        std::cerr << "Error: Could not initialize CURL" << std::endl;
        return;
    }

    FILE* responseFile = fopen((folderPath + "/" + urlHash + ".txt").c_str(), "wb");
    std::string headers;

    if(!responseFile) {
        std::cerr << "Error: Could not create response file" << std::endl;
        return;
    }

    // Set the URL to the desired web page
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // Set the callback function to handle the response
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);  // Enable automatic redirect following
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToFileCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, responseFile);

    // store the received headers
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HeaderCallback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &headers);

    // Perform the request
    CURLcode res = curl_easy_perform(curl);

    // Check for errors
    if (res != CURLE_OK) {
        std::cerr << "Error during CURL request: " << curl_easy_strerror(res) << std::endl;
        return;
    }

    // Get the content type from CURL
    char* contentTypeChar;
    curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &contentTypeChar);
    std::string contentType = contentTypeChar;

    // Remove the charset from the content type
    size_t charsetPos = contentType.find(";");
    if (charsetPos != std::string::npos) {
        contentType = contentType.substr(0, charsetPos);
    }

    // Determine the file extension based on the content type
    std::string fileExtension = getExtensionFromContentType(contentType);

    // Close the response file
    fclose(responseFile);

    // Wait for the response file to be closed
    while (!std::filesystem::exists(folderPath + "/" + urlHash + ".txt")) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Waiting for response file to be closed" << std::endl;
    }

    // Rename the response file to the correct extension
    try {
        std::filesystem::rename(folderPath + "/" + urlHash + ".txt", folderPath + "/" + urlHash + fileExtension);
    } catch (std::filesystem::filesystem_error& e) {
        std::cerr << "Error renaming file: " << e.what() << std::endl;
        return;
    }

    // Clean up
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    // If the content type is text/html
    if (contentType == "text/html" && isRoot) {
        std::ifstream responseFile(folderPath + "/" + urlHash + fileExtension);
        std::string responseHtml((std::istreambuf_iterator<char>(responseFile)), std::istreambuf_iterator<char>());
        responseFile.close();

        // Process all images in the response
        handleImagesFromHtml(responseHtml);

        // Process all CSS files in the response
        handleCssFromHtml(responseHtml);

        // Process all JS files in the response
        //handleJsFromHtml(responseHtml);

        responseHtml = replaceAllUrls(responseHtml);

        std::ofstream resonseHtmlFile(folderPath + "/" + urlHash + fileExtension);
        resonseHtmlFile << responseHtml;
        resonseHtmlFile.close();

        // Wait for the response file to be closed
        while (!std::filesystem::exists(folderPath + "/" + urlHash + fileExtension)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << "Waiting for response file to be closed" << std::endl;
        }
    }

    // Save the headers to a file in to the created folder
    std::ofstream headersFile(folderPath + "/headers.txt");
    headersFile << headers;
    headersFile.close();

    // Wait for the headers file to be closed
    while (!std::filesystem::exists(folderPath + "/headers.txt")) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Waiting for headers file to be closed" << std::endl;
    }

    // zip the folder contents
    zipFolderContents(folderPath);

    // Delete the folder
    try {
        std::filesystem::remove_all(folderPath);
    } catch (std::filesystem::filesystem_error& e) {
        std::cerr << "Error deleting folder: " << e.what() << std::endl;
        return;
    }
}
 */
/**
 * @brief Request a web content and store it in the source folder
 * 
 * @param url
 */
void WebPageRequester::requestContentFromUrl(string url, bool isRoot)
{
    // Create a hash of the URL to use as the folder name
    string urlHash = getUrlAsHash(url);

    // Create the folder to store the web page in the source folder
    string folderPath = string(BASE) + "/IO/Source1/" + urlHash;

    // Check if there is a folder and a zip file with the same name
    if (std::filesystem::exists(folderPath) || std::filesystem::exists(folderPath + ".zip")) {
        std::cout << "Folder or zip file already exists for URL: " << url << std::endl;
        return;
    }

    std::filesystem::create_directories(folderPath);

    // Get the page using wget with all its dependencies, not recursive
    std::string wgetCommand = "wget -p -k -E -H -nd -P " + folderPath + " " + url;
    FILE* wgetPipe = popen(wgetCommand.c_str(), "r");
    if (wgetPipe) {
        char buffer[128];
        while (!feof(wgetPipe)) {
            if (fgets(buffer, 128, wgetPipe) != nullptr)
                std::cout << buffer;
        }
        pclose(wgetPipe);
    }

    // zip the folder contents
    zipFolderContents(folderPath);

    // Wait zip file to be created
    while (!std::filesystem::exists(folderPath + ".zip")) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Waiting for zip file to be created" << std::endl;
    }

    // Delete the folder
    try {
        std::filesystem::remove_all(folderPath);
    } catch (std::filesystem::filesystem_error& e) {
        std::cerr << "Error deleting folder: " << e.what() << std::endl;
        return;
    }
}

/**
 * @brief Get the Url As Hash object
 *
 * @param url
 * @return string
 */
string WebPageRequester::getUrlAsHash(string url)
{
    std::hash<std::string> hasher;
    return std::to_string(hasher(url));
}

/**
 * @brief Execute a command and return the output
 * 
 * @param cmd
 * @return std::string
 */
std::string WebPageRequester::exec(const char *cmd)
{
    std::string result;
    FILE* pipe = popen(cmd, "r");
    if (pipe) {
        char buffer[128];
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != nullptr)
                result += buffer;
        }
        pclose(pipe);
    }
    return result;
}

/**
 * @brief Header callback for web page requests, that will be included in restcppsdk response
 * 
 * @return size_t The size of the header
 */
size_t WebPageRequester::HeaderCallback(char* contents, size_t size, size_t nmemb, std::string* response)
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
size_t WebPageRequester::WriteCallback(char* contents, size_t size, size_t nmemb, std::string* response)
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
 * @brief Write callback for web page requests, that will write the response to a file
 * 
 * @return size_t The size of the response
 */
size_t WebPageRequester::WriteToFileCallback(char* contents, size_t size, size_t nmemb, FILE* file)
{
   return fwrite(contents, size, nmemb, file);
}

/**
 * @brief Replace all URLs in the response by replacing http:// or https:// with the prefix "http://127.0.0.1:10102/"
 * 
 * @param responseHtml
 */
string WebPageRequester::replaceAllUrls(string responseHtml)
{
    std::string prefix = "http://127.0.0.1:10102/";

    std::regex httpRegex("http://");
    std::regex httpsRegex("https://");

    responseHtml = std::regex_replace(responseHtml, httpRegex, prefix);
    responseHtml = std::regex_replace(responseHtml, httpsRegex, prefix);

    return responseHtml;
}

/**
 * @brief Get the Extension From Content Type object
 * 
 * @param contentType
 * @return std::string
 */
string WebPageRequester::getExtensionFromContentType(string contentType)
{
    if (contentType == "text/html") {
        return ".html";
    } else if (contentType == "text/css") {
        return ".css";
    } else if (contentType == "text/javascript") {
        return ".js";
    } else if (contentType == "image/jpeg") {
        return ".jpg";
    } else if (contentType == "image/png") {
        return ".png";
    } else if (contentType == "image/gif") {
        return ".gif";
    } else if (contentType == "image/svg+xml") {
        return ".svg";
    } else if (contentType == "image/webp") {
        return ".webp";
    } else if (contentType == "image/bmp") {
        return ".bmp";
    } else if (contentType == "image/vnd.microsoft.icon") {
        return ".ico";
    } else if (contentType == "image/tiff") {
        return ".tiff";
    } else if (contentType == "image/x-icon") {
        return ".ico";
    } else if (contentType == "image/x-xbitmap") {
        return ".xbm";
    } else if (contentType == "image/x-xpixmap") {
        return ".xpm";
    } else if (contentType == "image/x-xwindowdump") {
        return ".xwd";
    } else if (contentType == "image/avif") {
        return ".avif";
    } else if (contentType == "image/apng") {
        return ".apng";
    } else if (contentType == "image/flif") {
        return ".flif";
    } else if (contentType == "image/jxl") {
        return ".jxl";
    } else if (contentType == "image/jpg") {
        return ".jpg";
    } else if (contentType == "image/jpm") {
        return ".jpm";
    } else if (contentType == "image/jpx") {
        return ".jpx";
    } else if (contentType == "image/xbm") {
        return ".xbm";
    } else if (contentType == "image/xpm") {
        return ".xpm";
    } else if (contentType == "image/xwd") {
        return ".xwd";
    } else if (contentType == "image/x-icon") {
        return ".ico";
    }
    
    return "";
}

/**
 * @brief Handle all images in the response
 * 
 * @param responseHtml
 */
void WebPageRequester::handleImagesFromHtml(string responseHtml)
{
    // Get all image URLs from the response
    std::regex imageRegex("<img.*?src=\"(.*?)\".*?>");
    std::smatch imageMatch;
    std::string::const_iterator searchStart(responseHtml.cbegin());

    // For each image URL perform the requestWebContent
    while (std::regex_search(searchStart, responseHtml.cend(), imageMatch, imageRegex)) {
        std::string imageUrl = imageMatch[1];
        // Request the image in a separate thread
        std::thread imageThread(&WebPageRequester::requestContentFromUrl, this, imageUrl, false);
        imageThread.detach();
        searchStart = imageMatch.suffix().first;
    }
}

/**
 * @brief Handle all CSS files in the response
 * 
 * @param responseHtml
 */
void WebPageRequester::handleCssFromHtml(string responseHtml)
{
    // Get all CSS URLs from the response
    std::regex cssRegex("<link.*?href=\"(.*?)\".*?>");
    std::smatch cssMatch;
    std::string::const_iterator searchStart(responseHtml.cbegin());

    // For each CSS URL perform the requestWebContent
    while (std::regex_search(searchStart, responseHtml.cend(), cssMatch, cssRegex)) {
        std::string cssUrl = cssMatch[1];
        // Request the CSS in a separate thread
        std::thread cssThread(&WebPageRequester::requestContentFromUrl, this, cssUrl, false);
        cssThread.detach();
        searchStart = cssMatch.suffix().first;
    }
}

/**
 * @brief Handle all JS files in the response
 * 
 * @param responseHtml
 */
void WebPageRequester::handleJsFromHtml(string responseHtml)
{
    // Get all JS URLs from the response
    std::regex jsRegex("<script.*?src=\"(.*?)\".*?>");
    std::smatch jsMatch;
    std::string::const_iterator searchStart(responseHtml.cbegin());

    // For each JS URL perform the requestWebContent
    while (std::regex_search(searchStart, responseHtml.cend(), jsMatch, jsRegex)) {
        std::string jsUrl = jsMatch[1];
        // Request the JS in a separate thread
        std::thread jsThread(&WebPageRequester::requestContentFromUrl, this, jsUrl, false);
        jsThread.detach();
        searchStart = jsMatch.suffix().first;
    }
}

/**
 * @brief Zip the web content
 * 
 * @param path
 */
void WebPageRequester::zipFolderContents(string path)
{
    // zip the folder contents
    std::filesystem::path folder(path);
    std::filesystem::path zipFile = folder.parent_path() / (folder.stem().string() + ".zip");
    std::string zipCommand = "zip -r " + zipFile.string() + " -j " + folder.string();

    int zipResult = std::system(zipCommand.c_str());

    //Wait till the zip file is created
    while (!std::filesystem::exists(zipFile)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Waiting for zip file to be created" << std::endl;
    }
    
    // check if the zip command was successful
    if (WEXITSTATUS(zipResult) != 0) {
        std::cerr << "Error: zip command failed with exit code " << WEXITSTATUS(zipResult) << std::endl;
        return;
    }
}