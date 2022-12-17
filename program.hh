#ifndef PROGRAM_HH
#define PROGRAM_HH

#include "city.hh"

#include <fstream>
#include <iostream>
#include <sstream>

class Program
{
    private:
    City loaded_city;

    bool isNumber(char);
    bool isNewLine(char);

    void toDottyHelper(std::ostream&);

    public:
    void takeInput(std::ifstream&);
    void printMap();
    
    void toDotty(std::ostream&);

    bool hasPath(const std::string&, const std::string&);
    void shortestPathsFromSource(const std::string&);
};

#endif