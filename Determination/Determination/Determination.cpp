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
				string symbol;
				string to;
				match = *it;
				string match_str = match.str();
				if (match_str.size() == 2)
				{
					symbol = match_str[0];
					to = match_str[1];
					machine[from][symbol].insert(to);
				}
				else
				{
					symbol = match_str[0];
					to = "H";
					machine[from][symbol].insert(to);
				}
			}
		}
	}
}

void ReadL(Machine& machine, ifstream& input)
{
	machine["H"];

	string line;
	Cell h_cell;
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
				string symbol;
				if (match_str.size() == 2)
				{
					symbol = match_str[1];
					from = match_str[0];

					auto a_it = machine.find(from);
					if (a_it != machine.end())
					{
						Cell::iterator c_it = (a_it->second).find(symbol);
						Cell::iterator c_end = (a_it->second).end();
						if (c_it != c_end)
						{
							(c_it->second).insert(to);
						}
						else
						{
							machine[from][symbol].insert(to);
						}
					}
					else
					{
						machine[from][symbol].insert(to);
					}
				}
				else
				{
					symbol = match_str[0];
					machine["H"][symbol].insert(to);
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

void WriteAutomaton(Machine& machine, ofstream& output, Machine::iterator& it, set<string>& history)
{
	if (it != machine.end())
	{
		if (history.find(it->first) == history.end())
		{
			output << it->first << " -> ";
			history.insert(it->first);
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
					output << elem.first << to << "\n";
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

				auto next_it = machine.find(to);
				WriteAutomaton(machine, output, next_it, history);
			}
		}
	}
}

void WriteDetermization(Machine& machine, string type)
{
	set<string> history;
	ofstream output("output.txt");
	auto it = type == "R"
		? machine.find("S")
		: machine.find("H");
	WriteAutomaton(machine, output, it, history);
}

Machine Determinization(Machine& machine, string type)
{
	Machine result;
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

	stack<string> nextState;

	for (auto elem : it->second)
	{
		string to;
		for (auto str : elem.second)
		{
			to += str;
		}

		nextState.push(to);
	}

	while (nextState.size() != 0)
	{
		string from = nextState.top();
		nextState.pop();

		if (result.find(from) == result.end())
		{
			for (size_t i = 0; i < from.size(); i++)
			{
				string ch;
				ch = from[i];

				auto a_it = machine.find(ch);
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

	return result;
}

int main()
{
	string type;
	ifstream input("input.txt");
	getline(input, type);

	auto machine = ReadMachine(input, type);
	auto result = Determinization(machine, type);
	WriteDetermization(result, type);
	return 0;
}