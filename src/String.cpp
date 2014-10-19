#include "String.h"

std::ostream& operator<<(std::ostream &out, const String &s)
{
	return out << s.str;
}

std::istream& operator>>(std::istream &in, String &s)
{
	char str[255];
	in >> str;
	s = String(str);
	return in;
}

