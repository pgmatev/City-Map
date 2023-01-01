#ifndef PROGRAM_HH
#define PROGRAM_HH

#include "city.hh"

#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>

class Program
{
    private:
    City loaded_city;

    bool isNumber(char);
    bool isNewLine(char);

    void toDottyHelper(std::ostream&);

    public:
    void takeInput(std::ifstream&);
    void passClosedJunctions(const std::string&);
    void printMap();
    
    void toDotty(std::ostream&);

    void hasPath(const std::string&, const std::string&);
    void hasCycle(const std::string&);
    void areReachable(const std::string&);
    void deadEnds();
    void threeShortestPaths(const std::string&, const std::string&);
    void touristLap();

};

#endif