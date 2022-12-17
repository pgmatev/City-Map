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

void City::shortestPathsFromSource(const std::string& source) //Dijkstra's algorithm
{
    std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, std::greater<std::pair<int, std::string>>> pq;
    //priority queue can be used to implement heap, in this case min heap because of the functor argument greater
    std::map<std::string, int> dist; //distances to all other junctions from the source

    for (auto it : city_map)
    {
        if (it.first == source)
        {
            dist.insert({source, 0}); //distance from A to A is 0
        }
        else
        {
            dist.insert({it.first, INF}); //at the beginning every distance is INF 
        }
    }
    std::string s = source;
    pq.push({0, s}); //pq is sorted by the first element of the pair, so it should be the distance
                         // if the name is first we will be sorting it by name which is wrong

    while(!pq.empty())//BFS logic
    {
        std::string curr = pq.top().second;
        pq.pop();

        for (auto i : city_map.find(curr)->second) //get the adjacent junctions of the current junction
        {
            std::string j = i.first; //get the name of the adj junction
            int d = i.second; // get the distance to the adj junction

            if (dist.find(j)->second > dist.find(curr)->second + d)
            //if the saved distance to j is bigger than the newly found path to it through curr
            {
                dist.find(j)->second = dist.find(curr)->second + d;
                pq.push({dist.find(j)->second, j});
            }
        }
    }

    for (auto i : dist)
    {
        std::cout << "Distance to: " << i.first << " = " << i.second << std::endl;
    }
}