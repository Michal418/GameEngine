#include "Game.h"
#include "../Wnd/Graphics/Window.h"


Game::Game(LPCWSTR name = L"default")
	: m_scene(320, 224, 4u)
{
	this->name = name;
	create();
}
Game::~Game()
{
	Input::s_instance->deinit();
}

void Game::create()
{
	Input::init(m_scene.camera);
	m_window_ptr = std::make_shared<Window>(*this, m_scene);

	on_create();
}

void Game::start()
{
	on_start();


}

int64_t Game::get_milliseconds_since_epoch() const
{
	using namespace std::chrono;

	auto now = system_clock::now();
	auto time = now.time_since_epoch();
	auto millis = duration_cast<milliseconds>(time);

	return millis.count();
}

void Game::update(float delta_time)
{
	on_update(delta_time);
	m_window_ptr->invalidate_surface();
	m_scene.update(delta_time);
	Input::s_instance->update(delta_time);
}

void Game::close()
{
	running = false;
}

void Game::reset_timer()
{
	last_update_time = get_milliseconds_since_epoch();
}

void Game::run()
{
	start();

	m_window_ptr->init(name);

	int64_t delta_time = 0;
	reset_timer();

	uint64_t start_time = get_milliseconds_since_epoch();
	uint64_t frames = 0;
	float delta_sum = 0;

	HANDLE_IF_FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED));

	while (m_window_ptr->process_messages())
	{
		delta_time = get_milliseconds_since_epoch() - last_update_time;

		if (delta_time > 0)
		{
			last_update_time = get_milliseconds_since_epoch();
			update(delta_time / 1000.0f);

			frames++;
			delta_sum += delta_time / 1000.0f;
		}
	}

	CoUninitialize();

	float elapsed_time = (float)(get_milliseconds_since_epoch() - start_time);
	elapsed_time /= 1000.0f;
	float fps = static_cast<float>(frames) / elapsed_time;

#ifdef LOGFPS
#define W(s) std::to_wstring(s)

	MessageBox(
		NULL,
		(L"frames:\t\t" + W(frames) + L'\n' +
		L"time:\t\t" + W(elapsed_time) + L"\n"
		L"delta sum\t" + W(delta_sum) + L"\n" +
		L"avg. delta\t" + W(delta_sum / frames) + L'\n' +
		L"FPS:\t\t" + W(fps) + L'\n').c_str(),
		L"FPS",
		MB_OK
	);
#endif
}