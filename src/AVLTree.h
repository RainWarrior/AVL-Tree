#ifndef __AVLTREE_H
#define __AVLTREE_H 1

#include "links_nodes.h"
#include "predicate.h"
#include "avl_misc.h"
#include <cassert>
#include <iostream>

template<class T, unsigned int ikey>
void dprint(T *r, std::ostream &out)
{
	out << Field<6>(*r) << ' ' << Field<5>(*r) << ' ' << Field<4>(*r) << std::endl;
	AVLinked *a = &Field<ikey>(*r);
	out << "{ this=" << a << ", p=" << a->p << ", l=" << a->l << ", r=" << a->r << ", coll=" << a->coll << " " << a->iter << " " << a->dead << "}" << std::endl;
}

template<class T, unsigned int ikey, unsigned int ivalue>
class AVLTree : public Index<T, ikey, ivalue>
{
	using Index<T, ikey, ivalue>::prvalue;
	typedef typename Index<T, ikey, ivalue>::ValueType ValueType;
protected:
	AVLinked *root;
public:
	class iterator
	{
		AVLinked *data;
		AVLinked *cdata;
	public:
		iterator(AVLinked *cur) : data(cur), cdata(cur) {}
		void operator++()
		{
			if(cdata && cdata->coll)
			{
				cdata = cdata->coll;
				return;
			}
			if(data->r)
			{
				data = data->r;
				while(data->l) data = data->l;
				cdata = data;
				return;
			}
			while(data->p && data->p->l != data) data = data->p;
			if(data->p) data = data->p;
			else data = 0;
			cdata = data;
		}
		iterator operator++(int)
		{
			iterator iter(*this);
			++(*this);
			return iter;
		}
		AVLinked* operator*()
		{
			return cdata;
		}
		AVLinked* operator->()
		{
			return cdata;
		}
		int operator==(const iterator &it)
		{
			return cdata == it.cdata;
		}
		int operator!=(const iterator &it)
		{
			return cdata != it.cdata;
		}
		operator AVLinked*()
		{
			return cdata;
		}
	};
	class AVLResultIterator : public ResultIterator
	{
		AVLTree *tree;
		ValueType min, max;
		iterator cur;
		int fired;
		BasePredicate<T> *pred;
		int format;
		int del;
	public:
		AVLResultIterator(AVLTree *tr, const ValueType &mn, const ValueType &mx, BasePredicate<T> *p, int f, int d)
			: tree(tr)
			, min(mn)
			, max(mx)
			, cur(0)
			, fired(0)
			, pred(p)
			, format(f)
			, del(d)
		{
			if(pred) pred->count++;
		}
		virtual String* next()
		{
			iterator oldcur = 0;
			String *s = 0;
			do
			{
				if(!fired)
				{
					fired = 1;
					AVLinked *t, **s;
					tree->find(min, t, s);
					if(*s) cur = *s;
					else if(t)
					{
						cur = t;
						if(prvalue(cur) < min) ++cur;
					}
					else cur = 0;
				}
				else if(cur)
				{
					cur->iter--;
					oldcur = cur;
					++cur;
				}

				while(cur && pred && !(*pred)(*rkey<T, ikey>((AVLinked*)cur))) ++cur;
				if(cur && max < prvalue(cur)) cur = 0;
				if(cur)
				{
					if(!del) s = rkey<T, ikey>((AVLinked*)cur)->format(format);
					else cur->dead = 1;
					if(cur) cur->iter++;
				}
				if(oldcur && oldcur->dead && !(oldcur->iter))
				{
//					dprint<T, ikey>(rkey<T, ikey>((AVLinked*)oldcur), std::cerr);
//					if(cur) dprint<T, ikey>(rkey<T, ikey>((AVLinked*)cur), std::cerr);
					tree->remove(rkey<T, ikey>((AVLinked*)oldcur));
//					if(cur) dprint<T, ikey>(rkey<T, ikey>((AVLinked*)cur), std::cerr);
//					std::cerr << std::endl;
					oldcur = 0;
					//if(del == 2) delete rkey<T, ikey>((AVLinked*)oldcur);
				}
			}
			while(del && (cur || oldcur));
			return s;
		}
		virtual ~AVLResultIterator()
		{
			if(pred && !--(pred->count)) delete pred;
		}
	};
	AVLTree() : root(0) {}
	void print(std::ostream &out)
	{
		AVLinked *s = 0, *t = root;
//		out << "test\n";
		while(t)
		{
			assert(!(t->l) || t->l->p == t);
			assert(!(t->r) || t->r->p == t);
			assert(!(t->p) || (t->p->r == t || t->p->l == t));
			if(t->p == s && t->l) // from top
			{
				s = t;
				t = t->l;
				continue;
			}
			if(t->r != s || !s) // between left and right
			{
				AVLinked *c = t;
				while(c)
				{
//					rkey<T, ikey>(c)->print(out);
					dprint<T, ikey>(rkey<T, ikey>(c), out);
					c = c->coll;
				}
//				out << "tree_print2 \t" << t->bal << '\t' << hex << int(t) << '\t' << int(t->p) << '\t' << int(t->l) << '\t' << int(t->r) << endl << dec;
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
		//record->print(cerr);
		AVLinked *t, **s;
		find(value(*record), t, s);
		if(*s) // collision
		{
			//std::cerr << "Collision! id: " << value(*record) << std::endl;
			key(*record).coll = (*s)->coll;
			(*s)->coll = pkey(record);
			(*s)->fix_coll();
			return;
		}
		*s = pkey(record);
		(*s)->p = t;
		if(t)
		{
			t->bal += ((*s == t->l) ? -1 : 1);
			fix_balance(t, 1);
		}
//		check_balance(root);
	}
	void find(const ValueType &v, AVLinked *&t, AVLinked **&s, int relative=0)
	{
		if(!relative)
		{
			t = 0;
			s = &root;
		}
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
	void remove(T* record)
	{
		AVLinked *t, **s;
		find(value(*record), t, s);
//		if(t) cout << "tree_print3 " << hex << int(t) << '\t' << int(t->p) << '\t' << int(t->l) << '\t' << int(t->r) << endl << dec;
		AVLinked *c = *s;
		if(pkey(record) != c)
		{
			while(c->coll != pkey(record)) c = c->coll;
			if(!c->coll)
			{
				std::cerr << "Failed to find record, id: " << value(*record) << std::endl;
				return;
			}
			c->coll = c->coll->coll;
			return;
		}
		else if(c->coll)
		{
			//key(*rkey<T, ikey>(c->r)) = **s;
			t = *s;
			t->fix_coll();
			c->coll->fix_coll();
//			std::cerr << '!' << ikey << ' ' << ivalue << '!';
//			dprint<T, ikey>(rkey<T, ikey>(t), std::cerr);
//			std::cerr << std::endl;
//			dprint<T, ikey>(rkey<T, ikey>(c->coll), std::cerr);
//			std::cerr << std::endl;

			t->swap(c->coll);
			c->coll->fix_coll();
			if(root == t) root = c->coll;

			//*s = pkey(rkey<T, ikey>(c->r));
//			std::cerr << '?' << ikey << ' ' << ivalue << '?';
//			dprint<T, ikey>(rkey<T, ikey>(t), std::cerr);
//			std::cerr << std::endl;
//			dprint<T, ikey>(rkey<T, ikey>(c->coll), std::cerr);
//			std::cerr << std::endl;
			return;
		}
		// no other objects with this value
		t = (*s)->p;
//		std::cerr << "WOOHOO" << endl;
		while(*s)
		{
//			rkey<T, ikey>(*s)->print(std::cerr);
//			std::cerr << endl;
			if(!((*s)->l || (*s)->r)) // leaf
			{
				if(t) t->bal += ((*s == t->l) ? 1 : -1);
				*s = 0;
				fix_balance(t, -1);
				break;
			}
			else if((*s)->l && (*s)->r) // both
			{
				AVLinked *r=(*s), **s2=&((*s)->r);
				find(value(*record), r, s2, 1); // should go to the leftmost, *s2 = 0
//				cerr << "! r: " << r << ",*s: " << *s << ",t: " << t << endl;
				assert(r != (*s));
				assert(!*s2);
				assert(prvalue(r) > value(*record));
				t = *s;
				(*s)->swap(r);
				if(s == &root) root = r;
				s = ((t->p->l == t) ? &(t->p->l) : &(t->p->r));
				t = t->p;
				assert(!(t->l) || t->l->p == t);
				assert(!(t->r) || t->r->p == t);
				assert(!(t->p) || (t->p->r == t || t->p->l == t));
			}
			else if((*s)->l) // only left
			{
				(*s)->l->p = t;
				if(t) t->bal += ((*s == t->l) ? 1 : -1);
				*s = (*s)->l;
				fix_balance(t, -1);
				break;
			}
			else // only right
			{
				(*s)->r->p = t;
				if(t) t->bal += ((*s == t->l) ? 1 : -1);
				*s = (*s)->r;
				fix_balance(t, -1);
				break;
			}
		}
//		check_balance(root);
	}
	void fix_balance(AVLinked *a, int flag=0)
	{
//		cerr << "!a: " << a << endl;
		while(flag && a)
		{
//			cerr << "[" << flag << "],";
//			check_balance(a);
			if(flag < 0) flag = 0;
			if(a->bal == -2)
			{
//				cerr << "(-2," << a->l->bal << "),";
				if(a->l->bal != 1)
				{
					a = a->rotate_right();
					flag += (a->bal);
					a->r->bal = - (++a->bal);
				}
				else
				{
					a->l->rotate_left();
					a = a->rotate_right();
					a->l->bal = -std::max(a->bal, 0);
					a->r->bal = -std::min(a->bal, 0);
					a->bal = 0;
					flag--;
				}
			}
			else if(a->bal == 2)
			{
//				cerr << "(2," << a->r->bal << "),";
				if(a->r->bal != -1)
				{
					a = a->rotate_left();
					flag -= a->bal;
					a->l->bal = - (--a->bal);
				}
				else
				{
					a->r->rotate_right();
					a = a->rotate_left();
					a->r->bal = -std::min(a->bal, 0);
					a->l->bal = -std::max(a->bal, 0);
					a->bal = 0;
					flag--;
				}
			}
			else
			{
//				cerr << "(" << a->bal << "),";
				if(!a->bal) flag--;
			}
//			check_balance(a);
			if(flag && a->p)
			{
				a->p->bal += ((a == a->p->l) ? -flag : flag);
			}
			if(!(a->p)) root=a;
			a->fix_coll();
			a = a->p;
		}
//		cerr << endl;
	}
	int check_balance(AVLinked *a)
	{
		if(!a) return 0;
		int ll = check_balance(a->l);
		int lr = check_balance(a->r);
		assert(a->bal == lr - ll);
		return std::max(ll, lr) + 1;
	}
	int check_root()
	{
		return check_balance(root);
	}
	int _size(AVLinked *a)
	{
		if(!a) return 0;
		return 1 + _size(a->l) + _size(a->r);
	}
	int size()
	{
		return _size(root);
	}
/*	AVLinked* getminkey(const ValueType &v)
	{
		AVLinked *t, **s;
		find(v, t, s);
		if(s) return s;
		if(t)
		{
			if(prvalue(t)
		}
	}
	T* getmin(const ValueType &v)
	{
		return rkey<T, ikey>(getminkey(v));
	}
	AVLinked* getmaxkey(const ValueType &v)
	{
		if(!root) return 0;
		AVLinked *s = root;
		while(s->r) s = s->r;
		return s;
	}
	T* getmax()
	{
		return rkey<T, ikey>(getmaxkey());
	}*/
	AVLinked* getkey(const ValueType &v)
	{
		AVLinked *t, **s;
		find(v, t, s);
		return *s;
	}
	T* get(const ValueType &v)
	{
		return rkey<T, ikey>(getkey(v));
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

#endif // __AVLTREE_H
