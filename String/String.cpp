#include "String.h"

#include <cstring>
#include <iostream>

//Default construct with stack buffer set to all zeros
String::String()
	: isSSO(true)
{
	std::memset(stack.buffer, 0, sizeof(stack.buffer));
	stack.size = 0;
	Debug();
}

//Construct from CString
//If length is less than SSO_CAPACITY string will be stack allocated
//If length is greater than SSO_CAPACITY string will be heap allocated
String::String(const char* string)
{
	size_t length = std::strlen(string);

	if (length < SSO_CAPACITY)
	{
		isSSO = true;
		std::memcpy(stack.buffer, string, length);
		stack.buffer[length] = '\0';
		stack.size = length;
	}
	else
	{
		isSSO = false;
		heap.ptr = new char[SSO_CAPACITY * 2 + 1];
		std::memcpy(heap.ptr, string, length);
		heap.ptr[length] = '\0';
		heap.size = length;
		heap.capacity = SSO_CAPACITY * 2;
	}

	Debug();
}

//Copy constructor
String::String(const String& string)
{
	size_t length = string.Size();

	if (string.isSSO)
	{
		isSSO = true;
		std::memcpy(stack.buffer, string.stack.buffer, length);
		stack.buffer[length] = '\0';
		stack.size = length;
	}
	else
	{
		isSSO = false;
		heap.ptr = new char[length + 1];
		std::memcpy(heap.ptr, string.heap.ptr, length);
		heap.ptr[length] = '\0';
		heap.size = length;
		heap.capacity = length;
	}

	Debug();
}

//Move constructor
//Will leave other string in a stack valid state
String::String(String&& string) noexcept
{
	size_t length = string.Size();

	if (string.isSSO)
	{
		isSSO = true;
		std::memcpy(stack.buffer, string.stack.buffer, length);
		stack.buffer[length] = '\0';
		stack.size = length;

		std::memset(string.stack.buffer, 0, sizeof(string.stack.buffer));
		string.stack.size = 0;
	}
	else
	{
		isSSO = false;
		heap.ptr = string.heap.ptr;
		heap.size = string.heap.size;
		heap.capacity = string.heap.capacity;

		string.isSSO = true;
		string.heap.ptr = nullptr;
		string.heap.size = 0;
		string.heap.capacity = SSO_CAPACITY;
		std::memset(string.stack.buffer, 0, sizeof(string.stack.buffer));
	}

	Debug();
}

String::~String()
{
	if (!isSSO)
	{
		delete[] heap.ptr;
	}
}

String& String::operator=(const String& string)
{
	if (this == &string) return *this;

	if (!isSSO)
	{
		delete[] heap.ptr;
	}

	size_t length = string.Size();

	if (string.isSSO)
	{
		isSSO = true;
		std::memcpy(stack.buffer, string.stack.buffer, length);
		stack.buffer[length] = '\0';
		stack.size = length;
	}
	else
	{
		isSSO = false;
		heap.ptr = new char[length + 1];
		std::memcpy(heap.ptr, string.heap.ptr, length);
		heap.ptr[length] = '\0';
		heap.size = length;
		heap.capacity = length;
	}

	Debug();

	return *this;
}

String& String::operator=(String&& string) noexcept
{
	if (this == &string) return *this;

	if (!isSSO)
	{
		delete[] heap.ptr;
	}

	size_t length = string.Size();

	if (string.isSSO)
	{
		isSSO = true;
		std::memcpy(stack.buffer, string.stack.buffer, length);
		stack.buffer[length] = '\0';
		stack.size = length;

		std::memset(string.stack.buffer, 0, sizeof(string.stack.buffer));
		string.stack.size = 0;
	}
	else
	{
		isSSO = false;
		heap.ptr = string.heap.ptr;
		heap.size = string.heap.size;
		heap.capacity = string.heap.capacity;

		string.isSSO = true;
		string.heap.ptr = nullptr;
		string.heap.size = 0;
		string.heap.capacity = SSO_CAPACITY;
		std::memset(string.stack.buffer, 0, sizeof(string.stack.buffer));
	}

	Debug();

	return *this;
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
