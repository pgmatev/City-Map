#ifndef MAP_HH
#define MAP_HH

#include <map>
#include <vector>
#include <queue>
#include <stack>
#include <tuple>
#include <utility> //for pair
#include <algorithm> //for std::find
#include <stdexcept>
#include <limits> //limits so to define infinity


#define INF std::numeric_limits<int>::max() //technical upper boundary for integers, can be treated as infinity 

typedef std::pair<std::string, int> Pair;

class City
{
private:
     //for every junction we have a vector of pairs of another junction and distance to it
    std::map<std::string, std::vector<Pair>> city_map;
    std::vector<std::string> closed_junctions;
    void dfs(const std::string&, std::vector<std::string>&); //Depth-first search function
    void dfsRoads(const std::string&, std::vector<std::pair<std::string, std::string>>&);
    int countInDegree(const std::string&); //counts how many streets enter a junction
    City getTransponse(); //reverses the streets 
    bool isStronglyConnected(); //used in finding eurelian cyrcle
    bool hasEurelianCycle();
    bool isBridge(const std::string&, const std::string&);

public:
    City();
    std::map<std::string, std::vector<Pair>>& getMap();
    std::vector<std::string>& getClosedJunctions();
    void setClosedJunctions(const std::vector<std::string>&);

    void addKey(const std::string&);
    void addRoad(const std::string&, const std::string&, int);
    bool hasPath(const std::string&, const std::string&);
    bool hasCycle(const std::string&);
    bool areReachable(const std::string&);
    std::vector<std::pair<std::string,std::string>> deadEnds();
    std::vector<std::string> shortestPathsFromSource(const std::string&, const std::string&);
    std::map<int, std::vector<std::string>>  kShortestPaths(const std::string&, const std::string&, int k);
    std::vector<std::string> generateEurelianCircuit();
};

#endif