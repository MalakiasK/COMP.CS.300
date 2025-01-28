// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <vector>

// Add your own STL includes below this comment
#include <unordered_map>
#include <algorithm>
#include <unordered_set>
#include <chrono>
#include <set>
#include <math.h>

// Add your own STL includes above this comment

#include "customtypes.hh"

// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Member function size is constant.
    unsigned int get_place_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Clearing containers is a linear operation.
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Without move-semantics, container has to be copied.
    std::vector<PlaceID> all_places();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Linear if reallocation happens.
    bool add_place(PlaceID id, Name const& name, PlaceType type, Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Constant on average, worst case container size.
    Name get_place_name(PlaceID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Constant on average, worst case container size.
    PlaceType get_place_type(PlaceID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Constant on average, worst case container size.
    Coord get_place_coord(PlaceID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: In the worst case is in reverse order, subsequent runs result in linear time if places are not added/removed.
    std::vector<PlaceID> get_places_alphabetically();

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: In the worst case is in reverse order, subsequent runs result in linear time if places are not added/removed.
    std::vector<PlaceID> get_places_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Constant on average, worst case container size.
    std::vector<PlaceID> find_places_with_coord(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Constant on average, worst case container size.
    std::vector<PlaceID> find_places_with_name(Name const& name);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Constant on average, worst case container size.
    std::vector<PlaceID> find_places_with_type(PlaceType type);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::find is linear.
    bool change_place_name(PlaceID id, const Name& newname);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::find is linear.
    bool change_place_coord(PlaceID id, Coord newcoord);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: Worst case linear.
    bool add_area(AreaID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Without move-semantics, container has to be copied.
    std::vector<AreaID> all_areas();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Worst case linear.
    Name get_area_name(AreaID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Worst case linear.
    std::vector<Coord> get_area_coords(AreaID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Worst case linear.
    bool add_subarea_to_area(AreaID id, AreaID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: While loop, worst case linear.
    std::vector<AreaID> ancestor_areas_of_subarea(AreaID id);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<AreaID> all_subareas_of_area(AreaID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PlaceID> get_places_closest_to(Coord xy);

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_place(PlaceID id);

    // Estimate of performance:
    // Short rationale for estimate:
    AreaID get_closest_common_ancestor_of_areas(AreaID id1, AreaID id2);

private:

    // Structure for places
    struct PlaceData {
        Name name;
        PlaceType type;
        Coord coord;
    };

    // Containers for storing places
    std::unordered_map<PlaceID, PlaceData> places;
    // Container for place-occupied coords
    std::unordered_map<Coord, std::vector<PlaceID>> occupied_coords;

    std::unordered_map<Name, std::vector<PlaceID>> place_names;
    std::unordered_map<PlaceType, std::vector<PlaceID>> place_types;

    struct cmpByDist {
        bool operator()(const std::pair<Coord, PlaceID>& a, const std::pair<Coord, PlaceID>& b) const {
            Distance ay2 = pow(a.first.y, 2);
            Distance dist_a = pow(a.first.x, 2) + ay2;
            //dist_a = sqrt(dist_a);
            Distance by2 = pow(b.first.y, 2);
            Distance dist_b = pow(b.first.x, 2) + by2;
            //dist_b = sqrt(dist_b);
            if (dist_a == dist_b) {
                if (ay2 == by2) {
                    return a.second < b.second;
                }
                else {
                    return ay2 < by2;
                }
            }
            return dist_a < dist_b;
        }
    };

    std::set<std::pair<Coord, PlaceID>, cmpByDist> place_dist;

    struct cmpByAlph {
        bool operator()(const std::pair<Name, PlaceID>& a, const std::pair<Name, PlaceID>& b) const {
            const Name na = a.first;
            const Name nb = b.first;
            if (na == nb) {
                return a.second < b.second;
            }
            return na < nb;
        }
    };

    std::set<std::pair<Name, PlaceID>, cmpByAlph> place_alph;

    // Container for place IDs, in alphabetical order
    std::vector<PlaceID> alph_vec;
    bool alph_is_sorted;
    // Container for place IDs, in order of increasing distance
    std::vector<PlaceID> dist_vec;
    bool dist_is_sorted;

    // Containers for areas

    struct AreaData {
        Name name;
        std::vector<Coord> coords;
        AreaID parent;
        std::vector<AreaID> children;
    };

    std::unordered_map<AreaID, AreaData> areas;
    std::vector<AreaID> area_ids;

};

#endif // DATASTRUCTURES_HH
