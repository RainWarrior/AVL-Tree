#ifndef __PREDICATE_H
#define __PREDICATE_H 1

#include "abstract.h"

template<class T>
class BasePredicate
{
public:
	int count;
	BasePredicate() : count(0) {}
	virtual ~BasePredicate() {};
	virtual int operator()(T &data) = 0;
};

template<class T, unsigned int ivalue>
class Predicate : public BasePredicate<T>
{
	typedef typename FieldHelper<T, ivalue>::ResultType DataType;
	int type;
	DataType limit;
	BasePredicate<T> *next;

public:
	Predicate(int t, const DataType &l, BasePredicate<T> *n=0) : BasePredicate<T>(), type(t), limit(l), next(n) {}
	virtual int operator()(T &data)
	{
		if(next && !(*next)(data)) return 0;
		DataType &d = Field<ivalue>(data);
		if(type == 1) return !(d < limit || limit < d); // ==
		if(type == 2) return d < limit || limit < d;    // !=
		if(type == 3) return d < limit;                 // <
		if(type == 4) return limit < d;                 // >
		if(type == 5) return !(limit < d);              // <=
		if(type == 6) return !(d < limit);              // >=
	}
	virtual ~Predicate()
	{
		if(next) delete next;
	};
};

#endif // __PREDICATE_H
