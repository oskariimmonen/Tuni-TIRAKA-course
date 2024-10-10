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

    // NOTE: I used course material and cppreference in all cases when I was estimating the performance


    // Estimate of performance: Theta(n) (O(n) and Omega(n))
    // Short rationale for estimate: In all cases function iterates through the map where towns are stored.
    // While loop iterates as many times as there are towns
    unsigned int town_count();

    // Estimate of performance: Omega(1) and O(n)
    // Short rationale for estimate: beacause if databases are clear, the function does nothing that is dependent on data.
    // And according to cppreference .clear is linear function so in case there is data .clear has O(n) efficiency.
    void clear_all();

    // Estimate of performance: O(n) and Omega(1)
    // Short rationale for estimate: In the worst case .find is linear but on average we can say that it is
    //  constant Theta(1) according to cppreference. Other lines in the function are not dependent on the amount of data.
    bool add_town(TownID id, Name const& name, Coord coord, int tax);

    // Estimate of performance: O(n) and Omega(1)
    // Short rationale for estimate: .find again so on average the efficiency is constant and linear in worst case.
    // Search from unordered_map on "town_name = Database[key].at(0);" is linear but theta notation is constant on average.
    Name get_town_name(TownID id);

    // Estimate of performance: O(n) and Omega(1)
    // Short rationale for estimate: .find and Database searches again
    Coord get_town_coordinates(TownID id);

    // Estimate of performance: O(n) and Omega(1)
    // Short rationale for estimate: Basically the same rationales as two previous functions
    int get_town_tax(TownID id);

    // Estimate of performance: O(n) and Omega(1)
    // Short rationale for estimate: In all cases dependent on the total amount of town so it is linear.
    // With the execption when there is no towns at all. Then the function does nothing that is dependent on data so Omega(1)
    std::vector<TownID> all_towns();

    // Estimate of performance: O(n) and Omega(1)
    // Short rationale for estimate: Always dependent of the total amount of towns so it is linear.  Vector push_back is constant
    // If there is no towns, does nothing
    std::vector<TownID> find_towns(Name const& name);

    // Estimate of performance: Theta(1)
    // Short rationale for estimate: Only thing that could slow this down is when we put data in unordered_map
    // according to course material and cppreference one can estimate that the insertion is (1) with theta notation.
    bool change_town_name(TownID id, Name const& newname);

    // Estimate of performance: O(n log n) and Omega(1)
    // Short rationale for estimate: For-loop is linear operation. Inside while loop (linear) is a insertion to a map which is log n operation.
    // So log n inside of linear loop. If no towns -> does nothing
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance: O(n log n) and Omega(1)
    // Short rationale for estimate: While loop is linear because it iterates thtough the database. Things inside of it are constant operations
    // My own function is called which has the performance of n log on because of sorting. Therefore performance is (n log n). If towns are not found, does nothing
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance: O(n log n) and Omega(1)
    // Short rationale for estimate: town_distance_increasing is called. Other parts are not dependent on the data.
    // If no towns -> constant
    TownID min_distance();

    // Estimate of performance: O(n log n) and Omega(1)
    // Short rationale for estimate: towns_distance increasing called. Other parts not dependent on the data. and if no  towns -> nothing that is dependent on data
    TownID max_distance();

    // Estimate of performance: Theta(1)
    // Short rationale for estimate: According to course material insertion with unordered map can be estimated to be constant
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance: Theta(n)
    // Short rationale for estimate: .find will be linear if no id is found. while loop iterates through the database so it is linear
    std::vector<TownID> get_town_vassals(TownID id);

    // Estimate of performance: Theta(n)
    // Short rationale for estimate: first if sentence will be linear if used because of .find
    // while loop is linear because it only finds wanted values. Operations inside of it have the performance of (1)
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory phase 1 operations

    // Estimate of performance: Theta(n)
    // Short rationale for estimate: If id is found .find is O(n) and .erase as well. If not found .find is O(n)
    // so it is always linear.
    bool remove_town(TownID id);

    // Estimate of performance: O(n log n) and Omega(1)
    // Short rationale for estimate: If no towns are found then nothing that requires saved data is done -> Omega(1)
    // If towns are found, while loop iterates throuhgh the database making it linear. Operations inside of it are constant. sort_map function is n log n.
    // NOTE! : When the distance to the desired coordinate is the same with two towns, the function does not sort them alphabetically as in the example
    // in the project instruction file. It was not required so I chose not to do it
    std::vector<TownID> towns_nearest(Coord coord);

    // Estimate of performance: Theta(n)
    // Short rationale for estimate: If no  towns are found .find is linear. swap_key_and_value is linear. while loop depends on the amount of found cities
    // So final performance is linear.
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance: Theta(n)
    // Short rationale for estimate: If id is not found .find is linear. If id is found --> get_town_vassals is linear, swap_key_and_value is linear
    // Other operations are constant
    int total_net_tax(TownID id);

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

};

#endif // DATASTRUCTURES_HH
