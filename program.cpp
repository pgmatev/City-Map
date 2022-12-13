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
                if (loaded_city.getMap().find(s) != loaded_city.getMap().end()) // key is found
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
    for (auto it : loaded_city.getMap())
    {
        std::cout << "Key: " << it.first.getName() << " | ";
        for (auto v : it.second)
        {
            std::cout << v.first.getName() << " " << v.second << ", ";
        }
        std::cout << std::endl;
    }
}

void Program::toDotty(std::ostream& out)
{
    out << "Digraph G {\n";
    toDottyHelper(out);
    out << "}";
}

void Program::toDottyHelper(std::ostream& out)
{
    for (auto it : loaded_city.getMap())
    {
        auto j1 = loaded_city.getMap().find(it.first);
        out << (long)&j1->first << "[label=\"" << j1->first.getName() << "\"];\n";
        for (auto v : j1->second)
        {
            auto j2 = loaded_city.getMap().find(v.first);
            out << (long)&j1->first << "->" << (long)&j2->first << "[label = \""<< v.second << "\" color = \"blue\"]\n";
        }
    }
}