#pragma once

class String
{
public:


private:
	size_t capacity;

	union
	{
		struct
		{
			char* heap;
			size_t size;
		};

		char stack[16];
	};

	
};

