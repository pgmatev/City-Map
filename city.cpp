#include "city.hh"

City::City()
{}

void City::addKey(std::string& s)
{
    std::vector<Pair> v;
    city_map.insert({s, v});
}

void City::addRoad(std::string& key, std::string& junction, int distance)
{
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