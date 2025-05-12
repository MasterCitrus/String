#include "CString.h"

#include <cstring>
#include <iostream>

//Default construct with stack buffer set to all zeros
String::String()
	: isSSO(true)
{
	std::memset(stack.buffer, 0, sizeof(stack.buffer));
	stack.size = 0;
}

//Construct from CString
//If length is less than SSO_CAPACITY string will be stack allocated
//If length is greater than SSO_CAPACITY string will be heap allocated
String::String(const char* string)
	: String()
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
		Allocate(length, SSO_CAPACITY);

		std::memcpy(heap.ptr, string, length);
		heap.ptr[length] = '\0';
		heap.size = length;
	}
}

//Copy constructor
String::String(const String& string)
	: String()
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
		Allocate(length, string.Capacity());

		std::memcpy(heap.ptr, string.heap.ptr, length);
		heap.ptr[length] = '\0';
		heap.size = length;
	}
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
}

String::~String()
{
	if (!isSSO)
	{
		delete[] heap.ptr;
	}
}

String& String::operator=( const char* string )
{
	size_t length = std::strlen( string );

	if(isSSO)
	{
		if (length < SSO_CAPACITY)
		{
			isSSO = true;
			std::memcpy( stack.buffer, string, length );
			stack.buffer[length] = '\0';
			stack.size = length;
		}
		else
		{
			Allocate(length, SSO_CAPACITY);

			std::memcpy( heap.ptr, string, length );
			heap.ptr[length] = '\0';
			heap.size = length;
		}
	}
	else
	{
		if (length < SSO_CAPACITY)
		{
			isSSO = true;
			std::memcpy( stack.buffer, string, length );
			stack.buffer[length] = '\0';
			stack.size = length;
		}
		else
		{
			Allocate(length, SSO_CAPACITY);

			std::memcpy( heap.ptr, string, length );
			heap.ptr[length] = '\0';
			heap.size = length;
		}
	}

	return *this;
}

String& String::operator=(const String& string)
{
	if (this == &string) return *this;

	size_t length = string.Size();

	if (string.isSSO)
	{
		if (isSSO)
		{
			std::memcpy(stack.buffer, string.stack.buffer, length);
			stack.buffer[length] = '\0';
			stack.size = length;
		}
		else
		{
			delete[] heap.ptr;

			isSSO = true;
			std::memcpy( stack.buffer, string.stack.buffer, length );
			stack.buffer[length] = '\0';
			stack.size = length;
		}
	}
	else
	{
		if (isSSO)
		{
			Allocate(length, SSO_CAPACITY);

			std::memcpy( heap.ptr, string.heap.ptr, length );
			heap.ptr[length] = '\0';
			heap.size = length;
		}
		else
		{
			Allocate(length, string.Capacity());

			std::memcpy(heap.ptr, string.heap.ptr, length);
			heap.ptr[length] = '\0';
			heap.size = length;
		}
	}

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
	String result = *this;
	result += string;
	return result;
}

String String::operator+(const String& string)
{
	String result = *this;
	result += string;
	return result;
}

char& String::operator[](size_t pos)
{
	return *( isSSO ? stack.buffer + pos : heap.ptr + pos );
}

