#ifndef MAP_HH
#define MAP_HH

#include <map>
#include <vector>
#include <utility> //for pair

#include "road.hh"
#include "junction.hh"

typedef std::pair<Junction*, int> Pair;

class City
{
    private:
    std::map<Junction*, std::vector<Pair>> city_map; //for every junction we have a vector of pairs of another junction and distance to it

    public:
    City();
    void addToMap(Road&);
    // void addJunctions()
};

#endif