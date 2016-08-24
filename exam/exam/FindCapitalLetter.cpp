#include <iostream>
#include <string>

using namespace std;

void main()
{
	string str1;
	string str2;
	getline(cin, str1);

	for (int i = 0; i < str1.size(); i++)
	{
		if (str1[i] >= 'A' && str1[i] <= 'Z')
		{
			str2.append(1u, str1[i]);
		}
		else if (str1[i] == ' ' && !str2.empty())
		{
			str2.append(1u, str1[i]);
		}
	}
	cout << str2 << endl;
	return;
}