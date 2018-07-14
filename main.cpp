#include <iostream>
#include <sstream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>

int main() {
    std::cout << curlpp::options::Url(std::string("http://data.itsfactory.fi/journeys/api/1/vehicle-activity"));
    return 1;
}

