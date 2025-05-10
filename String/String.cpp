#include "String.h"

#include <cstring>
#include <iostream>

String::String()
	: isSSO(true)
{
	std::memset(stack.buffer, 0, sizeof(stack.buffer));
	stack.size = 0;
	Debug();
}

String::String(const char* string)
{
	size_t length = std::strlen(string);

	if (length < SSO_CAPACITY)
	{
		std::memcpy(stack.buffer, string, length);
		stack.buffer[length] = '\0';
		stack.size = length;
	}
	else
	{
		heap.ptr = new char[SSO_CAPACITY * 2 + 1];
		std::memcpy(heap.ptr, string, length);
		heap.ptr[length] = '\0';
		heap.size = length;
		heap.capacity = SSO_CAPACITY * 2;
		isSSO = false;
	}

	Debug();
}

size_t String::Capacity() const
{
	return isSSO ? SSO_CAPACITY : heap.capacity;
}

size_t String::Size() const
{
	return isSSO ? stack.size : heap.size;
}

const char* String::CStr() const
{
	return isSSO ? stack.buffer : heap.ptr;
}

void String::Debug() const
{
	std::cout << "Contents: " << CStr() << '\n' << "Size: " << Size() << '\n' << "Capacity: " << Capacity() << '\n' << "Storage: " << (isSSO ? "Stack" : "Heap" ) << "\n\n";
}
