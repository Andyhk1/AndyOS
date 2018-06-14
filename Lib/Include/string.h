#pragma once

unsigned int strcmp(const char* str1, const char* str2);
char* strcpy(char *dest, const char *src);
unsigned int strlen(const char* str);
char* strcat(char* dest, const char* src);
char* strncat(char* dest, const char* src, int n);

//extern "C" void* memcpy(void* dest, void* src, unsigned int n);
void* memcpy(void* dest, void* src, unsigned int n);
//void memcpy_fast_128(void* dest, void* src, unsigned int count);

extern "C" void* memset(void* dest, int val, int n);
#pragma intrinsic(memset)
//void* memset(void *dest, unsigned char val, unsigned int n);
void* memset16(void *dest, unsigned short val, unsigned int n);
void* memset32(void *dest, unsigned int val, unsigned int n);
//void* memset_fast_128(void* dest, unsigned int val, unsigned int n);

int memcmp(const void* a, const void* b, unsigned int n);

char* search(char* str, const char* delim);
char* strtok_r(char* str, const char* delim, char** saveptr);

class String
{
public:
	String()
	{
		buffer = new char[0x1000];
		buffer[0] = 0;
	}

	String(char* str)
	{
		int length = strlen(str);
		buffer = new char[length + 1];
		memcpy(buffer, str, length + 1);
	}

	~String()
	{
		delete[] buffer;
	}

	inline int Length() { return strlen(buffer); }

	inline char* ToChar()
	{
		int len = Length();
		char* c = new char[len + 1];
		strcpy(c, buffer);
		c[len] = 0;
		return c;
	}

	bool Contains(String str)
	{
		char* a = buffer;

		while (*a)
		{
			char* temp = a++;
			char* b = str.buffer;

			while (*temp++ == *b++)
			{
				if (!*b)
					return 1;
			}
		}

		return 0;
	}

	/*void String::Split(List<String>& out, char delim)
	{
		char* start = buffer;
		char* end = buffer;

		while (*end)
		{
			while (*end != delim && *end++);

			while (*end == delim)
				*end++ = 0;

			out.Add(start);
			start = end;
		}
	}*/

	String& operator+=(String& str)
	{
		strcat(buffer, str.buffer);
		return *this;
	}

	/*String& operator+=(const char*& str)
	{
	strcat(buffer, str);
	return *this;
	}

	String& operator+=(char*& str)
	{
	strcat(buffer, str);
	return *this;
	}*/

	String& operator+=(const char str[])
	{
		strcat(buffer, str);
		return *this;
	}

	String& operator+=(const char c)
	{
		int len = Length();
		buffer[len++] = c;
		buffer[len] = 0;
		return *this;
	}

	String& operator+(String& str)
	{
		String* ns = new String(buffer);
		*ns += str;
		return *ns;
	}

	String& operator+(const char*& str)
	{
		String* ns = new String(buffer);
		*ns += str;
		return *ns;
	}

	String& operator+(char*& str)
	{
		String* ns = new String(buffer);
		*ns += str;
		return *ns;
	}

	String& operator=(const char*& str)
	{
		strcpy(buffer, str);
		return *this;
	}

	String& operator=(const String& str)
	{
		strcpy(buffer, str.buffer);
		return *this;
	}

	bool operator==(const String& str)
	{
		return !strcmp(buffer, str.buffer);
	}

private:
	char* buffer;
};