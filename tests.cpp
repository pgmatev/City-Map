#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.hh"

#include "city.hh"

template <typename Map>
bool map_compare (Map const &lhs, Map const &rhs) {
    // No predicate needed because there is operator== for pairs already.
    return lhs.size() == rhs.size()
        && std::equal(lhs.begin(), lhs.end(),
                      rhs.begin());
}

TEST_CASE("Map creation test")
{
    SUBCASE("Add key test")
    {
        City c;
        CHECK_EQ(c.getMap().size(), 0);
        c.addKey("A");
        CHECK_EQ(c.getMap().size(), 1);
        CHECK(c.getMap().find("A") != c.getMap().end());
        c.addKey("A");
        CHECK_EQ(c.getMap().size(), 1);  
    }
    SUBCASE("Add road test")
    {
        City c;
        c.addKey("A");
        c.addRoad("A", "B", 100);
        REQUIRE_EQ(c.getMap().size(), 2);
        c.addRoad("B", "A", 200);
        REQUIRE_EQ(c.getMap().size(), 2);
        REQUIRE_EQ(c.getMap().find("A")->second.size(), 1);
        REQUIRE_EQ(c.getMap().find("A")->second[0].second, 100);
    }
}

TEST_CASE("Algorithms tests")
{
    SUBCASE("Has path test")
    {
        City c;
        c.addKey("A");
        c.addRoad("A", "B", 4);
        c.addRoad("A", "C", 2);
        c.addRoad("B", "D", 2);
        c.addRoad("B", "C", 3);
        c.addRoad("B", "E", 3);
        c.addRoad("C", "D", 4);
        c.addRoad("C", "E", 5);
        c.addRoad("E", "D", 1);
        CHECK(c.hasPath("A", "E"));
        CHECK_FALSE(c.hasPath("D", "B"));
    }
    SUBCASE("Has cycle test")
    {
        City c;
        c.addKey("A");
        c.addRoad("A", "B", 4);
        c.addRoad("A", "C", 2);
        c.addRoad("B", "D", 2);
        c.addRoad("B", "A", 1);
        c.addRoad("B", "C", 3);
        c.addRoad("B", "E", 3);
        c.addRoad("C", "D", 4);
        c.addRoad("C", "E", 5);
        c.addRoad("E", "D", 1);
        CHECK(c.hasCycle("A"));
        CHECK_FALSE(c.hasCycle("E"));
    }
    SUBCASE("K shortest paths test")
    {
        City c;
        c.addKey("A");
        c.addRoad("A", "B", 4);
        c.addRoad("A", "C", 2);
        c.addRoad("B", "D", 2);
        c.addRoad("B", "C", 3);
        c.addRoad("B", "E", 3);
        c.addRoad("C", "B", 1);
        c.addRoad("C", "D", 4);
        c.addRoad("C", "E", 5);
        c.addRoad("E", "D", 1);
        std::map<int, std::vector<std::string>> results = {{3, std::vector<std::string>{"A", "C", "B"}},
                                                          {4, std::vector<std::string>{"A", "B"}},
                                                          {7, std::vector<std::string>{"A", "C", "B", "C", "B"}}};
        CHECK(map_compare(c.kShortestPaths("A", "B", 3), results));
    }
    SUBCASE("Reach all other junctions test")
    {
        City c;
        c.addKey("A");
        c.addRoad("A", "B", 4);
        c.addRoad("A", "C", 2);
        c.addRoad("B", "D", 2);
        c.addRoad("B", "C", 3);
        c.addRoad("B", "E", 3);
        c.addRoad("C", "B", 1);
        c.addRoad("C", "D", 4);
        c.addRoad("C", "E", 5);
        c.addRoad("E", "D", 1);
        CHECK(c.areReachable("A"));
        CHECK_FALSE(c.areReachable("B"));
    }
    SUBCASE("Dead-ends test")
    {
        City c;
        c.addKey("A");
        c.addRoad("A", "B", 4);
        c.addRoad("A", "C", 2);
        c.addRoad("A", "G", 10);
        c.addRoad("B", "D", 2);
        c.addRoad("B", "C", 3);
        c.addRoad("B", "E", 3);
        c.addRoad("C", "B", 1);
        c.addRoad("C", "D", 4);
        c.addRoad("C", "E", 5);
        c.addRoad("E", "D", 1);
        std::vector<std::pair<std::string, std::string>> results = {{"A", "G"}, {"B", "D"}, {"C", "D"}, {"E", "D"}};
        std::vector<std::pair<std::string, std::string>> hm = c.deadEnds();
        CHECK_EQ(c.deadEnds(), results);

    }
    SUBCASE("Eurelian Circuit")
    {
        City c;
        c.addKey("A");
        c.addRoad("A", "B", 4);
        c.addRoad("A", "C", 2);
        c.addRoad("A", "D", 10);
        c.addRoad("B", "C", 3);
        c.addRoad("D", "C", 7);
        CHECK_FALSE(c.hasEurelianCycle());
        City c2;
        c2.addKey("A");
        c2.addRoad("A", "B", 4);
        c2.addRoad("B", "C", 2);
        c2.addRoad("C", "D", 10);
        c2.addRoad("D", "B", 3);
        c2.addRoad("B", "G", 7);
        c2.addRoad("G", "A", 8);
        CHECK(c2.hasEurelianCycle());
    }
}