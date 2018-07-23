#include <iostream>
#include <string>
#include <cmath>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct coordinate
{
    double longitude;
    double latitude;
};

std::ostream &operator<<(std::ostream &os, const coordinate c)
{
    os << "{ long: " << c.longitude << ", lat:" << c.latitude << "}" << '\n';
    return os;
}

coordinate decode_coord_from_journey(const json journey)
{
    return {std::stod(journey["monitoredVehicleJourney"]["vehicleLocation"]["longitude"].get<std::string>()),
            std::stod(journey["monitoredVehicleJourney"]["vehicleLocation"]["latitude"].get<std::string>())};
}

coordinate decode_coord_from_digitransit_geoc(const json geoc)
{
    coordinate rv;
    std::tie(rv.longitude, rv.latitude) = geoc.at("features").at(0).at("geometry").at("coordinates").get<std::tuple<double, double>>();
    return rv;
}

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

// https://stackoverflow.com/questions/365826/calculate-distance-between-2-gps-coordinates
double distance_in_km(const coordinate ca, const coordinate cb)
{
    const double earth_radius_km = 6371;

    double dLat = deg_to_rad(cb.latitude - ca.latitude);
    double dLon = deg_to_rad(cb.longitude - ca.longitude);

    double lat_a = deg_to_rad(ca.latitude);
    double lat_b = deg_to_rad(cb.latitude);

    double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
               std::sin(dLon / 2) * std::sin(dLon / 2) * std::cos(lat_a) * std::cos(lat_b);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return earth_radius_km * c;
}

int main(int argc, char *argv[])
{
    coordinate ref = decode_coord_from_digitransit_geoc(request("https://api.digitransit.fi/geocoding/v1/search?text=" + std::string(argv[1]) + "+" + std::string(argv[2]) + "&size=1"));
   
    for (const auto journey : journeys_by_line(request("http://data.itsfactory.fi/journeys/api/1/vehicle-activity"), 3))
    {
        std::cout << distance_in_km(ref, decode_coord_from_journey(journey)) << std::endl;
    }
    return 1;
}
