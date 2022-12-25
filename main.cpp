#include "program.hh"

int main()
{
    std::ifstream file("test.txt");
    Program p;
    p.takeInput(file);
    p.printMap();

    std::ofstream dotty("graph.dot");
    p.toDotty(dotty);

    std::string s1 = "5Kiosheta";
    std::string s2 = "BSFS";
    p.hasPath(s2, s1);

    p.threeShortestPaths(s1, s2);
}