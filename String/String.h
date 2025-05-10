#pragma once

#include <cstdint>
#include <iterator>

class String
{
public:
	using iterator = char*;
	using const_iterator = const char*;

	String();
	String(const char* string);
	String(const String& string);
	String(String&& string) noexcept;
	~String();

	String& operator=(const String& string);
	String& operator=(String&& string) noexcept;

	String& operator+=(const char* string);
	String& operator+=(const String& string);

	String operator+(const char* string);
	String operator+(const String& string);

	void Clear();

	String& Append(const char* string);
	String& Append(const String& string);

	String& Insert(size_t index, const char* string);
	String& Insert(size_t index, const String& string);

	String& Erase(size_t index = 0, size_t count = -1);

	bool Empty() const;

	size_t Capacity() const;

	size_t Size() const;

	const char* CStr() const;

	iterator begin() { return isSSO ? stack.buffer : heap.ptr; }
	iterator end() { return begin() + Size(); }
	const_iterator begin() const { return isSSO ? stack.buffer : heap.ptr; }
	const_iterator end() const { return begin() + Size(); }
	const_iterator cbegin() const { return begin(); }
	const_iterator cend() const { return end(); }

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

