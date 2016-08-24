#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

typedef struct timer
{
    int id;
    int time;
}Timer;

string ErasePunct(string str)
{
	int i;
	string str_t = str;
	for (i = 0; i < str_t.size();)
	{
		if (ispunct(str_t[i]))
		{
			str_t[i] = ' ';
		}
		else ++i;
	}
	return str_t;
}


int main()
{
	string inputStr;
	string inputStr_t;
	string str1;
	string str2;
	string str3;

	vector<Timer> vec;

	while (cin)
	{
		inputStr = "";
		str1 = "";
		str2 = "";
		str3 = "";

		getline(cin, inputStr_t);
		inputStr=ErasePunct(inputStr_t);
		istringstream str(inputStr);
		str >> str1 >> str2 >> str3;

		if (str1[0] == 's')
		{
			int ID = 0;
			int time = 0;
			stringstream t1,t2;
			t1 << str2;
			t1 >> ID;
			t2 << str3;
			t2 >> time;
			
			if (str1[2] == 'a')
			{

				Timer temp;
				temp.id = ID;
				temp.time = time;
				vec.push_back(temp);

			}
			else if (str1[2] == 'o')
			{
				int i;
				for(i=0;i<vec.size();i++)
				{
					if(vec[i].id == ID)
					{
						vec[i].time = -1;
					}
				}			
			}
		}
		else if (str1[0] == 'e')
		{
			if (str1[1] == 'l')
			{
				int elapsetime_t = 0;
				stringstream t3;
				t3 << str2;
				t3 >> elapsetime_t;

				int i;
				for(i=0;i<vec.size();i++)
				{
					vec[i].time =vec[i].time-elapsetime_t;
				}
			}
			else if (str1[1] == 'n')
			{
				int flag = 0;
				int i;
				for (i = 0; i<vec.size(); i++)
				{
					if (vec[i].time>0)
					{
						flag = 1;
						cout << "timer:" << vec[i].id << "," << vec[i].time << endl;
					}
				}
				if (flag == 0)
				{
					cout << "none" << endl;
				}
				
			}
		}
	}
	return 0;
} 
