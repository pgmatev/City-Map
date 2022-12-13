#ifndef MAP_HH
#define MAP_HH

#include <map>
#include <vector>
#include <utility> //for pair
#include <stdexcept>


#include "road.hh"
#include "junction.hh"

typedef std::pair<Junction, int> Pair;

class City
{
    private:
     //for every junction we have a vector of pairs of another junction and distance to it
    std::map<Junction, std::vector<Pair>> city_map;

    public:
    City();

    std::map<Junction, std::vector<Pair>>& getMap();

    void addKey(std::string&);
    void addRoad(std::string&, std::string&, int);
};

#endif