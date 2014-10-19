#include "iMenu.h"
#include "links_nodes.h"

template<class T, int v>
class _Stack : public Container<T, v>
{
protected:
	T *root;
public:
	_Stack() : root(0) {}
	int doStaff(int n, std::istream &in, std::ostream &out)
	{
		T *t = root;
		switch(n)
		{
		case 1: // print top
			if(!root)
			{
				out << "empty\n";
				break;
			}
			root->print(out);
			break;
		case 2: // push
			t = new T;
			t->init(in);
			t->r = root;
			root = t;
			break;
		case 3: // pop
			if(!root)
			{
				out << "empty\n";
				break;
			}
			t = root->r;
			delete root;
			root = t;
			break;
		case -4: // go deeper
			if(!root)
			{
				out << "empty\n";
				break;
			}
			root->menu(in, out);
			break;
		}
		return 0;
	}
	const char* getHelp()
	{
		return " 1: print top\n 2: push\n 3: pop\n-4: go deeper\n";
	}
	void print(std::ostream &out)
	{
		for(T *t = root; t; t = t->r)
		{
			t->print(out);
		}
	}
	void init(std::istream &in, int *size = 0)
	{
		if(size)
		{
			T *t;
			for(int i=0; i < *size; i++)
			{
				t = new T;
				t->init(in, size+1);
				t->r = root;
				root = t;
			}
		}
	}
	~_Stack()
	{
		T *t = root;
		while(root)
		{
			t = root->r;
			delete root;
			root = t;
		}
	}
};

template<class T>
struct Stack
{
	typedef iMenu<_Stack<SLinkedNode<T> > > Tp;
};

