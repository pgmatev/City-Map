#include "program.hh"

void Program::run(int argc, char* argv[])
{
    if (argc == 1) //not starting in interactive mode
    {
        return;
    }
    if (argc != 4)
    {
        throw std::invalid_argument("Wrong number of arguments!");
    }

    if (strcmp(argv[1], "-i") == 0)
    {
        std::string dotty = "graph.dot";
        takeInput(argv[2], dotty, argv[3]); //load the map 
        std::cout << "Interactive mode initialised, type \"exit\" to quit" << std::endl;
        std::string input;
        while (input != "exit")
        {
            std::cout << ">";
            std::getline(std::cin, input);
            //==/ no argument commands /==
            if (input == "location")
            {
                printLocation();
                continue;
            }
            if (input == "neighbours")
            {
                printNeighbours();
                continue;
            }
            if (input == "closed")
            {
                printClosedJunctions();
                continue;
            }
            if (input == "tour")
            {
                touristLap();
                continue;
            }
            if (input == "dead-ends")
            {
                deadEnds();
                continue;
            }
            //==/ multiple argument commands /==
            std::stringstream ss(input);
            std::string s;
            std::vector<std::string> commands;
            while(ss >> s)
            {
                commands.push_back(s); //splitting the input by command arguments
            }
            try
            {
                if (commands[0] == "change")
                {
                    if (commands.size() != 2)
                    {
                        throw std::invalid_argument("The command \"change\" takes 1 argument");
                    }
                    changeLocation(commands[1]);
                    toDotty(dotty); //updates visual map
                    continue;
                }
                if (commands[0] == "move")
                {
                    if (commands.size() != 2)
                    {
                        throw std::invalid_argument("The command \"move\" takes 1 argument");
                    }
                    move(commands[1]);
                    toDotty(dotty); //updates visual map
                    continue;
                }
                if (commands[0] == "close")
                {
                    if (commands.size() != 2)
                    {
                        throw std::invalid_argument("The command \"close\" takes 1 argument");
                    }
                    closeJunction(commands[1]);
                    toDotty(dotty); //updates visual map
                    continue;
                }
                if (commands[0] == "open")
                {
                    if (commands.size() != 2)
                    {
                        throw std::invalid_argument("The command \"open\" takes 1 argument");
                    }
                    openJunction(commands[1]);
                    toDotty(dotty); //updates visual map
                    continue;
                }
                if (commands[0] == "path")
                {
                    if (commands.size() != 3)
                    {
                        throw std::invalid_argument("The command \"path\" takes 2 arguments");
                    }
                    hasPath(commands[1], commands[2]);
                    continue;
                }
                if (commands[0] == "cycle")
                {
                    if (commands.size() != 2)
                    {
                        throw std::invalid_argument("The command \"path\" takes 1 argument");
                    }
                    hasCycle(commands[1]);
                    continue;
                }
                if (commands[0] == "reachable")
                {
                    if (commands.size() != 2)
                    {
                        throw std::invalid_argument("The command \"reachable\" takes 1 argument");
                    }
                    areReachable(commands[1]);
                    continue;
                }
                if (commands[0] == "shortest-paths")
                {
                    if (commands.size() != 3)
                    {
                        throw std::invalid_argument("The command \"shortest-paths\" takes 2 arguments");
                    }
                    threeShortestPaths(commands[1], commands[2]);
                    continue;
                }
            }
            catch (std::invalid_argument& e)
            {
                std::cout << e.what() << std::endl;
            }
        }
    }
}

bool Program::isNewLine(char c)
{
    return c == '\r' || // also a new line symbol
           c == '\n';
}

void Program::takeInput(const std::string& filename, const std::string& dot_filename, const std::string& starting_location)
{
    std::ifstream in(filename);
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
    changeLocation(starting_location);
    toDotty(dot_filename);
    in.close();
}

