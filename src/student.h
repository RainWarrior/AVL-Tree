#include <iostream>
#include <algorithm>
#include <cstring>

class student
{
protected:
	int value;
	char* name;
public:
	void swap(student &s)
	{
		std::swap(value, s.value);
		std::swap(name, s.name);
	}
	student(int v=0, const char *n = 0) : value(v), name(0)
	{
		if(n)
		{
			name = new char[strlen(n)+1];
			strcpy(name, n);
//			std::cerr << "name" << ' ' << long(name) << '_' << name << '_' << v << std::endl;
//			std::cerr << "stud" << ' ' << (void*)this << ' ' << (void*)name << ' ' << v << std::endl;
		}
	}
	~student()
	{
//		std::cerr << "~stud" << ' ' << (void*)this << ' ' << (void*)name << std::endl;
		if(name) delete[] name;
	}
	student & operator=(student s)
	{
		swap(s);
		return *this;
	}
	student(const student & s) : value(s.value)
	{
		if(s.name)
		{
			name = new char[strlen(s.name)+1];
			strcpy(name, s.name);
//			std::cerr << "copy" << ' ' << long(name) << '_' << name << '_' << s.value << std::endl;
//			std::cerr << "copy" << ' ' << (void*)this << ' ' << (void*)&s << ' '  << (void*)name << std::endl;
		}
	}
	int getValue() const
	{
		return value;
	}
	friend std::ostream& operator<<(std::ostream &out, const student &s);
	friend std::istream& operator>>(std::istream &in, student &s);
	int operator<(const student &s) const
	{
//		std::cerr << "<<<<" << std::endl;
		return value < s.value;
	}
};
//} emptyStudent(0, "");
std::ostream& operator<<(std::ostream &out, const student &s)
{
	return out << s.name << ' ' << s.value << std::endl;
}

std::istream& operator>>(std::istream &in, student &s)
{
	char name[255];
	int value;
	in >> name >> value;
	s = student(value, name);
	return in;
}

