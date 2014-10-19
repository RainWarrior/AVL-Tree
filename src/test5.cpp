#include <iostream>
#include <typeinfo>
using namespace std;

#include "links_nodes.h"

struct test
{
};

template<class T, int U>
struct test1
{
	typedef T Type;
	enum { index = U };
};

template<class T>
struct select;

template<class T, unsigned int U>
struct select<test1<T, U> >
{
	enum { right=1 };
	typedef T Type;
};

template<class T>
struct select
{
	enum { right=0 };
	typedef T Type;
};

int main()
{
	cout << select<test>::right << ' ' << select<test1<test, 3> >::right << endl;
	cout << typeid(select<test1<test, 3> >::Type).name() << endl;
	cout << UnitType<int>::wrong << ' ' << UnitType<NumType<int, 15u> >::wrong << endl;
	return 0;
}
