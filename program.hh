#ifndef PROGRAM_HH
#define PROGRAM_HH

#include "city.hh"

#include <fstream>
#include <iostream>

class Program
{
    private:
    City loaded_city;

    public:
    void takeInput(std::ifstream&);
};

#endif