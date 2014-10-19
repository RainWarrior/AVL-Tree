#include <iostream>
#include <fstream>
#include "studentMenu.h"
#include "Stack.h"
#include "DList.h"
#include "Tree.h"

using namespace std;

int main()
{
	ifstream fin("a.txt");
//	studentMenu st;
	int a[3];
	cin >> a[0] >> a[1] >> a[2];
	Tree<DList<Stack<studentMenu>::Tp>::Tp>::Tp s;
//	Tree<studentMenu>::Tp s;
	s.init(fin, a);
	s.menu(cin, cout);
	return 0;
}
