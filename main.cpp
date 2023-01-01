#include "program.hh"

int main()
{
    std::ifstream file("test.txt");
    Program p;
    p.takeInput(file);
    std::string cj = "";
    p.passClosedJunctions(cj);
    std::ofstream dotty("graph.dot");
    p.toDotty(dotty);

    std::string s1 = "A";
    std::string s2 = "B";

    p.hasPath(s2, s1);
    p.hasCycle(s1);
    p.threeShortestPaths(s1, s2);
    p.deadEnds();
    p.touristLap();
}