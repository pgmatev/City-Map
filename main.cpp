#include "program.hh"

int main(int argc, char **argv)
{
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