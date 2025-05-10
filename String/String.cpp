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
		std::memset(string.stack.buffer, 0, sizeof(string.stack.buffer));
	}

	Debug();

	return *this;
}

String& String::operator+=(const char* string)
{
	Append(string);
	return *this;
}

String& String::operator+=(const String& string)
{
	Append(string);
	return *this;
}

String String::operator+(const char* string)
{
	String result(*this);
	result += string;
	return result;
}

String String::operator+(const String& string)
{
	String result(*this);
	result += string;
	return result;
}

void String::Clear()
{
	if (isSSO)
	{
		std::memset(stack.buffer, 0, sizeof(stack.buffer));
		stack.size = 0;
	}
	else
	{
		std::memset(heap.ptr, 0, heap.capacity);
		heap.size = 0;
	}
}

String& String::Append(const char* string)
{
	size_t length = std::strlen(string);

	// Check if stack allocated
	if (isSSO)
	{
		size_t newLength = stack.size + length;

		// Smaller than stack capacity
		if (newLength < SSO_CAPACITY)
		{
			std::memcpy(stack.buffer + stack.size, string, length);
			stack.buffer[newLength] = '\0';
			stack.size = newLength;
		}
		// Length bigger than stack capacity
		else
		{
			char* newData = new char[newLength + 1];
			std::memcpy(newData, stack.buffer, stack.size);
			std::memcpy(newData + stack.size, string, length);
			newData[newLength] = '\0';

			isSSO = false;
			heap.ptr = newData;
			heap.size = newLength;
			heap.capacity = newLength;
		}
	}
	// Heap based allocated
	else
	{
		size_t newLength = heap.size + length;

		// Allocate more memory if larger than heap capacity
		if(newLength > heap.capacity)
		{
			char* newData = new char[newLength + 1];
			std::memcpy(newData, heap.ptr, heap.size);
			std::memcpy(newData + heap.size, string, length);
			newData[newLength] = '\0';

			heap.ptr = newData;
			heap.ptr = newData;
			heap.size = newLength;
			heap.capacity = newLength;
		}
		// No need to allocate
		else
		{
			std::memcpy(heap.ptr + heap.size, string, length);
			heap.ptr[newLength] = '\0';
			heap.size = newLength;
		}
	}

	return *this;
}

String& String::Append(const String& string)
{
	size_t length = string.Size();

	// Check if incoming string is stack allocated
	if (string.isSSO)
	{
		// Check if string is stack allocated
		if (isSSO)
		{
			size_t newLength = stack.size + length;

			// Length smaller than stack capacity
			if (newLength < SSO_CAPACITY)
			{
				std::memcpy(stack.buffer + stack.size, string.stack.buffer, length);
				stack.buffer[length] = '\0';
				stack.size += length;
			}
			// Length larger than stack capacity
			else
			{
				char* newData = new char[newLength + 1];
				std::memcpy(newData, stack.buffer, stack.size);
				std::memcpy(newData + stack.size, string.stack.buffer, length);
				newData[newLength] = '\0';

				isSSO = false;
				heap.ptr = newData;
				heap.size = newLength;
				heap.capacity = newLength;
			}
		}
		// String is using heap allocated
		else
		{
			size_t newLength = heap.size + length;

			// Allocate more memory if new length is bigger than capacity
			if (newLength > heap.capacity)
			{
				char* newData = new char[newLength + 1];
				std::memcpy(newData, stack.buffer, stack.size);
				std::memcpy(newData + stack.size, string.stack.buffer, length);
				newData[newLength] = '\0';

				heap.ptr = newData;
				heap.size = newLength;
				heap.capacity = newLength;
			}
			// No allocation
			else
			{
				std::memcpy(heap.ptr + heap.size, string.stack.buffer, length);
				heap.ptr[newLength] = '\0';
				heap.size = newLength;
			}
		}
	}
	// Incoming string is heap allocated
	else
	{
		// String is stack allocated
		if (isSSO)
		{
			size_t newLength = stack.size + length;

			char* newData = new char[newLength + 1];
			std::memcpy(newData, stack.buffer, stack.size);
			std::memcpy(newData + stack.size, string.heap.ptr, length);
			newData[newLength] = '\0';

			isSSO = false;
			heap.ptr = newData;
			heap.size = newLength;
			heap.capacity = newLength;
		}
		// String is heap allocated
		else
		{
			size_t newLength = heap.size + length;

			// Allocate more memory is new length is bigger than capacity
			if (newLength > heap.capacity)
			{
				char* newData = new char[newLength + 1];
				std::memcpy(newData, heap.ptr, heap.size);
				std::memcpy(newData + heap.size, string.heap.ptr, length);
				newData[newLength] = '\0';

				heap.ptr = newData;
				heap.size = newLength;
				heap.capacity = newLength;
			}
			// No allocation
			else
			{
				std::memcpy(heap.ptr + heap.size, string.stack.buffer, stack.size);
				heap.ptr[newLength] = '\0';
				heap.size = newLength;
			}
		}
	}

	return *this;
}

