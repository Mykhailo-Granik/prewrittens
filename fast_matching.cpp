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

int mt[150100];
int was[150100];
vector<vector<int> > g;
bool dfs(int v)
{
	if (was[v])
		return false;
	was[v] = 1;
	FOR(i, 0, g[v].size())
	{
		int to = g[v][i];
		if ((mt[to] == -1) || (dfs(mt[to])))
		{
			mt[v] = to;
			mt[to] = v;
			return true;
		}
	}
	return false;
}



int main()
{
#ifdef Fcdkbear
	freopen("in.txt", "r", stdin);
	double beg = clock();
	//freopen("out.txt", "w", stdout);
#endif

	int t;
	scanf("%d",&t);
	while (t--)
	{
		int n;
		cin>>n;
		FOR(i,0,3*n)
			was[i]=0;
		FOR(i,0,3*n)
			mt[i] = -1;
		g.clear();
		g.resize(n);
		FOR(i,0,n)
		{
			int v1,v2;
			scanf("%d%d",&v1,&v2);
			v1--;
			v2--;
			g[i].push_back(n+v1);
			g[i].push_back(n+v2);
		}
		while (1)
		{
			bool good = false;
			MEMS(was, 0);
			FOR(i,0,n)
			if ((mt[i] == -1) && (dfs(i)))
				good = true;
			if (!good)
				break;
		}
		int res = 0;
		FOR(i, 0, n)
			res += (mt[i] != -1);
		if (res == n)
		{
			printf("Yes\n");
		}
		else
		{
			printf("No\n");
		}
	}

#ifdef Fcdkbear
	double end = clock();
	fprintf(stderr, "*** Total time = %.3lf ***\n", (end - beg) / CLOCKS_PER_SEC);
#endif
	return 0;
}

