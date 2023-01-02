#ifndef PROGRAM_HH
#define PROGRAM_HH

#include "city.hh"

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>

class Program
{
    private:
    City loaded_city;
    std::string location;

    bool isNumber(char);
    bool isNewLine(char);

    void toDottyHelper(std::ostream&);

    public:
    void takeInput(const std::string& filename);
    void passClosedJunctions(const std::string&);
    void printMap();
    void printLocation();
    void printClosedJunctions();
    void toDotty(const std::string&);

    void changeLocation(const std::string&);
    void printNeighbours();
    void move(const std::string&);
    void closeJunction(const std::string&);
    void openJunction(const std::string&);

    void hasPath(const std::string&, const std::string&);
    void hasCycle(const std::string&);
    void areReachable(const std::string&);
    void deadEnds();
    void threeShortestPaths(const std::string&, const std::string&);
    void touristLap();

    void run(int, char**);

};

#endif