#ifndef __AVL_MISC_H
#define __AVL_MISC_H 1

#include "links_nodes.h"
#include <cassert>
#include <algorithm>

template<class T>
void swap3(T &a, T &b, T &c)
{
	T t(a);
	a = b;
	b = c;
	c = t;
}

template<class T>
void swap3_2(T a, T &b, T &c)
{
	b = c;
	c = a;
}

struct AVLinked
{
	AVLinked *r;
	AVLinked *l;
	AVLinked *p;
	AVLinked *coll;
	int iter;
	int bal;
	int dead;
	AVLinked() : r(0), l(0), p(0), iter(0), bal(0), dead(0) {}
	template<class T>
	AVLinked(T a);
	AVLinked* rotate_right()
	{
		assert(l);
		AVLinked* a2 = this;
		AVLinked*& P = (p ? ((p->l == this) ? p->l : p->r) : a2);
		swap3(P, l, l->r);
		if(l) swap3(l->p, P->p, p);
		else swap3_2(P, P->p, p);
		P->fix_coll();
		if(P->l) P->l->fix_coll();
		fix_coll();
		if(l) l->fix_coll();
		if(r) r->fix_coll();
		return P;
	}
	
	AVLinked* rotate_left()
	{
		assert(r);
		AVLinked* a2 = this;
		AVLinked*& P = (p ? ((p->l == this) ? p->l : p->r) : a2);
		swap3(P, r, r->l);
		if(r) swap3(r->p, P->p, p);
		else swap3_2(P, P->p, p);
		P->fix_coll();
		if(P->r) P->r->fix_coll();
		fix_coll();
		if(l) l->fix_coll();
		if(r) r->fix_coll();
		return P;
	}
	void fix_coll()
	{
		AVLinked *a = coll;
		while(a)
		{
			a->r = r;
			a->l = l;
			a->p = p;
			a->bal = bal;
			a = a->coll;
		}
	}
	void swap(AVLinked *a)
	{
		if(a == this) return;
		std::swap(r, a->r);
		std::swap(l, a->l);
		std::swap(p, a->p);
		std::swap(bal, a->bal);
		if(a->r == a) std::swap(a->r, p);
		else if(a->l == a) std::swap(a->l, p);
		else if(l == this) std::swap(l, a->p);
		else if(r == this) std::swap(r, a->p);
		if(r && r->p == a) r->p = this;
		if(l && l->p == a) l->p = this;
		if(p)
		{
			if(p->r == a) p->r = this;
			else if(p->l == a) p->l = this;
		}
		if(a->r && a->r->p == this) a->r->p = a;
		if(a->l && a->l->p == this) a->l->p = a;
		if(a->p)
		{
			if(a->p->r == this) a->p->r = a;
			else if(a->p->l == this) a->p->l = a;
		}
		assert(l != p);
		assert(a->l != a->p);
		a->fix_coll();
		fix_coll();
	}
};

#endif // __AVL_MISC_H
