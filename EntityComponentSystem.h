#pragma once
#include <vector>
#include "Scene/Entity.h"
#include <memory>

class EntityComponentSystem
{
public:
	static std::shared_ptr<EntityComponentSystem> s_instance;

	void push_entity(Entity& e);

	template<class T>
	T* get_component(entity_id_t);

private:
	std::vector<Rigidbody> rigidbodys;
	std::vector<Transform> transforms;
	std::vector<Collider> dynamic_colliders;
	std::vector<Collider> static_colliders;
	std::vector<Sprite> sprites;
	std::vector<entity_id_t> ids;
};

