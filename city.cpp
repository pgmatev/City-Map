#include "city.hh"

City::City()
{}

std::map<std::string, std::vector<Pair>>& City::getMap()
{
    return this->city_map;
}

std::vector<std::string>& City::getClosedJunctions()
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

bool City::hasPath(const std::string& source, const std::string& destination) 
{
    if (city_map.find(source) == city_map.end())
    {
        throw std::invalid_argument("Source does not exist!");
    }

    if (city_map.find(destination) == city_map.end())
    {
        throw std::invalid_argument("Destination does not exist!");
    }

    if (source == destination)
    {
        return true;
    }
    //using BFS method
    std::queue<std::string> queue;
    std::vector<std::string> visited;

    queue.push(source);
    visited.push_back(source);

    while (!queue.empty())
    {
        std::string s = queue.front();
        queue.pop();
        for (auto i : city_map.find(s)->second)
        {
            if (i.first == destination)
            {
                return true;
            }

            if(std::find(visited.begin(), visited.end(), i.first) == visited.end())
            {
                visited.push_back(i.first);
                queue.push(i.first);
            }
        }
    }

    return false;
}