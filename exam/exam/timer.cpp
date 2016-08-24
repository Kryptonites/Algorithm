#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Timer
{
public:
	int id;
	int time;
};
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

	vector<Timer> Serialvec;

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
				Timer timer;
				timer.id=ID;
				timer.time = time;
				Serialvec.push_back(timer);
			}
			else if (str1[2] == 'o')
			{
				for (vector<Timer>::iterator it = Serialvec.begin(); it != Serialvec.end(); ++it)
				{
					if ((*it).id == ID)
					{
						Serialvec.erase(it);
						break;
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
				for (i = 0; i<Serialvec.size(); i++)
				{
					Serialvec[i].time = Serialvec[i].time - elapsetime_t;
				}
			}
			else if (str1[1] == 'n')
			{
				int m = 0;
				for (int i = 0; i < Serialvec.size(); i++)
				{
					if (Serialvec[i].time > 0)
					{
						cout << "timer:" << Serialvec[i].id << "," << Serialvec[i].time << endl;
						m=1;
					}
				}
					if (m==0)
					{
						cout << "none" << endl;
					}
			}
		}
	}
	Serialvec.~vector();
	return 0;
} 