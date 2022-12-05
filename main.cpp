#include "program.hh"

int main()
{
    std::ifstream file("test.txt");
    Program p;
    p.takeInput(file);
}