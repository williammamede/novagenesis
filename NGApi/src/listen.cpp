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

string getBindingsReport(string requestedBindings)
{
    // TODO: improve the way path is handled
    string path = "/mnt/c/Users/williamsm/Documents/personal_workspace/novagenesis/IO/NRNCS/" + requestedBindings + "bindings.json";
    stringstream ss;
    uclog << "Reading bindings from " << requestedBindings << endl;
    // TODO: create a way to validte the bindings
    std::ifstream file(path);
    ss << file.rdbuf();
    return ss.str();
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
		respond(req, status_codes::OK, json::value::string(getBindingsReport(bindingName)));
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