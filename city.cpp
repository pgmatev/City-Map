#include "city.hh"

City::City()
{}

std::map<Junction, std::vector<Pair>>& City::getMap()
{
    return this->city_map;
}

void City::addKey(std::string& s)
{
    Junction j(s);
    if (city_map.find(j) == city_map.end())
    {
        std::vector<Pair> v;
        city_map.insert({j, v});
    }

}

void City::addRoad(std::string& key, std::string& junction, int distance)
{
    Junction key_j(key);
    Junction road_j(junction);

    for (Pair p : city_map.find(key_j)->second)
    {
        if (p.first == road_j)
        {
            throw std::invalid_argument("Cannot have 2 roads with the same source and destination!");
        }
    }
    if (city_map.find(road_j) != city_map.end())
    {
        std::pair new_p = {road_j, distance};
        city_map.find(key_j)->second.push_back(new_p);
    }
    else
    {
        addKey(junction);
        addRoad(key, junction, distance);
    }

}