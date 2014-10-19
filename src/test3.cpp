#include <iostream>
#include "student.h"
using namespace std;

struct A
{
	int a;
};

struct B : public A
{
	int b;
};

struct C : public B, A
{
	int c;
};

int main()
{
	C c;
	B &pc = c;
	A *a;
	a = &pc;
	C c2 = *static_cast<C*>(a);
	student a(
	return 0;
}
