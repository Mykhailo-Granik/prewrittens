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

using namespace std;

#define ABS(a) ((a>0)?a:-(a))
#define MIN(a,b) ((a<b)?(a):(b))
#define MAX(a,b) ((a<b)?(b):(a))
#define FOR(i,a,n) for (int i=(a);i<(n);++i)
#define FI(i,n) for (int i=0; i<(n); ++i)
#define pnt pair <int, int>
#define mp make_pair
#define PI 3.1415926535897
#define MEMS(a,b) memset(a,b,sizeof(a))
#define LL long long
#define U unsigned

struct edge
{
	int u, v, cap;
	edge()
	{
		u = v = cap = 0;
	}
	edge(int u, int v, int cap)
	{
		this->u = u;
		this->v = v;
		this->cap = cap;
	}
};

vector<edge> e;
vector<vector<int> > g;
vector<int> ptr;
vector<int> d;

void addedge(int u, int v, int cap)
{
	edge e1 = *new edge(u, v, cap);
	g[u].push_back(e.size());
	e.push_back(e1);
	edge e2 = *new edge(v, u, 0);
	g[v].push_back(e.size());
	e.push_back(e2);
}

int dfs(int v, int finish, int have)
{
	if (v == finish)
	{
		return have;
	}
	for (; ptr[v] < g[v].size(); ++ptr[v])
	{
		int to = g[v][ptr[v]];
		int ne = e[to].v;
		if ((e[to].cap>0) && (d[v]+1==d[ne]))
		{
			int res = 0;
			if (have == -1)
				res = dfs(e[to].v, finish, e[to].cap);
			else
				res = dfs(e[to].v, finish, MIN(have, e[to].cap));
			if (res > 0)
			{
				e[to].cap -= res;
				e[to ^ 1].cap += res;
				return res;
			}
		}
	}
	return 0;
}
vector<int> q;
bool bfs(int start, int finish, int n)
{
	d.clear();
	d.resize(n, -1);
	q.clear();
	q.push_back(start);
	d[start] = 0;
	int l = 0;
	while (l < q.size())
	{
		int v = q[l++];
		FOR(i, 0, g[v].size())
		{
			int to = g[v][i];
			if ((e[to].cap > 0) && (d[e[to].v] == -1))
			{
				d[e[to].v] = d[v] + 1;
				q.push_back(e[to].v);
			}
		}
	}
	return (d[finish] != -1);
}

LL getFlow(int start, int finish, int n)
{
	LL res = 0;
	while (true)
	{
		bool f = bfs(start, finish, n);
		if (!f)
			break;
		ptr.clear();
		ptr.resize(n, 0);
		while (1)
		{
			int k = dfs(start, finish, -1);
			if (k <= 0)
				break;
			res += k;
		}
	}
	return res;
}


int main()
{
#ifdef Fcdkbear
	freopen("in.txt", "r", stdin);
	double beg = clock();
	//freopen("out.txt", "w", stdout);
#endif

	int n,m;
	cin>>n>>m;
	g.clear();
	e.clear();
	g.resize(n);
	FOR(i,0,m)
	{
		int a,b,c;
		scanf("%d%d%d",&a,&b,&c);
		a--;
		b--;
		addedge(a,b,c);
		addedge(b,a,c);
	}
	LL res=getFlow(0,n-1,n);
	cout<<res<<endl;


#ifdef Fcdkbear
	double end = clock();
	fprintf(stderr, "*** Total time = %.3lf ***\n", (end - beg) / CLOCKS_PER_SEC);
#endif
	return 0;
}

