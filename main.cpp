#include "program.hh"

int main()
{
    std::ifstream file("test.txt");
    Program p;
    p.takeInput(file);
    p.printMap();

    std::ofstream dotty("graph.dot");
    p.toDotty(dotty);

    std::string s1 = "A";
    std::string s2 = "B";
    if(p.hasPath(s2, s1))
    {
        std::cout << "Path exists" << std::endl;
    }

    p.shortestPathsFromSource(s1);
}