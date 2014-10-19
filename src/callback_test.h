#include <iostream>
#include <list>
#include <tuple>
#include <functional>
/*
template<bool enable, class T>
struct enable_if {};

template<class T>
struct enable_if<true, T>
{
	typedef T type;
};

template<size_t argIndex, size_t argSize, class F, class... Args, class... Unpacked>
inline typename enable_if<(argIndex == argSize), void>::type
applyImpl(F f, const std::tuple<Args...>&& t, Unpacked&&... u)
{
	f(std::forward<Unpacked> (u)...);
}

template<size_t argIndex, size_t argSize, class F, class... Args, class... Unpacked>
inline typename enable_if<(argIndex < argSize), void>::type
applyImpl(F f, const std::tuple<Args...>&& t, Unpacked&&... u)
{
	applyImpl<argIndex + 1, argSize>(f, t, u..., std::get<argIndex>(t));
}

template<class F, class... Args>
inline void apply(F f, const std::tuple<Args...>&& t)
{
	applyImpl<0, sizeof...(Args)>(f, t);
}

template<class Func, class... Args>
struct _Callback
{
	Func func;
	std::tuple<Args&&...>&& args;
	_Callback(Func f, Args&&... a) : func(f), args(std::forward_as_tuple(a...)) {}
	typename std::result_of<Func(Args...)>::type call()
	{
		return apply(func, std::forward<std::tuple<Args&&...>&&>(args));
	}
};

template<class Func, class... Args>
_Callback<Func, Args...> Callback(Func func, Args&&... args)
{
	return _Callback<Func, Args...>(func, args&&...);
};
*/


template<class T>
class _Callback
{
public:
	T cb;
	_Callback(T&& c) : cb(c) {}
	typename T::result_type call()
	{
		return cb();
	}
};


template<class Func, class... Args>
auto Callback(Func f, Args... a)
-> _Callback<decltype(std::bind(f, a...))>
{
	return _Callback<decltype(std::bind(f, a...))>(std::bind(f, a...));
}

int f(int x)
{
	return x+3;
}

