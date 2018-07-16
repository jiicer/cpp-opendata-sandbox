#include <iostream>
#include <string>
#include <cmath>
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

json request(const std::string &url)
{
    std::stringstream s;
    s << curlpp::options::Url(url);
    return json::parse(s.str());
}

static const double pi = std::acos(-1);

double deg_to_rad(const double deg)
{
    return deg * pi / 180;
}

double distanceInKmBetweenEarthCoordinates(double lat1, double lon1, double lat2, double lon2)
{
    const double earthRadiusKm = 6371;

    double dLat = deg_to_rad(lat2 - lat1);
    double dLon = deg_to_rad(lon2 - lon1);

    lat1 = deg_to_rad(lat1);
    lat2 = deg_to_rad(lat2);

    double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
               std::sin(dLon / 2) * std::sin(dLon / 2) * std::cos(lat1) * std::cos(lat2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return earthRadiusKm * c;
}

int main(int argc, char *argv[])
{
    std::cout << std::setw(4) << journeys_by_line(request("http://data.itsfactory.fi/journeys/api/1/vehicle-activity"), 3);
    std::cout << std::setw(4) << request("https://api.digitransit.fi/geocoding/v1/search?text=" + std::string(argv[1]) + "+" + std::string(argv[2]) + "&size=1");
    return 1;
}