const char& String::operator[](size_t pos) const
{
	return *( isSSO ? stack.buffer + pos : heap.ptr + pos );
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
			Allocate(newLength, SSO_CAPACITY);

			std::memcpy(heap.ptr + heap.size, string, length);
			heap.ptr[newLength] = '\0';
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
			Allocate(newLength, heap.capacity);

			std::memcpy(heap.ptr + heap.size, string, length);
			heap.ptr[newLength] = '\0';
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
				stack.buffer[newLength] = '\0';
				stack.size = newLength;
			}
			// Length larger than stack capacity
			else
			{
				Allocate(newLength, SSO_CAPACITY);

				std::memcpy(heap.ptr + heap.size, string.stack.buffer, length);
				heap.ptr[newLength] = '\0';
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
				Allocate(newLength, heap.capacity);

				std::memcpy(heap.ptr + heap.size, string.stack.buffer, length);
				heap.ptr[newLength] = '\0';
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

			Allocate(newLength, SSO_CAPACITY);

			std::memcpy(heap.ptr + heap.size, string.heap.ptr, length);
			heap.ptr[newLength] = '\0';
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
				Allocate(newLength, heap.capacity);

				std::memcpy(heap.ptr + heap.size, string.heap.ptr, length);
				heap.ptr[newLength] = '\0';
				heap.size = newLength;
				heap.capacity = newLength;
			}
			// No allocation
			else
			{
				std::memcpy(heap.ptr + heap.size, string.heap.ptr, length);
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
			Allocate(newLength, SSO_CAPACITY);

			std::memmove(heap.ptr + index + length, heap.ptr + index, heap.size - index + 1);
			std::memcpy(heap.ptr + index, string, length);
			heap.ptr[newLength] = '\0';
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
			Allocate(newLength, heap.capacity);

			std::memmove(heap.ptr + index + length, heap.ptr+ index, stack.size - index + 1);
			std::memcpy(heap.ptr + index, string, length);
			heap.ptr[newLength] = '\0';
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
				Allocate(newLength, SSO_CAPACITY);

				std::memmove(heap.ptr + index + length, heap.ptr + index, heap.size - index + 1);
				std::memcpy(heap.ptr + index, string.stack.buffer, length);
				heap.ptr[newLength] = '\0';
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
				Allocate(newLength, heap.capacity);

				std::memmove(heap.ptr + index + length, heap.ptr + index, heap.size - index + 1);
				std::memcpy(heap.ptr + index, string.stack.buffer, length);
				heap.ptr[newLength] = '\0';
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

			Allocate(newLength, SSO_CAPACITY);

			std::memmove(heap.ptr + index + length, heap.ptr + index, heap.size - index + 1);
			std::memcpy(heap.ptr + index, string.heap.ptr, length);
			heap.ptr[newLength] = '\0';
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
				Allocate(newLength, heap.capacity);

				std::memmove(heap.ptr + index + length, heap.ptr + index, heap.size - index + 1);
				std::memcpy(heap.ptr + index, string.heap.ptr, length);
				heap.ptr[newLength] = '\0';
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
	if (count == -1) count = stack.size;

	if (isSSO)
	{
		size_t newLength = stack.size - count;

		std::memmove( stack.buffer + index, stack.buffer + index + count, stack.size - count );
		stack.buffer[newLength] = '\0';
		stack.size = newLength;
	}
	else
	{
		size_t newLength = heap.size - count;

		std::memmove( heap.ptr + index, heap.ptr + index + count, heap.size - count );
		heap.ptr[newLength] = '\0';
		heap.size = newLength;
	}
	
	return *this;
}

String& String::Replace(size_t pos, size_t count, const char* string)
{
	size_t replaceLength = std::strlen(string);

	if( replaceLength == 0 ) return *this;

	int diff = replaceLength - count;

	// Stack allocated
	if( isSSO )
	{
		size_t newLength = stack.size + diff;

		// Less than stack size
		if( newLength < SSO_CAPACITY )
		{
			if( diff != 0 )
			{
				std::memmove(stack.buffer + pos + count + diff, stack.buffer + pos + count, count);
			}

			std::memcpy(stack.buffer + pos, string, replaceLength);
			stack.buffer[newLength] = '\0';
			stack.size = newLength;
		}
		// Greater than stack size
		else
		{
			Allocate(newLength, SSO_CAPACITY);

			if( diff != 0 )
			{
				std::memmove(heap.ptr + pos + count + diff, heap.ptr + pos + count, count - 1);
			}

			std::memcpy(heap.ptr + pos, string, replaceLength);
			heap.ptr[newLength] = '\0';
			heap.size = newLength;
		}
	}
	// Heap allocated
	else
	{
		size_t newLength = heap.size + diff;

		// Greater than heap size
		if( newLength > heap.capacity )
		{
			Allocate(newLength, heap.capacity);

			if(diff != 0 )
			{
				std::memmove(heap.ptr + pos + count + diff, heap.ptr + pos + count, Size() - pos + count - 1);
			}

			std::memcpy(heap.ptr + pos, string, replaceLength);
			heap.ptr[newLength] = '\0';
			heap.size = newLength;
		}
		// Smaller than heap size
		else
		{
			if( diff != 0 )
			{
				std::memmove(heap.ptr + pos + count + diff, heap.ptr + pos + count, Size() - pos + count - 1);
			}

			std::memcpy(heap.ptr + pos, string, replaceLength);
			heap.ptr[newLength] = '\0';
			heap.size = newLength;
		}
	}

	return *this;
}

