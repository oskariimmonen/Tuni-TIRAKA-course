// Datastructures.hh
//
// Student name: Oskari Immonen
// Student email: oskari.j.immonen@tuni.fi
// Student number: H291868

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <map>
#include <unordered_map>
#include <memory>
#include <deque>
#include <climits>
#include <set>

// Types for IDs
using TownID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
TownID const NO_TOWNID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

enum State { WHITE, GRAY, BLACK };

struct Road
{
    Road() : town1(), town2() {}
    Road(TownID newTown1, TownID newTown2)
        : town1(newTown1), town2(newTown2) {}
    TownID town1;
    TownID town2;

    bool operator ==(const Road &other) const
    {
        return((town1 == other.town1) or (town1 == other.town2 && town2 == other.town1));
    }
};

struct RoadHash {
    std::size_t operator()(Road road) const
    {
        auto hasher = std::hash<TownID>();
        auto town1Hash = hasher(road.town1);
        auto town2Hash = hasher(road.town2);
        return town1Hash * town2Hash;
    }
};

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance:
    // Short rationale for estimate:
    unsigned int town_count();

    // Estimate of performance: Omega(1) and O(n)
    // Short rationale for estimate: Same as clear_roads
    void clear_all();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_town(TownID id, Name const& name, Coord coord, int tax);

    // Estimate of performance:
    // Short rationale for estimate:
    Name get_town_name(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    Coord get_town_coordinates(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    int get_town_tax(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> all_towns();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> find_towns(Name const& name);

    // Estimate of performance:
    // Short rationale for estimate:
    bool change_town_name(TownID id, Name const& newname);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance:
    // Short rationale for estimate:
    TownID min_distance();

    // Estimate of performance:
    // Short rationale for estimate:
    TownID max_distance();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> get_town_vassals(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory phase 1 operations

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_town(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> towns_nearest(Coord coord);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    int total_net_tax(TownID id);


    // Phase 2 operations

    // Estimate of performance: Omega(1) and O(n)
    // Short rationale for estimate: If Roads map is clear does nothing. Otherwise .clear is a linear operation according to cppreference.
    void clear_roads();

    // Estimate of performance: Theta(1) if Roads is clear, otherwise Theta(n)
    // Short rationale for estimate: because it iterates through the whole Roads map --> linear operation
    std::vector<std::pair<TownID, TownID>> all_roads();

    // Estimate of performance: O(n) and Omega(1)
    // Short rationale for estimate: In worst case .find is linear but Theta(1) on average. Other parts that could affect the complexity are just insertions so O(n) is a good estimate.
    bool add_road(TownID town1, TownID town2);

    // Estimate of performance: O(n) and Omega(1)
    // Short rationale for estimate: If town is not just returns NO_TOWNID (push_back is constant). Otherwise iterates through database so is linear function.
    std::vector<TownID> get_roads_from(TownID id);

    // Estimate of performance: O((n+k) log n) any route = shortest route
    // Short rationale for estimate: Idea of the function is based on Dijkstra's algorithim so this is the estimated efficiency
    std::vector<TownID> any_route(TownID fromid, TownID toid);

    // Non-compulsory phase 2 operations

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_road(TownID town1, TownID town2);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> least_towns_route(TownID fromid, TownID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> road_cycle_route(TownID startid);

    // Estimate of performance: SEE ANY_ROUTE THIS IS THE SAME
    // Short rationale for estimate: -II-
    std::vector<TownID> shortest_route(TownID fromid, TownID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    Distance trim_road_network();

private:
    // Add stuff needed for your class implementation here
    //Towns' data stored here. Vassals data in other datastructure.
    std::unordered_map<std::string, std::vector<std::string>> Database;

    //return a vector of map values in sorted order
    // Estimate of performance: Theta(n log n)
    // Short rationale for estimate: for-loop is linear. std::sort is nlogn and for is linear again
    std::vector<std::string> sort_map(std::unordered_map<std::string, int>&M);

    // Vassal relations' info stored here
    std::unordered_map<std::string, std::string> Vassals;

    //Called when changes are made to towns Database to fix vassal relations.
    // Estimate of performance: Theta(n)
    // Short rationale for estimate: If param is not found .find is linear. Otherwise we continue and for loop iterates through the Vassals data so it is linear
    void fix_vassals(std::string);

    // returns a map where original maps key and value pairs are swapped
    // Estimate of performance: Theta(n)
    // Short rationale for estimate: Swaps the values so is always directly dependent on the amount of the data.
    std::unordered_map<std::string, std::string> swap_key_and_value(std::unordered_map<std::string, std::string> map);


    //Routes are stored here
    std::multimap<std::string, std::string> Roads;

    //Data of roads stored here
    std::unordered_map<Road, int, RoadHash> roads;

};

#endif // DATASTRUCTURES_HH
