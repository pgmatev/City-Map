#include "program.hh"


bool Program::isNewLine(char c)
{
    return c == '\r' || // also a new line symbol
           c == '\n';
}

void Program::takeInput(std::ifstream& in)
{
    bool isKey = true;
    std::string loaded_key;
    std::string s;

    while(getline(in, s)) // get every row of file
    {
        bool isKey = true;
        std::stringstream ss(s); //convert to stringstream so getline is usable
        while (getline(ss, s, ' '))
        {
            if (isKey) //key value for the map
            {
                if (loaded_city.city_map.find(s) != loaded_city.city_map.end()) // key is found
                {
                    loaded_key = s;
                }
                else //key not found
                {
                    loaded_city.addKey(s);
                    loaded_key = s;
                }
                isKey = false;
            }
            else
            {
                if(!isNewLine(ss.peek()))
                {
                    std::string s1;
                    getline(ss, s1, ' ');
                    if (stoi(s1)) {
                        int distance = stoi(s1);
                        loaded_city.addRoad(loaded_key, s, distance);
                    }
                }
            }
        }
    }
}

void Program::printMap()
{
    for (auto it : loaded_city.city_map)
    {
        std::cout << "Key: " << it.first << " | ";
        for (auto v : it.second)
        {
            std::cout << v.first << " " << v.second << ", ";
        }
        std::cout << std::endl;
    }
}
