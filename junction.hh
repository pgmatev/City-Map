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

    bool operator<(const Junction&) const;
    bool operator==(const Junction&) const;
};

#endif