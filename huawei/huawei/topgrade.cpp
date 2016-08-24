#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;


class Grade
{
public:
	int GRD;
};

int main()
{
	string inputStr;
	string str1;
	string str2;
	string str3;
	vector<Grade> vec;

	while (cin)
	{	
	int n = 0, m = 0;
	int i = 0, j = 0, k = 0;
	scanf_s("%d %d", &n, &m);
//	cin >> n >> m;

	int *temp = new int[m];
	for (j = 0; j < n; j++)
	{
		Grade grade;
		int x = 0;
		scanf_s("%d", &x);
		//cin >> x;
		grade.GRD = x;
		vec.push_back(grade);
	}
	while (i < m)
	{
		int id = 0, grd = 0;
		getline(cin, inputStr);
		istringstream str(inputStr);
		str >> str1 >> str2 >> str3;

		stringstream t1, t2;
		t1 << str2;
		t1 >> id;
		t2 << str3;
		t2 >> grd;

		if (str1[0] == 'Q')
		{
			int max_num = 0;
			if (id > grd)
			{
				int t;
				t = id;
				id = grd;
				grd = t;
			}
			for (int a = id; a <= grd; a++)
			{
				if (max_num < vec[a - 1].GRD)
				{
					max_num = vec[a - 1].GRD;
				}
			}
			temp[k] = max_num;
			//cout << max_num << temp[k]<<endl;
			k++;
			++i;
		}
		else if (str1[0] == 'U')
		{
			vec[id - 1].GRD = grd;
			++i;
		}

		if (i == m)
		{
			for (int l = 0; l < k; l++)
			{
				cout << temp[l] << ' ';
				cout << endl;
			}
			vec.~vector();
			delete[]temp;
		}
	}
	}
	return 0;
}
