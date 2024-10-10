// Datastructures.cc
//
// Student name: Oskari Immmonen
// Student email: oskari.j.immonen@tuni.fi
// Student number: H291868

#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

unsigned int Datastructures::town_count()
{
    unsigned int count = 0;
    std::unordered_map<std::string,std::vector<std::string>>::iterator iter;
    iter = Database.begin();

    while (iter != Database.end()) {
        count++;
        iter++;
    }
    return count;


}

void Datastructures::clear_all()
{
    if (!Database.empty()) {
       Database.clear();
    }
    if(!Vassals.empty()) {
        Vassals.clear();
    }
    if(!Roads.empty()) {
        Roads.clear();
    }
    if(!roads.empty()){
        roads.clear();
    }
}

bool Datastructures::add_town(TownID id, const Name &name, Coord coord, int tax)
{
    std::string town_id = id;
    std::string town_name = name;
    Coord a = coord;
    std::string tax_value = std::to_string(tax);

    std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
    iter = Database.find(town_id);
    if (iter != Database.end()) {
        return false;
    } else {


    std::string first_coord = std::to_string(a.x);
    std::string second_coord = std::to_string(a.y);

    std::vector<std::string> info = {town_name, first_coord, second_coord, tax_value};

    Database.insert({town_id, info});
    return true;
    }

}

Name Datastructures::get_town_name(TownID id)
{
    std::string key = id;
    std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
    iter = Database.find(key);
    if (iter == Database.end()) {
        return NO_NAME;
    } else {
    Name town_name;
    town_name = Database[key].at(0);
    return town_name;
    }

}

Coord Datastructures::get_town_coordinates(TownID id)
{
    std::string key = id;
    Coord to_return;
    int first_coord =0;
    int second_coord = 0;
    std::string first,second;

    std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
    iter = Database.find(key);
    if(iter == Database.end()) {
        return NO_COORD;
    } else {

    first = Database[key].at(1);
    second = Database[key].at(2);

    first_coord = std::stoi(first);
    second_coord = std::stoi(second);

    to_return.x = first_coord;
    to_return.y = second_coord;

    return to_return;
   }


}

int Datastructures::get_town_tax(TownID id)
{
    std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
    iter = Database.find(id);
    if (iter == Database.end()) {
        return NO_VALUE;
    } else {
        int tax = 0;
        std::string tax_string;
        tax_string = Database[id].at(3);
        tax = std::stoi(tax_string);
        return tax;
    }

}

std::vector<TownID> Datastructures::all_towns()
{
    std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
    iter = Database.begin();
    std::vector<std::string> result;

    while(iter != Database.end()) {
        result.push_back(iter->first);
        iter++;
    }
    return result;
}

std::vector<TownID> Datastructures::find_towns(const Name &name)
{
    std::string wanted_name = name;
    std::vector<std::string> result;
    std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
    iter = Database.begin();
    while(iter != Database.end()) {
        if (Database[iter->first].at(0) == wanted_name) {
            result.push_back(iter->first);
        }

        iter++;
    }

    return result;
}

bool Datastructures::change_town_name(TownID id, const Name &newname)
{
    std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
    iter = Database.begin();
    if(iter == Database.end()) {
        return false;
    } else {
        Database[id].at(0) = newname;
        return true;
    }

}

std::vector<TownID> Datastructures::towns_alphabetically()
{
    std::vector<std::string> result;
    std::map<std::string, std::string> temp;
    std::unordered_map<std::string, std::vector<std::string>>::iterator iter;

    iter = Database.begin();
    while (iter != Database.end()) {
        temp.insert({Database[iter->first].at(0), iter->first});
        iter++;
    }
    std::map<std::string, std::string>::iterator itr;
    for(itr = temp.begin(); itr != temp.end(); itr++) {
        result.push_back(itr->second);
    }
    return result;
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    std::vector<std::string> result;
    std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
    std::unordered_map<std::string, int> data;
    int first_coord = 0;
    int second_coord = 0;
    int distance = 0;

    iter = Database.begin();
    while(iter != Database.end()) {
       first_coord = std::stoi(Database[iter->first].at(1));
       second_coord = std::stoi(Database[iter->first].at(2));
       distance = std::sqrt(std::pow(first_coord,2) + std::pow(second_coord,2));
       data.insert({iter->first, distance});
       ++iter;
    }

    result = sort_map(data);

    return result;
}

