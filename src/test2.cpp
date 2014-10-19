#include <iostream>
#include <string>
using namespace std;

#include "links_nodes.h"
#include "avl_misc.h"
#include "Record.h"

struct Test;

struct Test : public TLinked<Test>
{
	string name;
	Test(string s) : name(s) {}
	friend ostream& operator<<(ostream&, Test&);
};

ostream& operator<<(ostream& out, Test& t)
{
	out << t.name << " l=" << (t.l ? t.l->name : "0") << ", r=" << (t.r ? t.r->name : "0") << ", p=" << (t.p ? t.p->name : "0")<< endl;
	return out;
}
int main()
{
	Test *D = new Test("D");
	Test *P = new Test("P");
	Test *Q = new Test("Q");
	Test *A = new Test("A");
	Test *B = new Test("B");
	Test *C = new Test("C");
	// left
/*	D->r = Q;
	Q->l = P;
	Q->r = C;
	Q->p = D;
	P->l = A;
	P->r = B;
	P->p = Q;
	A->p = P;
	B->p = P;
	C->p = Q;
	// right
	D->r = P;
	Q->l = B;
	Q->r = C;
	Q->p = P;
	P->l = A;
	P->r = Q;
	P->p = D;
	A->p = P;
	B->p = Q;
	C->p = Q;
	// left min
	D->r = Q;
	Q->l = P;
	Q->p = D;
	P->p = Q;
	// right min
	D->r = P;
	Q->p = P;
	P->r = Q;
	P->p = D;
	// left min2
	Q->l = P;
	P->p = Q;
*/	// right min2
	Q->p = P;
	P->r = Q;
	cout << *D << *P << *Q << *A << *B << *C << endl;
	//rotate_left(Q);
	rotate_right(P);
	cout << *D << *P << *Q << *A << *B << *C << endl;
	delete D;
	delete P;
	delete Q;
	delete A;
	delete B;
	delete C;
	return 0;
}

