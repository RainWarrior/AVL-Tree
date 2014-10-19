#ifndef __LINKS_NODES_H
#define __LINKS_NODES_H 1

#include <iostream>
#include "abstract.h"

template<class H, unsigned int i, class T>
H* rkey(T *obj)
{
	return static_cast<H*>(static_cast<TupleUnit<NumType<T, i> >*>(obj));
}

template<class H, unsigned int i, unsigned int j>
struct Index
{
	typedef typename FieldHelper<H, i>::ResultType KeyType;
	typedef typename FieldHelper<H, j>::ResultType ValueType;
	static KeyType& key(H& obj)
	{
		return FieldHelper<H, i>::Do(obj);
	}
	static KeyType* pkey(H* obj)
	{
		return &FieldHelper<H, i>::Do(*obj);
	}
	static ValueType& value(H& obj)
	{
		return FieldHelper<H, j>::Do(obj);
	}
	static ValueType& prvalue(KeyType* obj)
	{
		return FieldHelper<H, j>::Do(*rkey<H, i>(obj));
	}
};

struct SLinked
{
	SLinked *r;
	SLinked() : r(0) {}
};

struct DLinked
{
	DLinked *r;
	DLinked *l;
	DLinked() : r(0), l(0) {}
};

struct TLinked
{
	TLinked *r;
	TLinked *l;
	TLinked *p;
	TLinked() : r(0), l(0), p(0) {}
};

#endif

