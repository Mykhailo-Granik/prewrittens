#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <queue>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <memory.h>
#include <ctime>
#include <bitset>
#include <unordered_map>

using namespace std;

#define ABS(a) ((a>0)?a:-(a))
#define MIN(a,b) ((a<b)?(a):(b))
#define MAX(a,b) ((a<b)?(b):(a))
#define FOR(i,a,n) for (int i=(a);i<(n);++i)
#define MEMS(a,b) memset((a),(b),sizeof(a))
#define FI(i,n) for (int i=0; i<(n); ++i)
#define pnt pair <int, int>
#define mp make_pair
#define LL long long

struct item
{
	item* l;
	item* r;
	char c;
	int sz;
	int y;
	char rev;
	item(char c)
	{
		this->c = c;
		sz = 1;
		rev = 0;
		l = r = 0;
		y = rand();
	}
};

typedef item* pitem;

inline int getSz(pitem l) {
	if (!l)
		return 0;
	return l->sz;
}

inline void recalc(pitem& v)
{
	v->sz = 1 + getSz(v->l) + getSz(v->r);
}

inline void push(pitem &v)
{
	if (!v)
		return;
	if (v->rev) {
		v->rev = 0;
		if (v->l)
			v->l->rev^=1;
		if (v->r)
			v->r->rev^=1;
		swap(v->l, v->r);
	}
}

pitem merge(pitem l, pitem r)
{
	push(l);
	push(r);
	if (!l)
		return r;
	if (!r)
		return l;
	if (l->y>r->y)
	{
		l->r = merge(l->r, r);
		recalc(l);
		return l;
	}
	r->l = merge(l, r->l);
	recalc(r);
	return r;
}

void split(pitem v, pitem&l, pitem&r, int x)
{
	push(v);
	if (!v)
	{
		l = r = 0;
		return;
	}
	if (getSz(v->l) >= x) {
		split(v->l, l, v->l,x);
		r = v;
		recalc(r);
	}
	else
	{
		split(v->r,v->r,r,x-getSz(v->l)-1);
		l = v;
		recalc(l);
	}
}

void traverse(pitem v)
{
	push(v);
	if (!v)
		return;
	traverse(v->l);
	printf("%c",v->c);
	traverse(v->r);
}

char s[200100];
pitem t,t1,t2;

int main()
{
#ifdef Fcdkbear
	freopen("in.txt", "r", stdin);
	double beg = clock();
	//freopen("out.txt", "w", stdout);
#endif

	gets(s);
	t = 0;
	int len = strlen(s);
	FOR(i,0,len)
	{
		t = merge(t, new item(s[i]));
	}
	int q;
	scanf("%d",&q);
	FOR(i,0,q)
	{
		int x;
		scanf("%d",&x);
		split(t, t1, t2, x);
		if (t1)
			t1->rev^=1;
		if (t2)
			t2->rev^=1;
		t = merge(t1,t2);
	}
	traverse(t);
	printf("\n");


#ifdef Fcdkbear
	double end = clock();
	//fprintf(stderr, "*** Total time = %.3lf ***\n", (end - beg) / CLOCKS_PER_SEC);
#endif
	return 0;
}
