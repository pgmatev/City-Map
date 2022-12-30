#include "city.hh"

// ==========/ Private Helper Methods /==========

void City::dfs(const std::string& source, std::vector<std::string>& visited)
{
    visited.push_back(source);

    for(auto it : city_map.find(source)->second)
    {
        if(std::find(visited.begin(), visited.end(), it.first) == visited.end()) // if we haven't visited this junction yet
        {
            dfs(it.first, visited);
        }
    }
}

//traversing the roads instead of the junctions (pair of junctions)
void City::dfsRoads(const std::string& source, std::vector<std::pair<std::string, std::string>>& visited)
{
    for(auto it : city_map.find(source)->second)
    {
        std::pair<std::string, std::string> road = {source, it.first};
        if(std::find(visited.begin(), visited.end(), road) == visited.end()) // if we haven't visited this junction yet
        {
            visited.push_back(road);
            dfsRoads(it.first, visited);
        }
    }
}

int City::countInDegree(const std::string& source)
{
    int count = 0;
    for (auto it : city_map) //iterate all keys
    {
        for (auto v : it.second) //iterate adj vectors
        {
            if (v.first == source) //if there is a street to the chosen junction, count it 
            {
                count++;
            }
        }
    }
    return count;
}

City City::getTransponse()
{
    City trans_c;
    for (auto it : city_map)
    {
        trans_c.getMap().insert({it.first, std::vector<Pair>()}); //works because insert does not override
        for (auto v : it.second) //takes all junctions from adj vector and creates a reversed road
        {
            auto key = trans_c.getMap().find(v.first);
            if (key != trans_c.getMap().end())
            {
                key->second.push_back({it.first, v.second});
            }
            else
            {
                std::vector<Pair> new_v = {{it.first, v.second}};
                trans_c.getMap().insert({v.first, new_v});
            }
        }
    }
    return trans_c;
}

bool City::isStronglyConnected() // Kosaraju's DFS based simple algorithm
{
    std::vector<std::string> visited;
    std::string not_dead_end;
    for (auto it : city_map) 
    {
        if (it.second.size() > 0)
        {
            not_dead_end = it.first;
            break; //get a junction that has exiting streets
        }
    }

    dfs(not_dead_end, visited);
    if (visited.size() != city_map.size())
    {
        return false;
    }

    City transposed_city = getTransponse();

    visited.clear();
    transposed_city.dfs(not_dead_end, visited);

    if (visited.size() != transposed_city.getMap().size())
    {
        return false;
    }

    return true;
}

bool City::hasEurelianPath()
{
    if (!isStronglyConnected()) //1. the graph should be strongly connected
    {
        return false;
    }
    for (auto it : city_map) //2. the In-degree and Out-degree should be equal 
                            //(every junction has to have the same amount of streets entering and exiting it)
    {
        if (it.second.size() != countInDegree(it.first))
        {
            return false;
        }
    }
    return true;
}

bool City::isBridge(const std::string& u, const std::string& v)
{
    std::vector<std::pair<std::string, std::string>> visited1;
    dfsRoads(u, visited1); //firstly check all reachable junctions from u
    std::vector<std::pair<std::string, std::string>> visited2;
    visited2.push_back({u, v});
    dfsRoads(u, visited2); //then check all reachable junctions from u without the u-v road
    return visited1.size() > visited2.size(); //if with u-v we can reach more junctions it means its a bridge
}

// ==========/ Initialisation Methods /==========
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
    
    if (city_map.find(key) == city_map.end())
    {
        throw std::invalid_argument("Starting junction does not exist!");
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

// ==========/ Functionality /==========

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

bool City::areReachable(const std::string& source)
{
    if (city_map.find(source) == city_map.end())
    {
        throw std::invalid_argument("Source does not exist!");
    }
    //Another bfs
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
            if(std::find(visited.begin(), visited.end(), i.first) == visited.end()) // if we haven't visited this junction yet
            {
                visited.push_back(i.first);
                queue.push(i.first);
            }
        }
    }
    return visited.size() == city_map.size(); //if the number of visited junctions is equal to the total it means we can visit all of them
}

std::vector<std::pair<std::string, std::string>> City::deadEnds()
{
    std::string source;
    std::queue<std::pair<std::string, std::string>> queue; // the queue keeps pairs of junctions that form a street
    std::vector<std::pair<std::string, std::string>> visited; // also keeps pairs of junctions (aka streets)
    std::vector<std::pair<std::string, std::string>> results;

    for (auto it : city_map)
    {
        if (areReachable(it.first)) // guarantees full street coverage
        {
            source = it.first;
            break;
        }
    }

    for (auto it : city_map.find(source)->second) //preload the queue
    {
        queue.push({source, it.first});
        visited.push_back({source, it.first});
    }

    while (!queue.empty())
    {
        std::pair<std::string, std::string> road = queue.front();
        queue.pop();

        if (city_map.find(road.second)->second.empty()) // if there are no streets exiting the junction it means it is a dead-end
        {
            results.push_back(road);
            continue;
        }

        for (auto i : city_map.find(road.second)->second)
        {
            std::pair<std::string, std::string> next_road{road.second, i.first};
            if(std::find(visited.begin(), visited.end(), next_road) == visited.end()) // if we haven't visited this road yet
            {
                visited.push_back(next_road);
                queue.push(next_road);
            }
        }
    }
    return results;
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

std::vector<std::string> City::generateEurelianPath()
{
    std::vector<std::string> path;
    if(!hasEurelianPath())
    {
        return path;
    }
    std::string source;

    for (auto it : city_map)
    {
        if (it.second.size() % 2 != 0)
        {
            source = it.first; //start from a junction that has an odd degree(has odd number of streets exiting it)
            break;
        }
    }
    if (source.empty())
    {
        source = city_map.begin()->first; //if all junctions have an even degree, start from the first
    }
    path.push_back(source);
    std::vector<std::pair<std::string, std::string>> visited;
    generateEurelianPathHelper(source, path, visited);
    return path;


}

//TO DO: Hierholzer's Algorithm
void City::generateEurelianPathHelper(const std::string& source, std::vector<std::string>& path, std::vector<std::pair<std::string, std::string>>& visited)
{
    auto iterator = city_map.find(source);
    // if (iterator->second.size() == 1)
    // {
    //     path.push_back(iterator->second[0].first);
    //     visited.push_back({source, iterator->second[0].first});
    //     generateEurelianPathHelper(iterator->second[0].first, path, visited);
    // }
    // else
    // {
        for (auto it : iterator->second)
        {
            // if (!isBridge(source, it.first))
            // {
                std::pair<std::string, std::string> road = {source, it.first};
                if (std::find(visited.begin(), visited.end(), road) == visited.end())
                {
                    path.push_back(it.first);
                    visited.push_back(road);
                    generateEurelianPathHelper(it.first, path, visited);
                }
            // }
        }
    // }

}