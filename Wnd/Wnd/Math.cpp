#include "Math.h"

__int64 math::hash_string(const char* str)
{
	__int64 hash = 5381;

	while (int c = *str++)
		hash = ((hash << 5) + hash) + c;

	return hash;
}

__int64 math::hash_string(const wchar_t* str)
{
	__int64 hash = 5381;

	while (int c = *str++)
		hash = ((hash << 5) + hash) + c;

	return hash;
}