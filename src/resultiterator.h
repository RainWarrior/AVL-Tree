#include "String.h"

class ResultIterator
{
public:
	virtual String* next()
	{
		return 0;
	}
	virtual ~ResultIterator() {};
};

class Pong : public ResultIterator
{
	int empty;
public:
	Pong() : empty(0) {};
	virtual String* next()
	{
		if(!empty)
		{
			empty = 1;
			return new String("pong!");
		}
		return 0;
	}
	virtual ~Pong() {};
};

