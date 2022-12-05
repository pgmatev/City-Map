#ifndef JUNCTION_HH
#define JUNCTION_HH

#include <string>

class Junction
{
    private:
    std::string name;

    public:
    Junction(const std::string&);
    std::string getName() const;
    bool isOpen;
};

#endif