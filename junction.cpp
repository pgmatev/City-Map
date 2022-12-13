#include "junction.hh"

Junction::Junction(const std::string& name) : name(name), isOpen(true)
{}

std::string Junction::getName() const
{
    return name;
}

bool Junction::operator<(const Junction& other) const
{
    return this->name < other.name;
}

bool Junction::operator==(const Junction& other) const
{
    return this->name == other.name;
}