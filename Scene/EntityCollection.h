#pragma once
#include <vector>
#include <memory>

#include "../Scene/Entity.h"
#include "../Vector2.h"

class EntityCollection
{
public:
	EntityCollection();
	~EntityCollection();

	void add(std::shared_ptr<Entity> entity_ptr);
	void add(Entity* entity_ptr);
	size_t count() const;

	std::shared_ptr<Entity> operator[](size_t i);

	std::vector<std::shared_ptr<Entity>>::iterator begin();
	std::vector<std::shared_ptr<Entity>>::iterator end();
private:
	std::vector<std::shared_ptr<Entity>> entity_ptrs;
	entity_id_t last_id;
	inline entity_id_t get_unique_id();
};