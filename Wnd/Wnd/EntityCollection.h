#pragma once
#include <vector>

#include "Entity.h"
#include "Vector2.h"

class EntityCollection
{
public:
	EntityCollection();
	~EntityCollection();

	void Add(Entity* entity_ptr);
	size_t count() const;

	Entity* operator[](size_t i);

	std::vector<Entity*>::iterator begin();
	std::vector<Entity*>::iterator end();
private:
	std::vector<Entity*> entity_ptrs;
	entity_id_t last_id;
	inline entity_id_t get_unique_id();
};