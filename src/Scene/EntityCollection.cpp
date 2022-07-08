#include "EntityCollection.hpp"


#include <iostream>


EntityCollection::EntityCollection() : last_id{ 0 }, entity_ptrs{}
{
	std::cout << "EntityCollection::EntityCollection\n";
}

EntityCollection::~EntityCollection()
{
	std::cout << "EntityCollection::~EntityCollection\n";
}

void EntityCollection::add(std::shared_ptr<Entity> entity_ptr)
{
	std::cout << "EntityCollection::add\n";

	entity_ptr->id = get_unique_id();
	entity_ptr->on_create();
	entity_ptrs.push_back(entity_ptr);
}

void EntityCollection::add(Entity* entity_ptr)
{
	std::cout << "EntityCollection::add\n";

	add(std::shared_ptr<Entity>(entity_ptr));
}

std::vector<std::shared_ptr<Entity>>::iterator EntityCollection::begin()
{
	std::cout << "EntityCollection::begin\n";

	return entity_ptrs.begin();
}

std::vector<std::shared_ptr<Entity>>::iterator EntityCollection::end()
{
	std::cout << "EntityCollection::end\n";

	return entity_ptrs.end();
}

std::shared_ptr<Entity> EntityCollection::operator[](size_t i) { return entity_ptrs[i]; std::cout << "EntityCollection::operator[]\n"; }

size_t EntityCollection::count() const { return entity_ptrs.size(); std::cout << "EntityCollection::count\n"; }

entity_id_t EntityCollection::get_unique_id() { return ++last_id; std::cout << "EntityCollection::get_unique_id\n"; }