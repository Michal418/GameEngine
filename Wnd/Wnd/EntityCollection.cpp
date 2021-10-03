#include "EntityCollection.h"

EntityCollection::EntityCollection() : last_id{ 0 }, entity_ptrs{}
{ }

EntityCollection::~EntityCollection()
{
	for (Entity* entity : entity_ptrs)
		delete entity;
}

void EntityCollection::Add(Entity* entity_ptr)
{
	entity_ptr->id = get_unique_id();
	entity_ptr->on_create();
	entity_ptrs.push_back(entity_ptr);
}

std::vector<Entity*>::iterator EntityCollection::begin()
{
	return entity_ptrs.begin();
}

std::vector<Entity*>::iterator EntityCollection::end()
{
	return entity_ptrs.end();
}

Entity* EntityCollection::operator[](size_t i) { return entity_ptrs[i]; }

size_t EntityCollection::count() const { return entity_ptrs.size(); }

entity_id_t EntityCollection::get_unique_id() { return ++last_id; }