#include <iostream>
#include <typeinfo>
using namespace std;

#include "links_nodes.h"

struct Char
{
	char v;
};

struct Float
{
	float v;
};

struct Int
{
	int v;
};

//typedef Typelist<char, Typelist<float, Typelist<int, Typelist<int, NullType> > > > Test;
//typedef Typelist<char, Typelist<float, NullType> > Test;

typedef MakeNTypelist<Char, Float, Int, Int>::Result Test1;
typedef Tuple<Test1> Test2;

int main()
{
	Test2 t;
	cout << typeid(Test1).name() << endl;
	cout << typeid(TypeAt<Test1, 0>::Result).name() << endl;
	cout << typeid(TypeAt<Test1, 1>::Result).name() << endl;
	cout << typeid(TypeAt<Test1, 2>::Result).name() << endl;
	cout << typeid(TypeAt<Test1, 3>::Result).name() << endl;

	cout << typeid(Test2).name() << endl;
	cout << typeid(Test2::TList).name() << endl;
	cout << typeid(TypeAt<Test2::TList, 0>::Result).name() << endl;
	cout << typeid(TypeAt<Test2::TList, 1>::Result).name() << endl;
	cout << typeid(TypeAt<Test2::TList, 2>::Result).name() << endl;
	cout << typeid(TypeAt<Test2::TList, 3>::Result).name() << endl;

	cout << typeid(FieldHelper<Test2, 0>::ResultType).name() << endl;
	cout << typeid(FieldHelper<Test2, 1>::ResultType).name() << endl;
	cout << typeid(FieldHelper<Test2, 2>::ResultType).name() << endl;
	cout << typeid(FieldHelper<Test2, 3>::ResultType).name() << endl;
	return 0;
}

