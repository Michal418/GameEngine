#pragma once
#include "EntityCollection.h"
#include "Math.h"
#include "Physics.h"
#include "Camera.h"

struct ID2D1Bitmap;

class Scene
{
public:
	Scene();
	~Scene();

public:
	void init();

	template<typename T>
	void add_entity()
	{
		static_assert(std::is_base_of<Entity, T>::value,
			"T nebyl Entity");

		T* entity_ptr = new T();
		entity_ptr->frame_ptr = m_frame_ptr;
		m_entity_collection_ptr->Add(entity_ptr);
	}

	template<typename T>
	void add_entity(std::function<void(T&)> setup)
	{
		static_assert(std::is_base_of<Entity, T>::value,
			"T nebyl Entity");

		T* entity_ptr = new T();
		entity_ptr->frame_ptr = m_frame_ptr;
		setup(*entity_ptr);
		m_entity_collection_ptr->Add(entity_ptr);
	}

	void update(float delta_time);
	bool try_find_entity(entity_id_t, Entity&);

public:
	EntityCollection* m_entity_collection_ptr;
	Frame* m_frame_ptr;
	std::vector<entity_id_t> m_resource_ids;
	bool m_colliding = false;
	Camera camera{};
	float air_resistance = .1f;
};