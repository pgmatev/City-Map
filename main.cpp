#include "program.hh"

int main()
{
    std::ifstream file("test.txt");
    Program p;
    p.takeInput(file);
    p.printMap();

    std::ofstream dotty("graph.dot");
    p.toDotty(dotty);
}