TownID Datastructures::min_distance()
{
    if(Database.empty()) {
        return NO_TOWNID;
    } else {
        std::vector<std::string> result;
        result = towns_distance_increasing();
        return result.at(0);
    }
}

TownID Datastructures::max_distance()
{
    if(Database.empty()) {
        return NO_TOWNID;
    } else {
        std::vector<std::string> result;
        result = towns_distance_increasing();
        int size = result.size();
        return result.at(size-1);
    }
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{

    Vassals.insert({vassalid, masterid});

   return true;
}

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{
    std::vector<std::string> result;
    if(Database.find(id) == Database.end()) {
        result.push_back(NO_TOWNID);
        return result;
    }

    std::unordered_map<std::string, std::string>::iterator iter;
    iter = Vassals.begin();
    while (iter != Vassals.end()) {
        if(iter->second == id) {
            result.push_back(iter->first);
        }
        iter++;
    }
    return result;
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    std::vector<std::string> result;
    if (Database.find(id) == Database.end()) {
        result.push_back(NO_TOWNID);
        return result;
    }

    std::string city = id;
    result.push_back(city);
   std::unordered_map<std::string, std::string> temp = Vassals;
   while (temp.find(city) != temp.end()) {
       std::unordered_map<std::string, std::string>::iterator it;
       it = temp.find(city);
       result.push_back(it->second);
       city = it->second;
       temp.erase(it);
   }


    return result;
}

bool Datastructures::remove_town(TownID id)
{
    std::unordered_map<std::string, std::vector<std::string>>::iterator it;
    it = Database.find(id);
    if(it != Database. end()) {
        Database.erase(it);
        fix_vassals(id);
        return true;
    } else {
        return false;
    }
}

std::vector<TownID> Datastructures::towns_nearest(Coord coord)
{
    std::vector<std::string> result;
    std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
    std::unordered_map<std::string, int> data;
    int first_coord = 0;
    int second_coord = 0;
    int distance = 0;
    int x_compare = coord.x;
    int y_compare = coord.y;

    iter = Database.begin();
    while(iter != Database.end()) {
       first_coord = std::stoi(Database[iter->first].at(1));
       second_coord = std::stoi(Database[iter->first].at(2));
       distance = floor(std::sqrt(std::pow(first_coord - x_compare,2) + std::pow(second_coord - y_compare,2)));
       data.insert({iter->first, distance});
       ++iter;
    }

    result = sort_map(data);

    return result;
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID id)
{
    std::vector<std::string> result;
    std::unordered_map<std::string, std::vector<std::string>>::iterator it;
    it = Database.find(id);
    if(it == Database.end()) {
        result.push_back(NO_TOWNID);
        return result;
    }
    std::unordered_map<std::string, std::string> temp;
    temp = Vassals;
    temp = swap_key_and_value(temp);

    std::string city = id;
    result.push_back(city);

    while (temp.find(city) != temp.end()) {
        std::unordered_map<std::string, std::string>::iterator it;
        it = temp.find(city);
        result.push_back(it->second);
        city = it->second;
        temp.erase(it);
    }

    return result;
}

int Datastructures::total_net_tax(TownID id)
{
    std::unordered_map<std::string, std::vector<std::string>>::iterator it;
    it = Database.find(id);
    if(it == Database.end()) {
        return NO_VALUE;
    }

    int total_tax = std::stoi(Database[id].at(3));
    std::vector<std::string> vassals;
    vassals = get_town_vassals(id);

    std::unordered_map<std::string, std::string>::iterator iter;
    iter = Vassals.find(id);
    std::unordered_map<std::string, std::string> reversed;
    reversed = swap_key_and_value(Vassals);
    std::unordered_map<std::string, std::string>::iterator swapped;
    swapped = reversed.find(id);

    if(vassals.empty() and iter == Vassals.end()) {
        return total_tax;

    }

    else if (iter != Vassals.end() and swapped == reversed.end()) {
        return floor(total_tax*0.9);
    }


    else if(swapped != reversed.end() and iter == Vassals.end()){
        for (auto const& i:vassals) {
            int taxes_from_vassals = total_net_tax(i);
            total_tax += floor((taxes_from_vassals/0.9)*0.1);
        }
        return total_tax;
    }
    else {
        for (auto const& i:vassals) {
            int taxes_from_vassals = total_net_tax(i);
            total_tax += floor((taxes_from_vassals/0.9)*0.1);
        }
        return floor(total_tax*0.9);
    }

    return 0;
}

std::vector<std::string> Datastructures::sort_map(std::unordered_map<std::string, int>&M) {
    std::vector<std::pair<std::string, int> > A;

    for (auto& it : M) {
           A.push_back(it);
       }

    std::sort(A.begin(), A.end(),[] (std::pair<std::string, int>& a,
                                   std::pair<std::string, int>& b) {return a.second < b.second;});
    std::vector<std::string> result;
    for (auto& it : A) {

         result.push_back(it.first);
        }

    return result;
}
void Datastructures::fix_vassals(std::string param) {
    std::unordered_map<std::string, std::string>::iterator it;
    it = Vassals.find(param);
    if (it != Vassals.end()) {
        std::string helsinki = it->second;
        for(auto iter = Vassals.begin(); iter != Vassals.end(); ++iter) {
            if (iter->second == param) {
                iter->second = helsinki;
            }


        }

      Vassals.erase(it);

    }

}

std::unordered_map<std::string, std::string> Datastructures::swap_key_and_value(std::unordered_map<std::string, std::string> map) {
    std::unordered_map<std::string, std::string> reversed;

    for(std::unordered_map<std::string, std::string>::iterator i = map.begin(); i != map.end(); ++i) {
        reversed[i->second] = i->first;
    }
    return reversed;
}


//
// Phase 2 operations
//


void Datastructures::clear_roads()
{
    if(!Roads.empty()) {
        Roads.clear();
        roads.clear();
    }
}

std::vector<std::pair<TownID, TownID>> Datastructures::all_roads()
{
    std::vector<std::pair<std::string, std::string>> every_road;
    std::multimap<std::string, std::string>::iterator iter;
    iter = Roads.begin();

    while(iter != Roads.end()) {
        if (iter->first < iter->second) {
            every_road.push_back(std::make_pair(iter->first, iter->second));
        } else {
            every_road.push_back(std::make_pair(iter->second, iter->first));
        }

        iter++;
    }


    return every_road;


}

bool Datastructures::add_road(TownID town1, TownID town2)
{
    std::unordered_map<std::string, std::vector<std::string>>::iterator iter1;
    iter1 = Database.find(town1);
    if(iter1 == Database.end()) {
        return false;
    }
    iter1 = Database.find(town2);
    if(iter1 == Database.end()) {
        return false;
    }

    std::multimap<std::string, std::string>::iterator iter;
    iter = Roads.find(town1);

    if(iter != Roads.end() and iter->second == town2) {
        return false;
    }
    iter = Roads.find(town2);
    if(iter != Roads.end() and iter->second == town1) {
        return false;
    }

    Roads.insert({town1, town2});

    int dist;
    int x1,x2,y1,y2;
    x1 = std::stoi(Database[town1].at(1));
    x2 = std::stoi(Database[town2].at(1));
    y1 = std::stoi(Database[town1].at(2));
    y2 = std::stoi(Database[town2].at(2));

    dist = floor(sqrt(pow((x1-x2),2)+pow((y1-y2),2)));

    if(roads.insert({Road(town1, town2), dist}).second) {
        return true;
    } else {
        return false;
    }


    return true;
}
std::vector<TownID> Datastructures::get_roads_from(TownID id)
{
   std::vector<std::string> result;
    std::unordered_map<std::string, std::vector<std::string>>::iterator it;

    it = Database.find(id);
    if(it == Database.end()) {
        result.push_back(NO_TOWNID);
        return result;
    }

    std::multimap<std::string, std::string>::iterator iter;
    iter = Roads.begin();
    while(iter != Roads.end()) {
        if(iter->first == id){
            result.push_back(iter->second);
        } else if(iter->second == id) {
            result.push_back(iter->first);
        }
        iter++;
    }

    return result;


}

std::vector<TownID> Datastructures::any_route(TownID fromid, TownID toid)
{
  return shortest_route(fromid, toid);
 }
bool Datastructures::remove_road(TownID /*town1*/, TownID /*town2*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("remove_road()");
}

std::vector<TownID> Datastructures::least_towns_route(TownID /*fromid*/, TownID /*toid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("least_towns_route()");
}

std::vector<TownID> Datastructures::road_cycle_route(TownID /*startid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("road_cycle_route()");
}

std::vector<TownID> Datastructures::shortest_route(TownID fromid, TownID toid)
{
    std::vector<std::string> result;
    std::unordered_map<std::string, std::vector<std::string>>::iterator iter1;
    std::unordered_map<std::string, std::vector<std::string>>::iterator iter2;

    iter1 = Database.find(fromid);
    iter2 = Database.find(toid);

    if(iter1 == Database.end()) {
        result.push_back(NO_TOWNID);
        return result;
    }
    if(iter2 == Database.end()) {
        result.push_back(NO_TOWNID);
        return result;
    }

    auto allTowns = towns_alphabetically();
    int sizeAllTowns = allTowns.size();
    int graph[sizeAllTowns][sizeAllTowns];
    volatile int src = 0;
    volatile int to = 0;
    volatile int u = 0;

    for(int i = 0; i < sizeAllTowns; i++){
        if(allTowns[i] == fromid) {
            src = i;
        }
        for(int e = 0; e < sizeAllTowns; e++){
            if(allTowns[e] == toid){
                to = e;
            }
            if(roads.find(Road(allTowns[i], allTowns[e])) != roads.end()){
                graph[i][e] = roads.at(Road(allTowns[i], allTowns[e]));
            } else {
                graph[i][e] = 0;
            }
        }
    }

    int dist[sizeAllTowns];
    std::vector<TownID> townsBetween[sizeAllTowns];

    bool sptSet[sizeAllTowns];

    for(int i = 0; i < sizeAllTowns; i++)
        dist[i] = std::numeric_limits<int>::max(), sptSet[i] = false;

    dist[src] = 0;

    for(int count = 0; count < sizeAllTowns - 1; count++){
        int min = std::numeric_limits<int>::max();
        for(int v = 0; v < sizeAllTowns; v++) {
            if(sptSet[v] == false && dist[v] <= min){
                min = dist[v], u = v;
            }
        }
        sptSet[u] = true;
        for(int v = 0; v < sizeAllTowns; v++){
            if(!sptSet[v] && graph[u][v] && dist[u] != std::numeric_limits<int>::max() && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                townsBetween[v] = townsBetween[u];
                townsBetween[v].push_back(allTowns[u]);
            }
        }
    }
    if(!townsBetween[to].empty()){
        townsBetween[to].push_back(toid);
    }
    return townsBetween[to];
}

Distance Datastructures::trim_road_network()
{
    // Replace the line below with your implementation
    throw NotImplemented("trim_road_network()");
}
