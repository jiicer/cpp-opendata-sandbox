#include <iostream>
#include <sstream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <nlohmann/json.hpp>

int main() {
    std::stringstream s;
    s << curlpp::options::Url(std::string("http://data.itsfactory.fi/journeys/api/1/vehicle-activity"));
    nlohmann::json json = nlohmann::json::parse(s.str());
    std::cout << std::setw(4) << json;
    return 1;
}

