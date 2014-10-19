#ifndef __MY_STRING_H
#define __MY_STRING_H 1

#include <iostream>
#include <algorithm>
#include <cstring>

class String
{
protected:
	char* str;
public:
	void swap(String &s)
	{
		std::swap(str, s.str);
	}
	String(const char *s = 0) : str(0)
	{
		if(s)
		{
			str = new char[strlen(s)+1];
			strcpy(str, s);
		}
	}
	~String()
	{
		if(str) delete[] str;
	}
	String & operator=(String s)
	{
		swap(s);
		return *this;
	}
	String(const String & s)
	{
		if(s.str)
		{
			str = new char[strlen(s.str)+1];
			strcpy(str, s.str);
		}
	}
	size_t size() const
	{
		return strlen(str);
	}
	const char* c_str() const
	{
		return str;
	}
	friend std::ostream& operator<<(std::ostream &out, const String &s);
	friend std::istream& operator>>(std::istream &in, String &s);
	int operator==(const String &s) const
	{
		return !strcmp(str, s.str);
	}
	int operator!=(const String &s) const
	{
		return strcmp(str, s.str);
	}
	int operator<(const String &s) const
	{
		return strcmp(str, s.str) < 0;
	}
};

#endif // __MY_STRING_H