String& String::Replace(size_t pos, size_t count, const String& string)
{
	size_t replaceLength = string.Size();

	if( replaceLength == 0 ) return *this;

	int diff = replaceLength - count;

	// New sub string stack allocated
	if( string.isSSO )
	{
		// This string stack allocated
		if( isSSO )
		{
			size_t newLength = stack.size + diff;

			// Less than stack capacity
			if( newLength < SSO_CAPACITY )
			{
				if( diff != 0 )
				{
					std::memmove(stack.buffer + pos + count + diff, stack.buffer + pos + count, count);
				}

				std::memcpy(stack.buffer + pos, string.stack.buffer, replaceLength);
				stack.buffer[newLength] = '\0';
				stack.size = newLength;
			}
			// Greater than stack capacity
			else
			{
				Allocate(newLength, SSO_CAPACITY);

				if( diff != 0 )
				{
					std::memmove(heap.ptr + pos + count + diff, heap.ptr + pos + count, count - 1);
				}

				std::memcpy(heap.ptr + pos, string.stack.buffer, replaceLength);
				heap.ptr[newLength] = '\0';
				heap.size = newLength;
			}
		}
		// This string heap allocated
		else
		{
			size_t newLength = heap.size + diff;
			
			// Greater than heap size
			if( newLength > heap.capacity )
			{
				Allocate(newLength, heap.capacity);

				if( diff != 0 )
				{
					std::memmove(heap.ptr + pos + count + diff, heap.ptr + pos + count, Size() - pos + count - 1);
				}

				std::memcpy(heap.ptr + pos, string.stack.buffer, replaceLength);
				heap.ptr[newLength] = '\0';
				heap.size = newLength;
			}
			// Smaller than heap size
			else
			{
				if( diff != 0 )
				{
					std::memmove(heap.ptr + pos + count + diff, heap.ptr + pos + count, Size() - pos + count - 1);
				}

				std::memcpy(heap.ptr + pos, string.stack.buffer, replaceLength);
				heap.ptr[newLength] = '\0';
				heap.size = newLength;
			}
		}
	}
	// New substring heap allocated
	else
	{
		// This string stack allocated
		if( isSSO )
		{
			size_t newLength = stack.size+ diff;

			Allocate(newLength, SSO_CAPACITY);

			if( diff != 0 )
			{
				std::memmove(heap.ptr + pos + count + diff, heap.ptr + pos + count, Size() - pos + count - 1);
			}

			std::memcpy(heap.ptr + pos, string.heap.ptr, replaceLength);
			heap.ptr[newLength] = '\0';
			heap.size = newLength;
		}
		// This string heap allocated
		else
		{
			size_t newLength = heap.size + diff;

			// Greater than heap size
			if( newLength > heap.capacity )
			{
				Allocate(newLength, heap.capacity);

				if( diff != 0 )
				{
					std::memmove(heap.ptr + pos + count + diff, heap.ptr + pos + count, Size() - pos + count - 1);
				}

				std::memcpy(heap.ptr + pos, string.heap.ptr, replaceLength);
				heap.ptr[newLength] = '\0';
				heap.size = newLength;
			}
			// Smaller than heap size
			else
			{
				if( diff != 0 )
				{
					std::memmove(heap.ptr + pos + count + diff, heap.ptr + pos + count, Size() - pos + count - 1);
				}

				std::memcpy(heap.ptr + pos, string.heap.ptr, replaceLength);
				heap.ptr[newLength] = '\0';
				heap.size = newLength;
			}
		}
	}

	return *this;
}

size_t String::Find( const String& string, size_t pos ) const
{
	if (string.Size() == 0) return pos <= string.Size() ? pos : npos;

	if (string.Size() + pos > this->Size()) return npos;

	for (size_t i = pos; i <= this->Size() - string.Size(); ++i)
	{
		if (std::memcmp( this->CStr() + i, string.CStr(), string.Size() ) == 0)
		{
			return i;
		}
	}

	return npos;
}

size_t String::Find( const char* string, size_t pos ) const
{
	size_t length = std::strlen( string );

	if (length == 0) return pos <= length ? pos : npos;

	if (length + pos > this->Size()) return npos;

	for (size_t i = pos; i <= this->Size() - length; ++i)
	{
		if (std::memcmp( this->CStr() + i, string, length ) == 0)
		{
			return i;
		}
	}

	return npos;
}

