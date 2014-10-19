#include "links_nodes.h"
#include "avl_misc.h"
#include <cassert>
#include <iostream>

template<class T, int ikey, int ivalue>
class AVLTree : public Index<T, ikey, ivalue>
{
	using Index<T, ikey, ivalue>::prvalue;
	typedef typename Index<T, ikey, ivalue>::ValueType ValueType;
protected:
	AVLinked *root;
public:
	AVLTree() : root(0) {}
	/*int doStaff(int n, std::istream &in, std::ostream &out)
	{
		if(!cur) // empty
		{
			switch(n)
			{
				break;
			case 2:
			case 3: // adding to current anyway
				out << "warning: tree was empty\n";
				root = cur = new T;
				cur->init(in);
				break;
			default:
				out << "error: tree is empty\n";
				break;
			}
		}
		else
		{
			switch(n)
			{
			case 1: // print current
				cur->print(out);
//				out << "tree_print  " << cur << ' ' << cur->p << ' ' << cur->l << ' ' << cur->r << '\n';
				break;
			case 2: // add to left
				if(cur->l)
				{
					out << "left is occupied, can't add\n";
					break;
				}
				cur->l = new T;
				cur->l->init(in);
				cur->l->p = cur;
				break;
			case 3: // add to right
				if(cur->r)
				{
					out << "right is occupied, can't add\n";
					break;
				}
				cur->r = new T;
				cur->r->init(in);
				cur->r->p = cur;
				break;
			case 4: // delete left
				if(!cur->l)
				{
					out << "left is empty, can't delete\n";
					break;
				}
				delete cur->l;
				cur->l = 0;
				break;
			case 5: // delete right
				if(!cur->r)
				{
					out << "right is empty, can't delete\n";
					break;
				}
				delete cur->r;
				cur->r = 0;
				break;
			case 6: // go left
				if(!cur->l)
				{
					out << "no left child\n";
					break;
				}
				cur = cur->l;
				break;
			case 7: // go right
				if(!cur->r)
				{
					out << "no right child\n";
					break;
				}
				cur = cur->r;
				break;
			case 8: // go up
				if(!cur->p)
				{
					out << "at the top\n";
					break;
				}
				cur = cur->p;
				break;
			case -4: // go deeper
				cur->menu(in, out);
				break;
			}
		}
		return 0;
	}
	const char* getHelp()
	{
		return "1: print current\n 2: add to left\n 3: add to right\n 4: delete left\n 5: delete right\n 6: go left\n 7: go right\n 8: go up\n-4: go deeper\n";
	}*/
	void print(std::ostream &out)
	{
		AVLinked *s = 0, *t = root;
//		out << "test\n";
		while(t)
		{
			if(t->p == s && t->l) // from top
			{
				s = t;
				t = t->l;
				continue;
			}
			if(t->r != s || !s) // between left and right
			{
				rkey<T, ikey>(t)->print(out);
//				out << "tree_print2 " << t << ' ' << t->p << ' ' << t->l << ' ' << t->r << '\n';
				if(t->r)
				{
					s = t;
					t = t->r;
					continue;
				}
			}
			// been everywhere
			s = t;
			t = t->p;
		}
	}
	void init(std::istream &in, int size = 0)
	{
		if(size)
		{
			AVLinked *cur, **s;
			T *t;
			for(int i=0; i < size; i++)
			{
				t = new T;
				in >> value(*t);
				find(value(*t), cur, s);
				*s = pkey(t);
				(*s)->p = cur;
			}
			cur = root;
		}
	}
	void add_dumb(T* record)
	{
		pkey(record)->r = root;
		root = pkey(record);
	}
	void add(T* record)
	{
		AVLinked *t, **s;
		find(value(*record), t, s);
		if(*s) // collision
		{
			std::cerr << "Collision! data: " << value(*record) << std::endl;
			return; // TODO
		}
		*s = pkey(record);
		(*s)->p = t;
		if(t)
		{
			t->bal += ((*s == t->l) ? -1 : 1);
			fix_balance(t);
		}
	}
	void find(const ValueType &v, AVLinked *&t, AVLinked **&s)
	{
		t = 0;
		s = &root;
		while(*s)
		{
			t = *s;
			if(v < prvalue(*s))
			{
				s = &((*s)->l);
			}
			else if(prvalue(*s) < v)
			{
				s = &((*s)->r);
			}
			else return; // found
		}
	}
	void remove(T* record);
	void fix_balance(AVLinked *a)
	{
		for(int flag=1; a->p && flag; a = a->p)
		{
			flag = 0;
			if(a->bal == -2)
			{
				if(a->l->bal != 1)
				{
					a = a->rotate_left();
					flag = !(a->l->bal);
					a->bal = - (++a->l->bal);
				}
				else
				{
					a->l->rotate_right();
					a = a->rotate_left();
					a->r->bal = -max(a->bal, 0);
					a->l->bal = -min(a->bal, 0);
					a->bal = 0;
					flag = 1;
				}
			}
			else if(a->bal == 2)
			{
				if(a->r->bal != -1)
				{
					a = a->rotate_right();
					flag = !(a->r->bal);
					a->bal = - (--a->r->bal);
				}
				else
				{
					a->r->rotate_left();
					a = a->rotate_right();
					a->l->bal = -max(a->bal, 0);
					a->r->bal = -min(a->bal, 0);
					a->bal = 0;
					flag = 1;
				}
			}
			if(flag)
			{
				a->p->bal += ((a == a->p->l) ? -1 : 1);
			}
		}
	}
	/*~Tree()
	{
		cur = root;
		while(cur)
		{
			root = cur;
			if(pkey(cur)->l)
			{
				cur = pkey(cur)->l;
				pkey(root)->l = 0;
			}
			else if(pkey(cur)->r)
			{
				cur = pkey(cur)->r;
				pkey(root)->r = 0;
			}
			else
			{
				cur = pkey(cur)->p;
				delete root;
			}
		}
	}*/
};

