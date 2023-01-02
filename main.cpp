#include "program.hh"

int main(int argc, char **argv)
{
    // std::ifstream file("test.txt");
    // Program p;
    // p.takeInput(file);
    // std::string cj = "A B C";
    // p.passClosedJunctions(cj);
    // std::ofstream dotty("graph.dot");
    // p.toDotty(dotty);

    // p.closeJunction("A");
    // p.openJunction("A");
    // p.openJunction("G");
    // p.closeJunction("A");
    try
    {
        Program p;
        p.run(argc, argv);
    }
    catch(std::invalid_argument& e)
    {
        std::cout << e.what() << std::endl;
    }

}