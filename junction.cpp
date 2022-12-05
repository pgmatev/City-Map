#include "junction.hh"

Junction::Junction(const std::string& name) : name(name), isOpen(true)
{}

std::string Junction::getName() const
{
    return name;
}