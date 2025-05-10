#pragma once

#include <cstdint>

class String
{
public:
	String();
	String(const char* string);

	size_t Capacity() const;

	size_t Size() const;

	const char* CStr() const;

private:
	void Debug() const;

private:
	static constexpr size_t SSO_CAPACITY = 15;

	union
	{
		struct
		{
			char* ptr;
			size_t size;
			size_t capacity;
		} heap;

		struct
		{
			char buffer[SSO_CAPACITY + 1];
			uint8_t size;
		} stack;
	};

	bool isSSO;
	
};

