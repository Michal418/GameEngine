#pragma once
#include <Windows.h>
#include <vector>

#include "Renderer.h"
#include "Scene.h"
#include "Input.h"

class Game
{
public:
	Game();
	~Game();

public:
	void run();

protected:
	virtual void on_create() { };
	virtual void on_start() { };
	virtual void on_update(float delta_time) { };

	void close();

protected:
	Renderer* m_renderer_ptr; // vlastník je Game
	Scene* m_scene_ptr; // vlastník je Game

private:
	void create();
	void start();
	void update(float delta_time);

private:
	bool running = true;
};
