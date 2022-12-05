#include "program.hh"


bool isNewLine(char c)
{
    return c == '\r' || // also a new line symbol
           c == '\n';
}

void Program::takeInput(std::ifstream& in)
{
    bool isKey = true;
    bool isDistance = false;
    while (in && !in.eof())
    {
        std::string s;
        in >> s;
        if (isKey) //key value for the map
        {
            Junction j(s);
            std::cout << "Key name: " << j.getName() << std::endl;
            isKey = false;
            continue;
        }
        if (isDistance) // distance value {check stoi}
        {
            std::cout << "distance: " << s << std::endl;
            isDistance = false;
        }
        else // junction 
        {
            Junction j(s);
            std::cout << "J name: " << j.getName() << std::endl;
            isDistance = true;
        }
        if(isNewLine(in.peek()))
        {
            isKey = true;
        }
    }

}
