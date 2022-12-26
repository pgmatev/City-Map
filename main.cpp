#include "program.hh"

int main()
{
    std::ifstream file("test.txt");
    Program p;
    p.takeInput(file);

    std::ofstream dotty("graph.dot");
    p.toDotty(dotty);

    std::string s1 = "SU";
    std::string s2 = "BSFS";
    std::string cj = "";
    p.passClosedJunctions(cj);
    p.hasPath(s2, s1);
    p.hasCycle(s1);
    p.threeShortestPaths(s2, s1);
}