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

    public:
    void takeInput(std::ifstream&);
    void printMap();
};

#endif