#include "EntityCollection.h"

EntityCollection::EntityCollection() : last_id{ 0 }, entity_ptrs{}
{ }

EntityCollection::~EntityCollection()
{
}

void EntityCollection::add(std::shared_ptr<Entity> entity_ptr)
{
	entity_ptr->id = get_unique_id();
	entity_ptr->on_create();
	entity_ptrs.push_back(entity_ptr);
}

void EntityCollection::add(Entity* entity_ptr)
{
	add(std::shared_ptr<Entity>(entity_ptr));
}

std::vector<std::shared_ptr<Entity>>::iterator EntityCollection::begin()
{
	return entity_ptrs.begin();
}

std::vector<std::shared_ptr<Entity>>::iterator EntityCollection::end()
{
	return entity_ptrs.end();
}

std::shared_ptr<Entity> EntityCollection::operator[](size_t i) { return entity_ptrs[i]; }

size_t EntityCollection::count() const { return entity_ptrs.size(); }

entity_id_t EntityCollection::get_unique_id() { return ++last_id; }