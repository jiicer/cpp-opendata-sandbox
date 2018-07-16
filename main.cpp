#include <iostream>
#include <string>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

json journeys_by_line(const json &j, int line)
{
    json rv;
    for (const auto journey : j["body"])
    {
        if (std::stoi(journey["monitoredVehicleJourney"]["lineRef"].get<std::string>()) == line)
            rv.push_back(journey);
    }
    return rv;
}

json request(const std::string& url)
{
    std::stringstream s;
    s << curlpp::options::Url(url);
    return json::parse(s.str());
}

int main()
{
    std::cout << std::setw(4) << journeys_by_line(request("http://data.itsfactory.fi/journeys/api/1/vehicle-activity"), 3);
    return 1;
}