size_t String::RFind( const String& string, size_t pos ) const
{
	if (string.Size() == 0) return pos < string.Size() ? pos : npos;

	if (string.Size() > this->Size()) return npos;

	size_t start = std::min( pos, this->Size() - string.Size() );
	for (size_t i = start + 1; i-- > 0;)
	{
		if (std::memcmp( this->CStr() + i, string.CStr(), string.Size() ) == 0)
		{
			return i;
		}
	}

	return npos;
}

size_t String::RFind( const char* string, size_t pos ) const
{
	size_t length = std::strlen( string );

	if (length == 0) return pos < length ? pos : npos;

	if (length > this->Size()) return npos;

	size_t start = std::min( pos, this->Size() - length );
	for (size_t i = start + 1; i-- > 0;)
	{
		if (std::memcmp( this->CStr() + i, string, length ) == 0)
		{
			return i;
		}
	}

	return npos;
}

bool String::Empty() const
{
	return isSSO ? stack.size == 0 : heap.size == 0;
}

size_t String::Capacity() const
{
	return isSSO ? SSO_CAPACITY : heap.capacity;
}

size_t String::Length() const
{
	return Size();
}

size_t String::Size() const
{
	return isSSO ? stack.size : heap.size;
}

size_t String::MaxSize() const
{
	return std::distance(begin(), end());
}

void String::Reserve(size_t newCap)
{
	if (isSSO)
	{
		if (newCap > SSO_CAPACITY)
		{
			size_t length = stack.size;

			isSSO = false;
			char* newData = new char[newCap + 1];
			std::memcpy( newData, stack.buffer, length );
			newData[length] = '\0';
			heap.ptr = newData;
			heap.size = length;
			heap.capacity = newCap;
		}
	}
	else
	{
		if (newCap > heap.capacity)
		{
			char* newData = new char[newCap + 1];
			std::memcpy( newData, heap.ptr, heap.size );
			newData[heap.size] = '\0';
			delete[] heap.ptr;
			heap.ptr = newData;
			heap.capacity = newCap;
		}
	}
}

void String::Shrink()
{
	if (isSSO) return;

	size_t size = heap.size;

	if (!isSSO && size < SSO_CAPACITY)
	{
		char* temp = new char[size + 1];
		std::memcpy( temp, heap.ptr, size );
		temp[size] = '\0';
		delete[] heap.ptr;


		isSSO = true;
		std::memcpy( stack.buffer, temp, size );
		stack.buffer[size] = '\0';
		delete[] temp;
	}
	else
	{
		char* temp = new char[size + 1];
		std::memcpy( temp, heap.ptr, size );
		temp[size] = '\0';
		delete[] heap.ptr;

		heap.ptr = temp;
		heap.capacity = size;
	}
}

int String::Compare( const char* string ) const
{
	size_t lhs = Size();
	size_t rhs = std::strlen( string );

	int result = std::memcmp( Data(), string, std::min( lhs, rhs ) );

	if (result != 0)
	{
		return result;
	}
	if (lhs < rhs)
	{
		return -1;
	}
	if (lhs > rhs)
	{
		return 1;
	}
	return 0;
}

int String::Compare( const String& string ) const
{
	size_t lhs = Size();
	size_t rhs = string.Size();

	int result = std::memcmp( Data(), string.Data(), std::min( lhs, rhs ) );

	if (result != 0)
	{
		return result;
	}
	if (lhs < rhs)
	{
		return -1;
	}
	if (lhs > rhs)
	{
		return 1;
	}
	return 0;
}

const char* String::CStr() const
{
	return isSSO ? stack.buffer : heap.ptr;
}

const char* String::Data() const
{
	return isSSO ? stack.buffer : heap.ptr;
}

char* String::Data()
{
	return isSSO ? stack.buffer : heap.ptr;
}

char& String::At(size_t pos)
{
	if( pos >= Size() )
	{
		throw std::out_of_range("This index is out of range.");
	}

	return *( isSSO ? stack.buffer + pos : heap.ptr + pos );
}

const char& String::At(size_t pos) const
{
	if( pos >= Size() )
	{
		throw std::out_of_range("This index is out of range.");
	}

	return *( isSSO ? stack.buffer + pos : heap.ptr + pos );
}

