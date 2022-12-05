#ifndef ROAD_HH
#define ROAD_HH

#include "junction.hh"

class Road
{
    private:
    Junction* source;
    Junction* destination;
    int distance;

    public:
    Road(Junction*, Junction*, int);
};

#endif