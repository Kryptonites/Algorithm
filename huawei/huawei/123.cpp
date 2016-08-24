#include<iostream> 
using namespace std;

int main()
{
	int N, M;
	char c;
	int A, B;
	char co;
	int k;
	while (cin >> N >> M)
	{
		//学生数目和操作数目 
		int *cj = new int[N + 1];
		int len = N + 1;
		cj[0] = -1;
		for (int i = 1; i <= N; i++)
			cin >> cj[i];
		while (M--)
		{
			int max = -1;
			cin >> c;
			cin >> A >> B;
			if ('Q' == c)
			{
				if (A > B)
				{
					int tmp = A; A = B; B = tmp;
				}
				for (int j = A; j <= B; ++j)
				{
					max = cj[j] > max ? cj[j] : max;
				}
				cout << max << endl;
			}
			else
			{
				cj[A] = B;
			}
		}
		delete[] cj;
	}
	return 0;
}