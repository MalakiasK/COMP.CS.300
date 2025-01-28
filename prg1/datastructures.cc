/**
 * @brief The Datastructures class
 * STUDENTS: Modify the code below to implement the functionality of the class.
 * Also remove comments from the parameter names when you implement an operation
 * (Commenting out parameter name prevents compiler from warning about unused
 * parameters on operations you haven't yet implemented.)
 */

#include "datastructures.hh"
#include "customtypes.hh"

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)
Datastructures::Datastructures()
{
    alph_vec.reserve(100000);
    dist_vec.reserve(100000);
}

Datastructures::~Datastructures()
{

}

unsigned int Datastructures::get_place_count()
{
    return places.size();
}

void Datastructures::clear_all()
{
    places.clear();
    place_dist.clear();
    place_alph.clear();

    occupied_coords.clear();

    place_names.clear();
    place_types.clear();

    alph_vec.clear();
    dist_vec.clear();

    areas.clear();
    area_ids.clear();
}

std::vector<PlaceID> Datastructures::all_places()
{
    return *(&alph_vec);
}

bool Datastructures::add_place(PlaceID id, const Name& name, PlaceType type, Coord xy)
{
    // fail check
    if (places.find(id) != places.end()) {
        return false;
    }
    // check if coordinate is occupied
    auto iter = occupied_coords.find(xy);
    if (iter != occupied_coords.end()) {
        (*iter).second.emplace_back(id);
    }
    else {
        occupied_coords.insert({xy, {id}});
    }

    // add to containers
    places.insert({id, {name, type, xy}});
    place_dist.insert(std::make_pair(xy, id));
    place_alph.insert(std::make_pair(name, id));

    // check if name is already in
    auto iter2 = place_names.find(name);
    if (iter2 != place_names.end()) {
        (*iter2).second.emplace_back(id);
    }
    else {
        place_names.insert({name, {id}});
    }

    // check if type is already in
    auto iter3 = place_types.find(type);
    if (iter3 != place_types.end()) {
        (*iter3).second.emplace_back(id);
    }
    else {
        place_types.insert({type, {id}});
    }

    alph_vec.emplace_back(id);
    dist_vec.emplace_back(id);
    alph_is_sorted = false;
    dist_is_sorted = false;
    return true;
}

Name Datastructures::get_place_name(PlaceID id)
{
    auto iter = places.find(id);
    if (iter != places.end()) {
        return (*iter).second.name;
    }
    // not found
    return NO_NAME;
}

PlaceType Datastructures::get_place_type(PlaceID id)
{
    auto iter = places.find(id);
    if (iter != places.end()) {
        return (*iter).second.type;
    }
    // not found
    return PlaceType::NO_TYPE;
}

Coord Datastructures::get_place_coord(PlaceID id)
{
    auto iter = places.find(id);
    if (iter != places.end()) {
        return (*iter).second.coord;
    }
    // not found
    return {};
}

std::vector<PlaceID> Datastructures::get_places_alphabetically()
{
    // sorted
    if (alph_is_sorted) {
        return alph_vec;
    }

    alph_vec.clear();
    for (auto&& item : place_alph) {
        alph_vec.push_back(item.second);
    }

    alph_is_sorted = true;
    return alph_vec;
}

std::vector<PlaceID> Datastructures::get_places_distance_increasing()
{
    // sorted
    if (dist_is_sorted) {
        return dist_vec;
    }

    dist_vec.clear();
    for (auto&& item : place_dist) {
        dist_vec.push_back(item.second);
    }

    dist_is_sorted = true;
    return dist_vec;
}

std::vector<PlaceID> Datastructures::find_places_with_coord(Coord xy)
{
    auto iter = occupied_coords.find(xy);
    if (iter != occupied_coords.end()) {
        return (*iter).second;
    }
    // not found
    return {};
}

std::vector<PlaceID> Datastructures::find_places_with_name(Name const& name)
{
    auto iter = place_names.find(name);
    if (iter != place_names.end()) {
        return (*iter).second;
    }
    // not found
    return {};
}

std::vector<PlaceID> Datastructures::find_places_with_type(PlaceType type)
{
    auto iter = place_types.find(type);
    if (iter != place_types.end()) {
        return (*iter).second;
    }
    // not found
    return {};
}


