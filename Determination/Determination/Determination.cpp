#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <regex>
#include <stack>
#include <set>
#include <map>

using namespace std;

typedef map<string, set<string>> Cell;
typedef map<string, Cell> Machine;

const regex Regex("[a-zA-Z]+");

Machine result;
set<string> exist;

void ReadR(Machine& machine, ifstream& input)
{
	string line;
	smatch rmatch;

	while (getline(input, line))
	{
		sregex_iterator it = sregex_iterator(line.begin(), line.end(), Regex), end;

		if (it != end)
		{
			smatch match = *it;
			string from = match.str();

			it++;
			for (int index = 0; it != end; ++it)
			{
				match = *it;
				string match_str = match.str();
				match_str.size() == 2
					? machine[from][to_string(match_str[0])].insert(to_string(match_str[1]))
					: machine[from][to_string(match_str[0])].insert("H");
			}
		}
	}
}

void ReadL(Machine& machine, ifstream& input)
{
	machine["H"];

	string line;
	while (getline(input, line))
	{
		sregex_iterator it = sregex_iterator(line.begin(), line.end(), Regex), end;

		if (it != end)
		{
			smatch match = *it;
			string to = match.str();
			string from;
			it++;

			for (int index = 0; it != end; ++it)
			{
				match = *it;
				string match_str = match.str();
				if (match_str.size() == 2)
				{
					string symbol = to_string(match_str[1]);
					from = match_str[0];

					auto a_it = machine.find(from);
					if (a_it != machine.end())
					{
						auto c_it = (a_it->second).find(symbol);
						auto c_end = (a_it->second).end();
						c_it != c_end
							? (c_it->second).insert(to)
							: machine[from][symbol].insert(to);
					}
					else
					{
						machine[from][symbol].insert(to);
					}
				}
				else
				{
					machine["H"][to_string(match_str[0])].insert(to);
				}
			}
		}
	}
}

Machine ReadMachine(ifstream& input, string type)
{
	Machine machine;
	type == "R"
		? ReadR(machine, input)
		: ReadL(machine, input);
	return machine;
}

void WriteMachine(ofstream& output, Machine::iterator& it)
{
	if (it != result.end())
	{
		if (exist.find(it->first) == exist.end())
		{
			output << it->first << " -> ";
			exist.insert(it->first);
			size_t count = 1;
			for (auto elem : it->second)
			{
				auto e_it = elem.second.begin();
				string to;
				for (auto str : elem.second)
				{
					to += str;
				}

				if (count < (it->second).size())
				{
					output << elem.first << to << " | ";
					count++;
				}
				else
				{
					output << elem.first << to << endl;
				}
			}

			for (auto elem : it->second)
			{
				auto e_it = elem.second.begin();
				string to;
				for (auto str : elem.second)
				{
					to += str;
				}

				Machine::iterator next_it = result.find(to);
				WriteMachine(output, next_it);
			}
		}
	}
}

void WriteDetermination(string type)
{
	ofstream output("output.txt");
	auto it = type == "R"
		? result.find("S")
		: result.find("H");
	WriteMachine(output, it);
}

void Determinize(Machine& machine, string type)
{
	stack<string> nextState;
	Machine::iterator it;
	if (type == "R")
	{
		it = machine.find("S");
		result["S"] = it->second;
	}
	else
	{
		it = machine.find("H");
		result["H"] = it->second;
	}

	for (auto elem : it->second)
	{
		string to;
		for (auto str : elem.second)
		{
			to += str;
		}

		nextState.push(to);
	}

	while (nextState.size())
	{
		string from = nextState.top();
		nextState.pop();

		if (result.find(from) == result.end())
		{
			for (size_t i = 0; i < from.size(); i++)
			{
				auto a_it = machine.find(to_string(from[i]));
				if (a_it != machine.end())
				{
					auto c_it = a_it->second;

					for (auto i = c_it.begin(); i != c_it.end(); i++)
					{
						for (auto elem : i->second)
						{
							result[from][i->first].insert(elem);
						}
					}

					for (auto elem : result[from])
					{
						string to;
						for (auto str : elem.second)
						{
							to += str;
						}

						nextState.push(to);
					}
				}
			}
		}
	}
}

int main()
{
	string type;
	ifstream input("input.txt");
	getline(input, type);

	auto machine = ReadMachine(input, type);

	Determinize(machine, type);
	WriteDetermination(type);
}