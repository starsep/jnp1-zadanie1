#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <version.hpp>
#include "http_client.hpp"

std::string getXml() {
    HttpClient *client;
    try {
        boost::asio::io_service io_service;
        client = new HttpClient(io_service, "www.currency-iso.org", "/dam/downloads/lists/list_one.xml");
        io_service.run();
    }
    catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << "\n";
    }
    return client->getResponse();
}

int main() {
    std::cout << getXml();
}
