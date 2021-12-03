#pragma once
#include <windows.h>
#include <comdef.h>
#include <memory>
#include <string>

#define HANDLE_IF_FAILED(hr) if (FAILED(hr)) Logger::on_error(hr, "line " + std::to_string(__LINE__) + " in " + __FILE__);
#define ALERT_IF_FAILED(hr) if (FAILED(hr)) Logger::on_alert(hr, "line " + std::to_string(__LINE__) + " in " + __FILE__);
#define SHOW_INFO(hr) Logger::show(hr, std::to_string(__LINE__) + " in " + __FILE__);
class Logger
{
public:
	static void(*on_error)(HRESULT, std::string);
	static void(*on_alert)(HRESULT, std::string);
	static void show(HRESULT, std::string at, std::wstring title = L"Neošetøená výjimka", UINT flags = 0u) noexcept;
	static void show_text(std::string str);
	static void show_text(std::wstring str);
	static void default_handler(HRESULT hr, std::string file) noexcept;
	static void default_alerter(HRESULT hr, std::string file);
};

