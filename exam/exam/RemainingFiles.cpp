#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main()
{
	string inputStr;
	string str1;
	string str2;
	int remain = 0;

	while (cin)
	{
		inputStr = "";
		str1 = "";
		str2 = "";
		getline(cin, inputStr);
		istringstream str(inputStr);
		str >> str1 >> str2;

		int a=0;
		stringstream t;
		t << str2;
		t >> a;

		if (str1[0] == 'b')
		{
			remain += a;
		}
		if (str1[0] == 'r')
		{
			if (a > remain)
			{
				remain = 0;
			}
			else remain -= a;
		}
		else if (str1[0] == 'e')
		{
			cout<<"current "<<remain<<endl;
			remain = 0;
		}
	}
	return 0;
}