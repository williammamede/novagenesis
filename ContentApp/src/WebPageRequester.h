/*
    NovaGenesis

    Name:       WebPageRequester
    Object:     WebPageRequester
    File:       WebPageRequester.h
    Author:     William Mamede
    Date:       05/2022
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

#ifndef WEB_PAGE_REQUESTER_H
#define WEB_PAGE_REQUESTER_H

#ifndef _IOSTREAM_H
#include <iostream>
#endif

#include <string>
#include <curl/curl.h>

#include <fstream>
#include <filesystem>

using namespace std;

class WebPageRequester
{
public:
    WebPageRequester();
    ~WebPageRequester();
    void requestWebContent(const string &url);
private:
    string getUrlAsHash(string url);
    std::string exec(const char *cmd);
    string replaceAllUrls(string responseHtml);
    string getExtensionFromContentType(string contentType);
    void requestContentFromUrl(string url, bool isRootUrl);
    void handleImagesFromHtml(string responseHtml);
    void handleCssFromHtml(string responseHtml);
    void handleJsFromHtml(string responseHtml);
    void zipFolderContents(string path);
    // static write and header callbacks
    static size_t WriteCallback(char *contents, size_t size, size_t nmemb, std::string *response);
    static size_t HeaderCallback(char *contents, size_t size, size_t nmemb, std::string *response);
    static size_t WriteToFileCallback(char *contents, size_t size, size_t nmemb, FILE *file);
};
#endif