bool Datastructures::change_place_name(PlaceID id, const Name& newname)
{
    // not found
    auto iter = places.find(id);
    if (iter == places.end()) {
        return false;
    }
    // found
    // change in containers
    Name old = (*iter).second.name;
    std::vector<PlaceID> name_ids = place_names[old];
    auto pos = std::find(name_ids.begin(), name_ids.end(), id);
    if (pos != name_ids.end()) {
        name_ids.erase(pos);
        place_names[old] = name_ids;
    }
    place_names[newname].push_back(id);

    (*iter).second.name = newname;

    place_alph.erase({old, id});
    place_alph.insert(std::make_pair(newname, id));
    alph_is_sorted = false;
    return true;
}


bool Datastructures::change_place_coord(PlaceID id, Coord newcoord)
{
    // not found
    auto iter = places.find(id);
    if (iter == places.end()) {
        return false;
    }
    // found
    // change in containers
    Coord old = (*iter).second.coord;
    std::vector<PlaceID> coord_ids = occupied_coords[old];
    auto pos = std::find(coord_ids.begin(), coord_ids.end(), id);
    if (pos != coord_ids.end()) {
        coord_ids.erase(pos);
        occupied_coords[old] = coord_ids;
    }
    occupied_coords[newcoord].push_back(id);
    (*iter).second.coord = newcoord;

    place_dist.erase({old, id});
    place_dist.insert(std::make_pair(newcoord, id));
    dist_is_sorted = false;
    return true;
}


bool Datastructures::add_area(AreaID id, const Name& name, std::vector<Coord> coords)
{
    // check unique ID
    auto iter = areas.find(id);
    if (iter != areas.end()) {
        return false;
    }
    // add to containers
    areas.insert({id, {name, coords, NO_AREA, {}}});
    area_ids.push_back(id);
    return true;
}

std::vector<AreaID> Datastructures::all_areas()
{
    return *(&area_ids);
}

Name Datastructures::get_area_name(AreaID id)
{
    auto iter = areas.find(id);
    if (iter != areas.end()) {
        return (*iter).second.name;
    }
    // not found
    return NO_NAME;
}

std::vector<Coord> Datastructures::get_area_coords(AreaID id)
{
    auto iter = areas.find(id);
    if (iter != areas.end()) {
        return (*iter).second.coords;
    }
    // not found
    return {NO_COORD};
}

bool Datastructures::add_subarea_to_area(AreaID id, AreaID parentid)
{
    auto child = areas.find(id);
    auto parent = areas.find(parentid);
    // not found
    if ((child == areas.end()) ||
        (parent == areas.end())) {
        return false;
    }
    // parent check
    if ((*child).second.parent != NO_AREA) {
        return false;
    }

    // add data to parent
    std::vector<AreaID> children = (*parent).second.children;
    children.push_back(id);
    areas[parentid].children = children;

    //add data to child
    (*child).second.parent = parentid;

    return true;
}

std::vector<AreaID> Datastructures::ancestor_areas_of_subarea(AreaID id)
{
    std::vector<AreaID> ancestors = {};
    // no area
    auto iter = areas.find(id);
    if (iter == areas.end()) {
        ancestors.push_back(NO_AREA);
        return ancestors;
    }
    // no parent
    if ((*iter).second.parent == NO_AREA) {
        return ancestors;
    }

    // collect parents
    ancestors.push_back((*iter).second.parent);
    // find parents of contourid
    auto parent = areas.find((*iter).second.parent);
    while (parent != areas.end()) {
        if ((*parent).second.parent == NO_AREA) {
            break;
        }
        ancestors.push_back((*parent).second.parent);
        parent = areas.find((*parent).second.parent);
    }
    return ancestors;
}

std::vector<AreaID> Datastructures::all_subareas_of_area(AreaID /*id*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("all_subareas_of_area");
}

std::vector<PlaceID> Datastructures::get_places_closest_to(Coord /*xy*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("get_places_closest_to");
}

bool Datastructures::remove_place(PlaceID /*id*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("remove_place");
}

AreaID Datastructures::get_closest_common_ancestor_of_areas(AreaID /*id1*/, AreaID /*id2*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("get_closest_common_ancestor_of_areas");
}


