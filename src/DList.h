#include "iMenu.h"
#include "links_nodes.h"

template<class T>
class _DList : public DLinked
{
	DLinked *cur;
protected:
	T *root;
public:
	_DList() : root(0)
	{
		this->r = this->l = this->cur = this;
	}
	int doStaff(int n, std::istream &in, std::ostream &out)
	{
		DLinked *t;
		T *v;
		if(cur == this) // empty
		{
			switch(n)
			{
			case 2:
			case 3: // adding after and before current is equivalent
				out << "warning: list was empty\n";
				cur = v = new T;
				v->init(in);
				this->r = this->l = cur;
				cur->r = cur->l = this;
				break;
			default:
				out << "error: list is empty\n";
				break;
			}
		}
		else
		{
			v = (T*)cur;
			switch(n)
			{
			case 1: // print current
				v->print(out);
				break;
			case 2: // add after current
				v = new T;
				v->init(in);
				v->r = cur->r;
				v->l = cur;
				cur->r->l = v;
				cur->r = v;
				break;
			case 3: // add before current
				v = new T;
				v->init(in);
				v->l = cur->l;
				v->r = cur;
				cur->l->r = v;
				cur->l = v;
				break;
			case 4: // delete after current
				if(cur->r == this)
				{
					out << "at the end, can't delete next\n";
					break;
				}
				t = cur->r;
				t->r->l = cur;
				cur->r = t->r;
				delete t;
				break;
			case 5: // delete before current
				if(cur->l == this)
				{
					out << "at the beginning, can't delete previous\n";
					break;
				}
				t = cur->l;
				t->l->r = cur;
				cur->l = t->l;
				delete t;
				break;
			case 6: // delete current
				cur->r->l = cur->l;
				cur->l->r = cur->r;
				t = cur->r;
				delete cur;
				cur = cur->r;
				if(cur == this) cur = cur->l;
				break;
			case 7: // increment current
				if(cur->r == this)
				{
					out << "at the end, can't go further\n";
					break;
				}
				cur = cur->r;
				break;
			case 8: // decrement current
				if(cur->l == this)
				{
					out << "at the beginning, can't go further\n";
					break;
				}
				cur = cur->l;
				break;
			case -4: // go deeper
				v->menu(in, out);
				break;
			}
		}
		if(this->l == this) root = 0;
		else root = reinterpret_cast<T*>(this->l);
		return 0;
	}
	const char* getHelp()
	{
		return " 1: print current\n 2: add after current\n 3: add before current\n 4: delete after current\n 5: delete before current\n 6: delete current\n 7: increment current\n 8: decrement current\n-4: go deeper\n";
	}
	void print(std::ostream &out)
	{
		for(DLinked *t = this->r; t != this; t = t->r)
		{
			reinterpret_cast<T*>(t)->print(out);
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
				t->l = this->l;
				this->l->r = t;
				t->r = this;
				this->l = t;
				cur = t;
			}
			if(this->l == this) root = 0;
			else root = reinterpret_cast<T*>(this->l);
		}
	}
	~_DList()
	{
		cur = this->r;
		while(cur != this)
		{
			cur = cur->r;
			delete cur->l;
		}
	}
};

template<class T>
struct DList
{
	typedef iMenu<_DList<DLinkedNode<T> > > Tp;
};

