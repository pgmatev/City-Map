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
        bool isKey = true; // first value of a row is a key junction
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
            else //not a key, not first value
            {
                if(!isNewLine(ss.peek())) //if the symbol after the value is a new line                           
                {                        //there is no next value
                    std::string s1;     //pairing the values so to get the junction and its distance
                    getline(ss, s1, ' ');
                    try
                    {
                        int distance = stoi(s1); //distance should be a number
                        loaded_city.addRoad(loaded_key, s, distance);
                    }catch(std::invalid_argument& e)
                    {
                        std::cout << "Error: " << e.what() << std::endl;
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
        std::cout << "Key: " << it.first << " | ";
        for (auto v : it.second)
        {
            std::cout << v.first << " " << v.second << ", ";
        }
        std::cout << std::endl;
    }
}

void Program::toDotty(std::ostream& out) // writes to a file in the .dot language syntax
{                                       // visualisation of the graph using graphviz
    out << "Digraph G {\n";
    toDottyHelper(out);
    out << "}";
}

void Program::toDottyHelper(std::ostream& out)
{
    for (auto it : loaded_city.getMap())
    {
        auto j1 = loaded_city.getMap().find(it.first);
        out << (long)&j1->first << "[label=\"" << j1->first << "\"];\n";
        for (auto v : j1->second)
        {
            auto j2 = loaded_city.getMap().find(v.first);
            out << (long)&j1->first << "->" << (long)&j2->first << "[label = \""<< v.second << "\" color = \"blue\"]\n";
        }
    }
}