String& String::Insert(size_t index, const char* string)
{
	if (index > Size())
	{
		throw std::out_of_range("Insert index is out of range.");
	}

	size_t length = std::strlen(string);

	// String is stack allocated
	if (isSSO)
	{
		size_t newLength = stack.size + length;

		// Length smaller than stack capacity
		if (newLength < SSO_CAPACITY)
		{
			std::memmove(stack.buffer + index + length, stack.buffer + index, stack.size - index + 1);
			std::memcpy(stack.buffer + index, string, length);
			stack.buffer[newLength] = '\0';
			stack.size = newLength;
		}
		// Length bigger than stack capacity
		else
		{
			isSSO = false;
			char* newData = new char[newLength + 1];
			std::memcpy(newData, stack.buffer, stack.size);
			std::memmove(newData + index + length, newData + index, stack.size - index + 1);
			std::memcpy(newData + index, string, length);
			newData[newLength] = '\0';

			heap.ptr = newData;
			heap.size = newLength;
			heap.capacity = newLength;
		}
	}
	// String is heap allocated
	else
	{
		size_t newLength = heap.size + length;

		// Allocate more memory
		if (newLength > heap.capacity)
		{
			char* newData = new char[newLength + 1];
			std::memcpy(newData, stack.buffer, stack.size);
			std::memmove(newData + index + length, newData + index, stack.size - index + 1);
			std::memcpy(newData + index, string, length);
			newData[newLength] = '\0';

			heap.ptr = newData;
			heap.size = newLength;
			heap.capacity = newLength;
		}
		// No Allocation
		else
		{
			std::memmove(heap.ptr + index + length, heap.ptr + index, heap.size - index + 1);
			std::memcpy(heap.ptr + index, string, length);
			heap.ptr[newLength] = '\0';
			heap.size = newLength;
		}
	}

	return *this;
}

String& String::Insert(size_t index, const String& string)
{
	if (index > Size())
	{
		throw std::out_of_range("Insert index is out of range.");
	}

	size_t length = string.Size();

	// Incoming string is stack allocated
	if (string.isSSO)
	{
		// String is stack allocated
		if(isSSO)
		{
			size_t newLength = stack.size + length;

			// Length smaller than stack capacity
			if (newLength < SSO_CAPACITY)
			{
				std::memmove(stack.buffer + index + length, stack.buffer + index, stack.size - index + 1);
				std::memcpy(stack.buffer + index, string.stack.buffer, length);
				stack.buffer[newLength] = '\0';
				stack.size = newLength;
			}
			// Length bigger than stack capacity
			else
			{
				isSSO = false;
				char* newData = new char[newLength + 1];
				std::memcpy(newData, stack.buffer, stack.size);
				std::memmove(newData + index + length, newData + index, stack.size - index + 1);
				std::memcpy(newData + index, string.stack.buffer, length);
				newData[newLength] = '\0';

				heap.ptr = newData;
				heap.size = newLength;
				heap.capacity = newLength;
			}
		}
		// String is heap allocated
		else
		{
			size_t newLength = heap.size + length;

			// Allocate more memory
			if (newLength > heap.capacity)
			{
				char* newData = new char[newLength + 1];
				std::memcpy(newData, heap.ptr, heap.size);
				std::memmove(newData + index + length, newData + index, heap.size - index + 1);
				std::memcpy(newData + index, string.stack.buffer, length);
				newData[newLength] = '\0';

				heap.ptr = newData;
				heap.size = newLength;
				heap.capacity = newLength;
			}
			// No allocation
			else
			{
				std::memmove(heap.ptr + index + length, heap.ptr + index, heap.size - index + 1);
				std::memcpy(heap.ptr + index, string.stack.buffer, length);
				heap.ptr[newLength] = '\0';
				heap.size = newLength;
			}
		}
	}
	// Incoming String is heap allocated
	else
	{
		// String is stack allocated
		if(isSSO)
		{
			size_t newLength = stack.size + length;

			isSSO = false;
			char* newData = new char[newLength + 1];
			std::memcpy(newData, stack.buffer, stack.size);
			std::memmove(newData + index + length, newData + index, stack.size - index + 1);
			std::memcpy(newData + index, string.heap.ptr, length);
			newData[newLength] = '\0';

			heap.ptr = newData;
			heap.size = newLength;
			heap.capacity = newLength;
		}
		// String is heap allocated
		else
		{
			size_t newLength = heap.size + length;

			// Allocate more memory
			if (newLength > heap.capacity)
			{
				char* newData = new char[newLength + 1];
				std::memcpy(newData, heap.ptr, heap.size);
				std::memmove(newData + index + length, newData + index, heap.size - index + 1);
				std::memcpy(newData + index, string.heap.ptr, length);
				newData[newLength] = '\0';

				heap.ptr = newData;
				heap.size = newLength;
				heap.capacity = newLength;
			}
			// No Allocation
			else
			{
				std::memmove(heap.ptr + index + length, heap.ptr + index, heap.size - index + 1);
				std::memcpy(heap.ptr + index, string.heap.ptr, length);
				heap.ptr[newLength] = '\0';
				heap.size = newLength;
			}
		}
	}

	return *this;
}

String& String::Erase(size_t index, size_t count)
{
	// TODO: Implement function
	return *this;
}

bool String::Empty() const
{
	return isSSO ? stack.size == 0 : heap.size == 0;
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
	std::cout << "Contents: " << CStr() << '\n' << "Size: " << Size() << '\n' << "Capacity: " << Capacity() << '\n' << "allocated: " << (isSSO ? "Stack" : "Heap" ) << "\n\n";
}
