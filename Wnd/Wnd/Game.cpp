#include <comdef.h>
#include <chrono>
#include <ostream>
#include <string>
#include "Game.h"
#include <iostream>

Game::Game()
{
	Input::init();
	m_scene_ptr = new Scene();
	m_renderer_ptr = new Renderer(m_scene_ptr);

	create();
}

Game::~Game()
{
	Input::s_instance->deinit();
	delete m_renderer_ptr;
	delete m_scene_ptr;
}

void Game::create()
{
	on_create();
}

void Game::start()
{
	on_start();
}

void Game::update(float delta_time)
{
	Input::s_instance->update(delta_time);
	on_update(delta_time);
	m_scene_ptr->update(delta_time);
}

void Game::close()
{
	running = false;
}

void Game::run()
{
	start();

	m_renderer_ptr->init(1920u, 1080u);

	std::function<int64_t(void)> get_milliseconds_since_epoch = []()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now()
				.time_since_epoch()
			).count();
	};

	int64_t last_update_time = get_milliseconds_since_epoch();
	HRESULT hr{};
	int64_t delta_time = 0;

	while (running && m_renderer_ptr->process_messages(hr))
	{
		if (hr != 2)
			delta_time = get_milliseconds_since_epoch() - last_update_time;

		if (delta_time > 16)
		{
			update(delta_time / 1000.0f);
			m_renderer_ptr->invalidate_surface();
			last_update_time = get_milliseconds_since_epoch();
		}
	}

	if (FAILED(hr))
	{
		_com_error err(hr);
		LPCTSTR err_msg = err.ErrorMessage();
		m_renderer_ptr->show_message(err_msg);
	}

	//CoUninitialize();
}