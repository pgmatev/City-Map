#ifndef MAP_HH
#define MAP_HH

#include <map>
#include <vector>
#include <utility> //for pair
#include <stdexcept>


#include "road.hh"
#include "junction.hh"

typedef std::pair<std::string, int> Pair;

class City
{
    private:
     //for every junction we have a vector of pairs of another junction and distance to it

    public:
    City();
    std::map<std::string, std::vector<Pair>> city_map;
    void addToMap(Road&);
    void addKey(std::string&);
    void addRoad(std::string&, std::string&, int);
};

#endif