#include "program.hh"

int main()
{
    std::ifstream file("test.txt");
    Program p;
    p.takeInput(file);
    p.printMap();
    // std::map<std::string, std::vector<Pair>> city_map = {{"Popa", {{"BSFS", 150}, {"5Kiosheta", 1000}}}, 
    //                                                      {"BSFS", {{"Popa", 150}}},
    //                                                      {"5Kiosheta", {{"NDK", 700}}},
    //                                                      {"NDK", {{"Popa", 400}}}};
    // for (Pair p: city_map.find("Popa")->second)
    // {
    //         std::cout << p.first << std::endl;
    // }

}