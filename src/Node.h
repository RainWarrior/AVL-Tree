#include <iostream>

#ifndef __I_MENU_H
#define __I_MENU_H 1
template<class T>
class iMenu : public T
{
public:
	int doStaff(int n, std::istream &in, std::ostream &out)
	{
		switch(n)
		{
		case 0: // exit
			return -1;
		case -1:
			out << " 0: exit\n"
			    << "-1: print this message\n"
			    << "-2: print data\n"
			    << "-3: update data\n"
			    << static_cast<T*>(this)->getHelp();
			break;
		case -2: // print
			T::print(out);
			break;
		case -3: // update value
			T::init(in);
			break;
		default: // do specific stuff
			return T::doStaff(n, in, out);
		}
		return 0;
	}
	void menu(std::istream &in, std::ostream &out)
	{
		int n;
		while(in >> n)
		{
			if(doStaff(n, in, out)) return;
		}
	}
	void init(std::istream &in, int *size = 0)
	{
		T::init(in, size);
	}
	int operator<(const iMenu<T> &a) const
	{
		if(this->root)
		{
			if(a.root)
			{
				return *this->root < *a.root;
			}
			return 0;
		}
		return 0;
	}
};

#endif
