#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include "Rules.h"

using namespace std;

bool CompareLine(vector<Rule> const& arr, string line)
{
	stack<int> nextStep;
	int index = 0;
	int ch = 0;

	while (true)
	{
		char symbol = (ch >= line.size()) ? '\0' : line[ch];
		auto found = find(arr[index].m_guidSimbol.begin(), arr[index].m_guidSimbol.end(), symbol);
		if (arr[index].m_error && found == arr[index].m_guidSimbol.end())
		{
			return false;
		}
		if (!arr[index].m_error && found == arr[index].m_guidSimbol.end())
		{
			++index;
			continue;
		}
		if (arr[index].m_isNextInStack)
		{
			nextStep.push(index + 1);
		}
		if (arr[index].m_shift)
		{
			++ch;
		}
		if (arr[index].m_isEnd)
		{
			break;
		}
		if (arr[index].m_ptrNext != -1)
		{
			index = arr[index].m_ptrNext;
		}
		else
		{
			index = nextStep.top();
			nextStep.pop();
		}
	}

	return true;
}

int main()
{
	Rules rules = Rules();

	ifstream input("input.txt");
	string line;
	while (getline(input, line))
	{
		cout << line << ":  is ";
		CompareLine(rules.GetRules(), line)
			? cout << "true" << endl
			: cout << "false" << endl;
	}
}