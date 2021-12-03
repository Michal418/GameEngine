#pragma once
#include <functional>
#include "EntityCollection.h"
#include "Math.h"
#include "Physics.h"
#include "Camera.h"
#include "../Tilemap.h"

struct ID2D1Bitmap;

class Scene
{
public:
	Scene(uint16_t width, uint16_t height, uint16_t camera_scale = 1.0f);
	Scene(vec2 world_size, vec2 screen_size);
	~Scene();

public:
	void init();

	void update(float delta_time);
	bool try_find_entity(entity_id_t, Entity&);

	template<typename T>
	inline void add_entity()
	{
		static_assert(std::is_base_of<Entity, T>::value,
			"T nebyl Entity");

		T* entity_ptr = new T();
		entity_ptr->frame_ptr = m_frame_ptr;
		m_entity_collection_ptr->add(entity_ptr);
	}

	template<typename T>
	inline void add_entity(std::function<void(T&)> setup)
	{
		static_assert(std::is_base_of<Entity, T>::value,
			"T nebyl Entity");

		T* entity_ptr = new T();
		entity_ptr->frame_ptr = m_frame_ptr;
		setup(*entity_ptr);
		m_entity_collection_ptr->add(entity_ptr);
	}

	template<typename T>
	inline void add_entity(T entity)
	{
		T* entity_ptr = new T();
		*entity_ptr = entity;
		entity_ptr->frame_ptr = m_frame_ptr;
		m_entity_collection_ptr->add(entity_ptr);
	}

public:
	EntityCollection* m_entity_collection_ptr;
	Frame* m_frame_ptr;
	Tilemap tilemap;
	Camera camera;
	bool m_colliding = false;
	float air_resistance = .1f;
};