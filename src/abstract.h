#ifndef __ABSTRACT_H
#define __ABSTRACT_H 1

#include <typeinfo>
#include <cassert>

struct NullType
{
};

template<class T, class U>
struct Typelist
{
	typedef T Head;
	typedef U Tail;
};

template<class T1=NullType, class T2=NullType, class T3=NullType,
         class T4=NullType, class T5=NullType, class T6=NullType,
         class T7=NullType, class T8=NullType, class T9=NullType,
         class T10=NullType, class T11=NullType, class T12=NullType>
struct MakeTypelist
{
	typedef Typelist<T1, typename MakeTypelist<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::Result> Result;
};

template<>
struct MakeTypelist<>
{
	typedef NullType Result;
};

template<class T, unsigned int i>
struct NumType
{
	typedef T Type;
	enum { index = i };
};

template<class T> struct NumTypeType;

template<class T, unsigned int i>
struct NumTypeType<NumType<T, i> >
{
	typedef T Type;
	enum { index = i };
};

template<class T>
struct NumTypeType
{
	typedef T Type;
	enum { index = -1 };
};

template<class TList, unsigned int i> struct TypeAt;

template<class T1, class T2>
struct TypeAt<Typelist<T1, T2>, 0>
{
	typedef typename NumTypeType<T1>::Type Result;
};

template<class T1, class T2, unsigned int i>
struct TypeAt<Typelist<T1, T2>, i>
{
	typedef typename TypeAt<T2, i-1>::Result Result;
};

template<int v>
struct Int2Type
{
	enum { value = v };
};

template<class T>
struct Type2Type
{
	typedef T OriginalType;
};

template<class T, class U>
class Conversion
{
	typedef char Small;
	class Big { char dummy[2]; };
	static Small Test(U);
	static Big Test(...);
	static T MakeT();
public:
	enum
	{
		exists = sizeof(Test(MakeT())) == sizeof(Small),
		sametype = 0
	};
};

template<class T>
class Conversion<T, T>
{
public:
	enum
	{
		exists = 1,
		sametype = 1
	};
};

template<bool flag, class T, class U>
struct Select
{
	typedef T Result;
};
template<class T, class U>
struct Select<false, T, U>
{
	typedef U Result;
};

template<bool>
struct CompileTimeError;
template<>
struct CompileTimeError<true> {};
#define STATIC_CHECK(expr, msg) \
{ CompileTimeError<((expr) != 0)> ERROR_##msg; (void)ERROR_##msg; }


template<int i=0, class T1=NullType, class T2=NullType, class T3=NullType,
         class T4=NullType, class T5=NullType, class T6=NullType,
         class T7=NullType, class T8=NullType, class T9=NullType,
         class T10=NullType, class T11=NullType, class T12=NullType>
struct _MakeNTypelist
{
	typedef Typelist<NumType<T1, i>, typename _MakeNTypelist<i+1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::Result> Result;
};

template<int i>
struct _MakeNTypelist<i>
{
	typedef NumType<NullType, i> Result;
};

template<class T1=NullType, class T2=NullType, class T3=NullType,
         class T4=NullType, class T5=NullType, class T6=NullType,
         class T7=NullType, class T8=NullType, class T9=NullType,
         class T10=NullType, class T11=NullType, class T12=NullType>
struct MakeNTypelist
{
	typedef typename _MakeNTypelist<0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::Result Result;
};

template<class, class>
struct Tag;

template<class T>
struct TupleUnit : public T
{
	typedef T Type;
	enum { index = -1 };
};

template<class T, unsigned int i>
struct TupleUnit<NumType<T, i> > : public T
{
	typedef T Type;
	enum { index = i };
};

template <class TList>
class Tuple;

template <class T1, class T2>
class Tuple<Typelist<T1, T2> >
	: public Tuple<Tag<T1, T2> >
	, public Tuple<T2>
{
public:
	typedef Typelist<T1, T2> TList;
	typedef Tuple<Tag<T1, T2> > LeftBase;
	typedef Tuple<T2> RightBase;
};

template<class T1, class T2>
class Tuple<Tag<T1,T2> > : public Tuple<T1>
{
};

template <class AtomicType>
class Tuple : public TupleUnit<AtomicType>
{
public:
	typedef TupleUnit<AtomicType> LeftBase;
};

template<>
class Tuple<NullType>
{
};
/*
template <class TList>
class NTuple;

template <template<class, unsigned int> class NT, class T1, class T2, unsigned int i>
class NTuple<Typelist<NT<T1, i>, T2> >
	: public NTuple<Tag<NT<T1, i>, T2> >
	, public NTuple<T2>
{
public:
	typedef Typelist<T1, T2> TList;
	typedef NTuple<Tag<NT<T1, i>, T2> > LeftBase;
	typedef NTuple<T2> RightBase;
};

template<class T1, class T2>
class NTuple<Tag<T1,T2> > : public NTuple<T1>
{
};

template <class AtomicType, unsigned int i>
class NTuple<NumType<AtomicType, i> > : public TupleUnit<AtomicType, i>
{
public:
	typedef TupleUnit<AtomicType, i> LeftBase;
};

template<unsigned int i>
class NTuple<NumType<NullType, i> >
{
};

template<class TList>
class NTuple
{
	NTuple()
	{
		assert(0);
	}
};
*/
template<class H, unsigned int i> struct FieldHelper;

template<class H>
struct FieldHelper<H, 0>
{
	typedef typename TypeAt<typename H::TList, 0>::Result ResultType;
	static inline ResultType& Do(H& obj)
	{
		typename H::LeftBase& subobj = obj;
		return subobj;
	}
};

template<class H, unsigned int i>
struct FieldHelper
{
	typedef typename TypeAt<typename H::TList, i>::Result ResultType;
	static inline ResultType& Do(H& obj)
	{
		typename H::RightBase& subobj = obj;
		return FieldHelper<typename H::RightBase, i-1>::Do(subobj);
	}
};

template<unsigned int i, class H>
typename FieldHelper<H, i>::ResultType& Field(H& obj)
{
	return FieldHelper<H, i>::Do(obj);
}

#endif

