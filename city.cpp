#include "city.hh"

City::City()
{}

std::map<std::string, std::vector<Pair>>& City::getMap()
{
    return this->city_map;
}

std::vector<std::string>>& City::getClosedJunctions()
{
    return this->closed_junctions;
}

void City::addKey(const std::string& s)
{
    if (city_map.find(s) == city_map.end()) //insert if this junction doesn't exist in the map
    {
        std::vector<Pair> v;
        city_map.insert({s, v});
    }

}

void City::addRoad(const std::string& key, const std::string& junction, int distance)
{
    if (key == junction)
    {
        throw std::invalid_argument("Cannot have a road leading to the same junction!");
    }
    
    if (city_map.find(junction) == city_map.end()) //even if the second junction doesn't have roads exiting from it
    {                                           //make it a key
        addKey(junction);
    }

    for (Pair p : city_map.find(key)->second)
    {
        if (p.first == junction)
        {
            throw std::invalid_argument("Cannot have 2 roads with the same source and destination!");
        }
    }

    std::pair new_p = {junction, distance};
    city_map.find(key)->second.push_back(new_p);
}