void String::Allocate(size_t newSize, size_t oldCap)
{
	size_t newCap = std::max(newSize, oldCap + oldCap / 2);

	if( this->Empty() )
	{
		if( isSSO )
		{
			isSSO = false;
			heap.ptr = new char[newCap + 1];
			std::memset(heap.ptr, 0, newCap);
			heap.size = 0;
			heap.capacity = newCap;
		}
		else
		{
			delete[] heap.ptr;

			isSSO = false;
			heap.ptr = new char[newCap + 1];
			std::memset(heap.ptr, 0, newCap);
			heap.capacity = newCap;
		}
	}
	else
	{
		if( isSSO )
		{
			size_t size = stack.size;

			isSSO = false;
			char* newData = new char[newCap + 1];
			std::memcpy(newData, stack.buffer, size);
			newData[size] = '\0';

			heap.ptr = newData;
			heap.size = size;
			heap.capacity = newCap;
		}
		else
		{

			isSSO = false;
			char* newData = new char[newCap + 1];
			std::memcpy(newData, heap.ptr, heap.size);
			newData[heap.size] = '\0';

			delete[] heap.ptr;

			heap.ptr = newData;
			heap.capacity = newCap;
		}
	}
	
}

void String::Debug() const
{
	std::cout << "Contents: " << CStr() << '\n' << "Size: " << Size() << '\n' << "Capacity: " << Capacity() << '\n' << "allocated: " << (isSSO ? "Stack" : "Heap" ) << "\n\n";
}

String operator+( const String& lhs, const char* rhs )
{
	String result = lhs;
	result += rhs;
	return result;
}

String operator+( const char* lhs, const String& rhs )
{
	String result = lhs;
	result += rhs;
	return result;
}

String operator+( const String& lhs, const String& rhs )
{
	String result = lhs;
	result += rhs;
	return result;
}

bool operator==( const String& lhs, const char* rhs )
{
	return lhs.Compare(rhs) == 0;
}

bool operator==( const char* lhs, const String& rhs )
{
	return rhs.Compare( lhs ) == 0;
}

bool operator==( const String& lhs, const String& rhs )
{
	return lhs.Compare( rhs ) == 0;
}

bool operator!=( const String& lhs, const char* rhs )
{
	return lhs.Compare( rhs ) != 0;
}

bool operator!=( const char* lhs, const String& rhs )
{
	return rhs.Compare( lhs ) != 0;
}

bool operator!=( const String& lhs, const String& rhs )
{
	return lhs.Compare( rhs ) != 0;
}

bool operator<( const String& lhs, const char* rhs )
{
	return lhs.Compare( rhs ) < 0;
}

bool operator<( const char* lhs, const String& rhs )
{
	return rhs.Compare( lhs ) < 0;
}

bool operator<( const String& lhs, const String& rhs )
{
	return lhs.Compare( rhs ) < 0;
}

bool operator>( const String& lhs, const char* rhs )
{
	return lhs.Compare( rhs ) > 0;
}

bool operator>( const char* lhs, const String& rhs )
{
	return rhs.Compare( lhs ) > 0;
}

bool operator>( const String& lhs, const String& rhs )
{
	return lhs.Compare( rhs ) > 0;
}

bool operator<=( const String& lhs, const char* rhs )
{
	return lhs.Compare( rhs ) <= 0;
}

bool operator<=( const char* lhs, const String& rhs )
{
	return rhs.Compare( lhs ) <= 0;
}

bool operator<=( const String& lhs, const String& rhs )
{
	return lhs.Compare( rhs ) <= 0;
}

bool operator>=( const String& lhs, const char* rhs )
{
	return lhs.Compare( rhs ) >= 0;
}

bool operator>=( const char* lhs, const String& rhs )
{
	return rhs.Compare( lhs ) >= 0;
}

bool operator>=( const String& lhs, const String& rhs )
{
	return lhs.Compare( rhs ) >= 0;
}

std::ostream& operator<<(std::ostream& os, const String& string)
{
	os << string.CStr();
	return os;
}

std::istream& operator>>(std::istream& is, String& string)
{
	std::istream::sentry sentry(is);

	if( !sentry ) return is;

	char buffer[1024];
	is.get(buffer, sizeof(buffer), '\n');

	if( is.peek() == ' ' ) is.get();

	string = buffer;

	return is;
}
