#include "Logger.hpp"

void(*Logger::on_error)(HRESULT, std::string) = Logger::default_handler;
void(*Logger::on_alert)(HRESULT, std::string) = Logger::default_alerter;

void Logger::show(HRESULT hr, std::string at, std::wstring title, UINT flags) noexcept
{
	_com_error err(hr);

	std::wstring str =
		L"[Error code] " + std::to_wstring(err.Error()) + L"\n" +
		L"[Message] " + err.ErrorMessage() + L"\n" +
		L"[At] " + std::wstring(at.begin(), at.end());

	MessageBox(NULL, str.c_str(), title.c_str(), flags);
}

void Logger::show_text(std::string str)
{
	MessageBoxA(NULL, str.c_str(), "Info", MB_OK);
}

void Logger::show_text(std::wstring str)
{
	MessageBox(NULL, str.c_str(), L"Info", MB_OK);
}

void Logger::default_alerter(HRESULT hr, std::string at)
{
	show(hr, at, L"Warning", MB_OK | MB_ICONWARNING);
}

void Logger::default_handler(HRESULT hr, std::string at) noexcept
{
	show(hr, at, L"Error", MB_OK | MB_ICONERROR);
	exit(-1);
}
