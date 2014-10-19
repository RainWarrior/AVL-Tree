#ifndef __RECORD_H
#define __RECORD_H 1

#include <limits.h>

#include "links_nodes.h"
#include "predicate.h"
#include "AVLTree.h"
#include "String.h"

//typedef DList<Record, 0> GroupList;

template<class T>
class Holder
{
private:
	T _value;
public:
	operator T&()
	{
		return _value;
	}
	operator const T&() const
	{
		return _value;
	}
	Holder() : _value() {}
	Holder(T &v) : _value(v) {}
	Holder(const T &v) : _value(v) {}
};

class DB
{
	SLinked * pool;
public:

	struct Record : public Tuple<MakeNTypelist<
		SLinked,     // 0 simple linear link
		SLinked,     // 1 Group list (TODO)
		AVLinked,    // 2 group phone index
		AVLinked,    // 3 phone index
		Holder<int>, // 4 group
		Holder<int>, // 5 phone
		String       // 6 name
	>::Result>
	{
		void print(std::ostream &out)
		{
			out << Field<6>(*this) << ' ' << Field<5>(*this) << ' ' << Field<4>(*this) << std::endl;
		}
		String* format(int fields)
		{
			char buf[256];
			int len=0, n=0;
			memset(buf, 0, 256 * sizeof(char));
			while(fields)
			{
				n=0;
				if((fields & 3) == 1) // name
				{
					sprintf(buf+len, "%s%n", Field<6>(*this).c_str(), &n);
//					std::cerr << "AA" << Field<6>(*this) << "AA" << std::endl;
				}
				else if((fields & 3) == 2) // phone
				{
					sprintf(buf+len, "%d%n", Field<5>(*this), &n);
				}
				else if((fields & 3) == 3) // group
				{
					sprintf(buf+len, "%d%n", Field<4>(*this), &n);
				}
				len += n;
//				std::cerr << "F: " << fields << ' ' << (fields & 3) << ' ' << len << ' ' << n << std::endl;
				fields >>= 2;
				if(fields) buf[len++] = ' ';
			}
			return new String(buf);
		}
	};

	DB() : pool(0) {}
	~DB()
	{
		SLinked *t;
		while(pool)
		{
			t = pool->r;
			delete rkey<Record, 0>(pool);
			pool = t;
		}
	}

	String& name(Record *record)
	{
		return Field<6>(*record);
	}

	AVLTree<Record, 2, 5> group_ph[600];
	AVLTree<Record, 3, 5> ph;

	typedef AVLTree<Record, 3, 5>::iterator IndexIterator;
	void add(int group, int phone, char *name)
	{
		Record *r = new Record();
		Field<4>(*r) = group;
		Field<5>(*r) = phone;
		Field<6>(*r) = String(name);
		add(r);
	}
	void add_dumb(int group, int phone, char *name)
	{
		Record *r = new Record();
		Field<4>(*r) = group;
		Field<5>(*r) = phone;
		Field<6>(*r) = String(name);
		//add(r);
	}
	void add(Record *r)
	{
		Field<0>(*r).r = pool;
		pool = &Field<0>(*r);
		group_ph[Field<4>(*r)].add(r);
		ph.add(r);
	}
	void remove(Record *r)
	{
//		std::cerr << "WOOHOO2" << endl;
		group_ph[Field<4>(*r)].remove(r);
		ph.remove(r);
		SLinked *R = &Field<0>(*r);
		if(pool != R)
		{
			SLinked *res=pool;
			for(; res && res->r != R; res = res->r);
			assert(res); // should be in list
			res->r = R->r;
		}
		else pool = R->r;
		delete r;
	}
/*	Record* get_group(int group)
	{
		return group_ph[group].getmin();
	}*/
	Record* get_phone(int phone)
	{
		return ph.get(phone);
	}
	AVLinked* getkey_phone(int phone, int group=-1)
	{
		if(group == -1) return ph.getkey(phone);
		return group_ph[group].getkey(phone);
	}
	Record* get_name(const String &s)
	{
		Record *res;
		for(res=rkey<Record, 0>(pool); res && Field<6>(*res) != s; res = rkey<Record, 0>(Field<0>(*res).r));
		return res;
	}
	ResultIterator* get_all()
	{
		ResultIterator *it = new AVLTree<Record, 3, 5>::AVLResultIterator(
			&ph,
			INT_MIN,
			INT_MAX,
			0,
			57,
			0
		);
		return it;
	}
};

#endif // __RECORD_H
