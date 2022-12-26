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

void City::setClosedJunctions(const std::vector<std::string>& v)
{
    for (auto i : v)
    {
        if (city_map.find(i) == city_map.end())
        {
            throw std::invalid_argument("Junction does not exist!");
        }
    }
    this->closed_junctions = v;
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

            if(std::find(visited.begin(), visited.end(), i.first) == visited.end()) // if we haven't visited this junction yet
            {
                visited.push_back(i.first);
                queue.push(i.first);
            }
        }
    }

    return false;
}

bool City::hasCycle(const std::string& source)
{
    if (city_map.find(source) == city_map.end())
    {
        throw std::invalid_argument("Source does not exist!");
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
            if (i.first == source)
            {
                return true;
            }

            if(std::find(visited.begin(), visited.end(), i.first) == visited.end()) // if we haven't visited this junction yet
            {
                visited.push_back(i.first);
                queue.push(i.first);
            }
        }
    }

    return false;
}

std::vector<std::string> City::shortestPathsFromSource(const std::string& source, const std::string& destination) //Dijkstra's algorithm
{
    if (city_map.find(source) == city_map.end())
    {
        throw std::invalid_argument("Source does not exist!");
    }

    if (city_map.find(destination) == city_map.end())
    {
        throw std::invalid_argument("Destination does not exist!");
    }
    std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, std::greater<std::pair<int, std::string>>> pq;
    //priority queue can be used to implement heap, in this case min heap because of the functor argument greater
    std::map<std::string, int> dist; //distances to all other junctions from the source
    std::map<std::string, std::vector<std::string>> paths; //paths to all other junctions from the source

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
        paths.insert({it.first, std::vector<std::string>()});
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
            if (std::find(closed_junctions.begin(), closed_junctions.end(), j) == closed_junctions.end()) //if junction is not closed
            {
                if (dist.find(j)->second > dist.find(curr)->second + d)
                //if the saved distance to j is bigger than the newly found path to it through curr
                {
                    dist.find(j)->second = dist.find(curr)->second + d;
                    paths.find(j)->second = paths.find(curr)->second;
                    paths.find(j)->second.push_back(curr);
                    pq.push({dist.find(j)->second, j});
                }
            }
        }
    }

    std::vector<std::string> result = paths.find(destination)->second;
    result.push_back(destination);
    return result;
}

//FUTURE TO DO: Make loopless (Yen's algorithm), Optimise by removing junctions with no path to destination
std::map<int, std::vector<std::string>> City::kShortestPaths(const std::string& source, const std::string& destination, int k)
//Eppstein's algorithm, based on Dijkstra's algorithm
{
    if (city_map.find(source) == city_map.end())
    {
        throw std::invalid_argument("Source does not exist!");
    }

    if (city_map.find(destination) == city_map.end())
    {
        throw std::invalid_argument("Destination does not exist!");
    }
    std::priority_queue<std::tuple<int, std::string, std::vector<std::string>>,
                        std::vector<std::tuple<int, std::string, std::vector<std::string>>>,
                        std::greater<std::tuple<int, std::string, std::vector<std::string>>>> pq;   
    //implementation of min heap, sorting the paths helps us to pop the shortest current path first, 
    //therefore the first 3 pops will be the 3 shortests paths
    std::map<std::string, int> visited_count; //count the amount of times we visited a junction
    std::map<int, std::vector<std::string>> results;

    for (auto it : city_map)
    {
        visited_count.insert({it.first, 0}); //filling the visited map
    }

    pq.push({0, source, std::vector<std::string>{source}});
    //starting point, distance to it is 0 and the path to it is itself

    while (!pq.empty())
    {
        std::tuple<int, std::string, std::vector<std::string>> tp = pq.top();
        int distance = std::get<0>(tp);
        std::string junction = std::get<1>(tp);
        std::vector<std::string> path = std::get<2>(tp);
        pq.pop();

        if(visited_count.find(junction)->second == k) //only count the first k visits, in order to get the k shortest paths
        {
            continue;
        }

        visited_count.find(junction)->second += 1;

        if (junction == destination) //reached destination, insert accumulated path
        {
            results.insert({distance, path});
        }

        for (auto adjacent : city_map.find(junction)->second) //queue all adjacent junctions to current be traversed
        {
            if (std::find(closed_junctions.begin(), closed_junctions.end(), adjacent.first) == closed_junctions.end()) //if junction is not closed
            {
                std::vector<std::string> new_path = path;
                new_path.push_back(adjacent.first);
                pq.push({distance + adjacent.second, adjacent.first, new_path});
            }

        }
    }

    return results;
}