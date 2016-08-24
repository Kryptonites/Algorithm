#include <iostream>  
#include <algorithm>  
#include <cstring>  
#include <string>  


#define MAXN 55555  

using namespace std;

int mi[MAXN][17], mx[MAXN][17], w[MAXN];
int n, q;
void rmqinit()
{
	for (int i = 1; i <= n; i++) mi[i][0] = mx[i][0] = w[i];
	int m = (int)(log(n * 1.0) / log(2.0));
	for (int i = 1; i <= m; i++)
		for (int j = 1; j <= n; j++)
		{
			mx[j][i] = mx[j][i - 1];
			if (j + (1 << (i - 1)) <= n) mx[j][i] = max(mx[j][i], mx[j + (1 << (i - 1))][i - 1]);
			mi[j][i] = mi[j][i - 1];
			if (j + (1 << (i - 1)) <= n) mi[j][i] = min(mi[j][i], mi[j + (1 << (i - 1))][i - 1]);
		}
}
int rmqmin(int l, int r)
{
	int m = (int)(log((r - l + 1) * 1.0) / log(2.0));
	return min(mi[l][m], mi[r - (1 << m) + 1][m]);
}
int rmqmax(int l, int r)
{
	int m = (int)(log((r - l + 1) * 1.0) / log(2.0));
	return max(mx[l][m], mx[r - (1 << m) + 1][m]);
}
int main()
{
	scanf_s("%d%d", &n, &q);
	for (int i = 1; i <= n; i++) scanf_s("%d", &w[i]);
	rmqinit();
	int l, r;
	while (q--)
	{
		scanf_s("%d%d", &l, &r);
		printf("%d\n", rmqmax(l, r));
	}
	return 0;
}