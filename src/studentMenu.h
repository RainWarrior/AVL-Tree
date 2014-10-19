#include <cstring>
#include <cstdlib>
#include "student.h"
#include "iMenu.h"

class _studentMenu : public student
{
protected:
	student *root;
public:
	_studentMenu() : student(), root(this) {}
	int doStaff(int n, std::istream &in, std::ostream&)
	{
		char name[255];
		switch(n)
		{
		case 1: // update name
			in >> name;
			delete[] this->name;
			this->name = new char[strlen(name)+1];
			strcpy(this->name, name);
			break;
		case 2: // update value
			in >> n;
			this->value = n;
			break;
		}
		return 0;
	}
	const char* getHelp()
	{
		return " 1: update name\n 2: update value\n";
	}
	void init(std::istream &in, int *size=0)
	{
		if(this->name) delete[] this->name;
		char *c, s[256], n[256];
		int v;
		if(!(in >> n >> v)) throw(-1);
//		in >> n >> v;
//		if(!in.getline(s, 256)) throw(-1);
//		sscanf(s, "%s %d", n, &v);
//		c = s;
//		while(isspace(*c)) c++;
//		v = strchr(c, ' ') - c;
//		std::cout << v << ' ' << strchr(c, ' ') << ' ' << c << std::endl;
//		strncpy(n, c, v);
//		n[v] = 0;
//		v = atoi(c + v);
		this->value = v;
		this->name = new char[strlen(n)+1];
		strcpy(this->name, n);
	}
	void print(std::ostream &out)
	{
		out << *(student*) this;
	}
//	virtual ~studentMenu() {}
};

typedef iMenu<_studentMenu> studentMenu;
