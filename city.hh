#ifndef MAP_HH
#define MAP_HH

#include <map>
#include <vector>
#include <queue>
#include <utility> //for pair
#include <algorithm> //for std::find
#include <stdexcept>


#include "road.hh"
#include "junction.hh"

typedef std::pair<std::string, int> Pair;

class City
{
    private:
     //for every junction we have a vector of pairs of another junction and distance to it
    std::map<std::string, std::vector<Pair>> city_map;
    std::vector<std::string> closed_junctions;
    public:
    City();

    std::map<std::string, std::vector<Pair>>& getMap();
    std::vector<std::string>& getClosedJunctions();

    void addKey(const std::string&);
    void addRoad(const std::string&, const std::string&, int);
    bool hasPath(const std::string&, const std::string&);
};

#endif