void Program::passClosedJunctions(const std::string& s)
{
    std::stringstream ss(s);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> v(begin, end);
    try
    {
        loaded_city.setClosedJunctions(v);
    }
    catch (std::invalid_error& e)
    {
        std::cout << e.what() << std::endl;
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

void Program::printLocation()
{
    std::cout << "Current location: " << loaded_city.getLocation() << std::endl;
}

void Program::printClosedJunctions()
{
    std::cout << "Closed junctions: ";
    std::vector<std::string> closed_junctions = loaded_city.getClosedJunctions();
    for (auto v : closed_junctions)
    {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

void Program::toDotty(const std::string& filename) // writes to a file in the .dot language syntax
{                                       // visualisation of the graph using graphviz
    std::ofstream out(filename);
    out << "digraph G {\n";
    toDottyHelper(out);
    out << "}";
    out.close();
}
void Program::toDottyHelper(std::ostream& out)
{
    for (auto it : loaded_city.getMap())
    {
        auto j1 = loaded_city.getMap().find(it.first);
        if (std::find(loaded_city.getClosedJunctions().begin(), 
                      loaded_city.getClosedJunctions().end(),
                      j1->first) == loaded_city.getClosedJunctions().end()) //if the junction is not closed
        {
            if (j1->first == loaded_city.getLocation())
            {
                out << (long)&j1->first << "[label=\"" << j1->first << "\" fontcolor = \"blue\"];\n";
            }
            else
            {
                out << (long)&j1->first << "[label=\"" << j1->first << "\"];\n";
            }
        }
        else // if the junction is closed
        {
            if (j1->first == loaded_city.getLocation())
            {
                out << (long)&j1->first << "[label=\"" << j1->first << "\" color = \"red\" fontcolor = \"blue\"];\n";
            }
            else
            {
                out << (long)&j1->first << "[label=\"" << j1->first << "\" color = \"red\"];\n";
            }

        }
        for (auto v : j1->second)
        {
            auto j2 = loaded_city.getMap().find(v.first);
            out << (long)&j1->first << "->" << (long)&j2->first << "[label = \""<< v.second << "\" color = \"blue\"]\n";
        }
    }
}

void Program::changeLocation(const std::string& new_location)
{
    try
    {
        loaded_city.setLocation(new_location);
    }
    catch (std::invalid_argument& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void Program::printNeighbours()
{
    std::vector<std::pair<std::string, int>> neighbours = loaded_city.getNeighbours();
    std::cout << "Neighbours of " << loaded_city.getLocation() << ": ";
    for (auto v : neighbours)
    {
        std::cout << "("<< v.first << ", distance " << v.second << ") ";
    }
    std::cout << std::endl;
}

void Program::move(const std::string& new_location)
{
    try
    {
        std::cout << "Moving from " << loaded_city.getLocation() << " to " << new_location << "...\n";
        std::vector<std::string> path = loaded_city.move(new_location);
        for (auto it = path.begin(); it != path.end(); ++it)
        {
            std::cout << *it; //it is iterator pointing towards the string
            if (std::next(it) != path.end()) //doesn't put arrow after last element
            {
                std::cout << "->";
            }
        }
        std::cout << std::endl;
    }
    catch (std::invalid_argument& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void Program::closeJunction(const std::string& junction)
{
    try
    {   
        loaded_city.closeJunction(junction);
        std::cout << "Closing " << junction << std::endl;
    }
    catch(std::invalid_argument& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void Program::openJunction(const std::string& junction)
{
    try
    {   
        loaded_city.openJunction(junction);
        std::cout << "Opening " << junction << std::endl;
    }
    catch(std::invalid_argument& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void Program::hasPath(const std::string& source, const std::string& destination)
{
    if (loaded_city.hasPath(source, destination))
    {
        std::cout << "There is a path between " << source << " and " << destination << std::endl;
    }
    else
    {
        std::cout << "There is no path between " << source << " and " << destination << std::endl;
    }
}

void Program::hasCycle(const std::string& source)
{
    if(loaded_city.hasCycle(source))
    {
        std::cout << "It is possible to return to " << source << std::endl;
    }
    else
    {
        std::cout << "It is not possible to return to " << source << std::endl;
    }
}

void Program::areReachable(const std::string& source)
{
    if (loaded_city.areReachable(source))
    {
        std::cout << "There are paths to all other junctions from " << source << std::endl; 
    }
    else
    {
        std::cout << "Cannot reach every other junction from " << source << std::endl; 
    }
}

void Program::deadEnds()
{
    std::cout << "Dead-end streets: ";
    for (auto it : loaded_city.deadEnds())
    {
        std::cout << "{" << it.first << ", " << it.second << "} ";
    }
    std::cout << std::endl;
}

//to do: make sure source and destination are valid junctions
void Program::threeShortestPaths(const std::string& source, const std::string& destination)
{
    std::map<int, std::vector<std::string>>  results = loaded_city.kShortestPaths(source, destination, 3); //it can work with k amount of paths
    try
    {
        if (results.size() < 3)
        {
            throw results.size();
        }
    }catch(unsigned long e)
    {
        if(e == 0)
        {
            std::cout << "There are no paths between " << source << " and " << destination << std::endl;
            return;
        }
        else
        {
            std::cout << "There are only " << e << " path/s between " << source << " and " << destination << std::endl;
        }

    }

    std::cout << "The shortest paths from " << source << " to " << destination << ": " << std::endl;
    for (auto i : results)
    {
        for (auto j : i.second)
        {
            std::cout << j << " ";
        }
        std::cout << "= " << i.first << std::endl;
    }
}

void Program::touristLap()
{
    std::vector<std::string> reversed_lap = loaded_city.generateEurelianCircuit();
    if (reversed_lap.empty())
    {
        std::cout << "No such lap exists." << std::endl;
        return;
    }

    std::cout << "Possible lap: ";
    for (auto it = reversed_lap.rbegin(); it != reversed_lap.rend(); ++it) //reversed iterator, incrementing is actually decrementing
    {
        std::cout << (*it) << " ";
    }
    std::cout << std::endl;
}