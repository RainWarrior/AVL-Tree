#include <iostream>
using namespace std;

#include "links_nodes.h"
#include "Record.h"
#include "student.h"

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

//typedef MakeTypelist<Char, Float, Int, Int>::Result Test1;

struct test2
{
	char x;
	float y;
	int z;
	int t;
};

int main()
{
	//cout << sizeof(Test) << ' ' << sizeof(test2) << ' ' << sizeof(int)*2 + sizeof(float) + sizeof(char) << endl;
	//Test1 *a = new Test1();
	//Tuple<Test1> t;
	//Field<0>(t).v = 'f';
	//Field<1>(t).v = .5;
	//Field<2>(t).v = 1;
	//Field<3>(t).v = 2;
	//cout << Field<2>(t).v << endl;
	//cout << Field<3>(t).v << endl;
	//char b;
	//b = Field<0>(t).v;
	//cout << b << endl << endl;

	DB test;

	//test.phone.init(cin, 10);
	//test.phone.print(cout);

	int n, phone, group;
	char name[255];
	char cmd[255];
	DB::Record *r;
	//n = 1000000;
	while(cin >> cmd)
	{
		if(!strcmp(cmd, "ping"))
		{
			cout << "pong" << endl;
		}
		if(!strcmp(cmd, "add")) // add vasya 89165555555 201
		{
			cin >> name >> phone >> group;
			test.add(group, phone, name);
		}
		else if(!strcmp(cmd, "madd"))
		{
			cin >> n;
			cerr << "madd " << n << endl;
			for(int i=0; i<n; i++)
			{
				cin >> name >> phone >> group;
				test.add(group, phone, name);
			}
			//break;
		}
		else if(!(strcmp(cmd, "get") && strcmp(cmd, "del"))) // get name vasya; get group 201
		{
			cin >> name;
			if(!strcmp(cmd, "del")) cout << "del" << endl;
			if(!strcmp(name, "name"))
			{
				cin >> name;
				r = test.get_name(String(name));
				if(r) r->print(cout);
				else cout << "not found : " << name << endl;
			}
			else if(!strcmp(name, "phone"))
			{
				cin >> phone;
				r = test.get_phone(phone);
				if(r) r->print(cout);
				else cout << "not found : " << phone << endl;
			}
			else if(!strcmp(name, "group"))
			{
				cin >> group;
				r = test.get_group(group);
				if(r) r->print(cout);
				else cout << "not found : " << group << endl;
			}
			else continue;
			if(!strcmp(cmd, "del"))
			{
				if(r) test.remove(r);
			}
		}
	}

/*	for(int i=0; i<4; i++)
	{
		test.group_phone[i].print(cout);
		cout << endl;
	}*/
	test.ph.print(cout);
	cout << endl;
	//cout << test.ph.check_root() << endl;

	//cout << Conversion<DB::Record*, DLinked<DB::Record>* >::exists << endl;
	//student s1(0,"test1"), s2(1, "test2");
	//cout << s1 << s2 << endl;
	//s1 = s2;
	//cout << s1 << s2 << endl;
	//cout << sizeof(DB::Record);
	return 0;
}

