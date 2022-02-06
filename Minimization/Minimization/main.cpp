#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include<algorithm>
#include <map>

using namespace std;

const int SETW_VALUE = 20;

struct Cell
{
	int multiplicity;
	int to;
	int out;
};

typedef map<int, vector<int>> EMatrix;
typedef vector<vector<Cell>> Machine;

struct Result
{
	Machine resultMachine;
	vector<int> resultIndexes;
};

void GetEquivalenceMatrix(Machine& machine, vector<int>& indexes, EMatrix& equivalenceMatrix)
{
	int k = 0;
	equivalenceMatrix[k].push_back(indexes[0]);

	for (int i = 1; i < indexes.size(); i++)
	{
		bool isFound = false;
		EMatrix::iterator it = equivalenceMatrix.begin();

		while (it != equivalenceMatrix.end() && !isFound)
		{
			isFound = true;
			for (int j = 0; j < machine.size(); j++)
			{
				if (machine[j][it->second[0]].multiplicity != machine[j][indexes[i]].multiplicity)
				{
					isFound = false;
					it++;
					break;
				}
			}
		}

		isFound
			? equivalenceMatrix[it->first].push_back(indexes[i])
			: equivalenceMatrix[++k].push_back(indexes[i]);
	}
}

void OverrideAutomaton(Machine& machine, EMatrix& equivalenceMatrix)
{
	for (int i = 0; i < machine.size(); i++)
	{
		for (int j = 0; j < machine[0].size(); j++)
		{
			auto it = equivalenceMatrix.begin();
			while (find((it->second).begin(), (it->second).end(), machine[i][j].to) == (it->second).end())
			{
				it++;
			}
			machine[i][j].multiplicity = it->first;
		}
	}
}

Result GetResult(Machine& machine, EMatrix& ResultMatrix)
{
	vector<int> resultIndexes;
	Machine resultMachine(machine.size(), vector<Cell>(ResultMatrix.size()));
	EMatrix::iterator it = ResultMatrix.begin();

	for (int i = 0; i < resultMachine[0].size(); i++)
	{
		for (int j = 0; j < resultMachine.size(); j++)
		{
			resultMachine[j][i] = machine[j][it->second[0]];

			auto n = ResultMatrix.begin();
			while (find((n->second).begin(), (n->second).end(), machine[j][it->second[0]].to) == (n->second).end())
			{
				n++;
			}
			resultMachine[j][i].to = n->second[0];
		}
		resultIndexes.push_back(it->second[0]);
		it++;
	}

	return { resultMachine, resultIndexes };
}

Result Minimize(Machine& machine, EMatrix& matrix, int prevMachineSize)
{
	EMatrix equivalenceMatrix;
	for (auto it = matrix.begin(); it != matrix.end(); it++)
	{
		EMatrix matrixE;
		GetEquivalenceMatrix(machine, it->second, matrixE);

		int k = equivalenceMatrix.size();
		for (auto arr : matrixE)
		{
			equivalenceMatrix[k] = arr.second;
			k++;
		}
	}
	matrix = equivalenceMatrix;

	OverrideAutomaton(machine, equivalenceMatrix);
	if (equivalenceMatrix.size() == prevMachineSize)
	{
		return GetResult(machine, equivalenceMatrix);
	}

	prevMachineSize = equivalenceMatrix.size();
	return Minimize(machine, matrix, prevMachineSize);
}

void ReadMilli(Machine& a, ifstream& input)
{
	string lineS;
	string lineY;
	getline(input, lineS);

	for (int i = 0; i < a.size(); i++)
	{
		getline(input, lineS);
		replace_if(lineS.begin(), lineS.end(), isalpha, ' ');

		getline(input, lineY);
		replace_if(lineY.begin(), lineY.end(), isalpha, ' ');

		stringstream strS(lineS);
		stringstream strY(lineY);

		int ar = a[0].size();
		for (int j = 0; j < a[0].size(); j++)
		{
			int numS;
			strS >> numS;
			a[i][j].to = numS - 1;

			int numY;
			strY >> numY;
			a[i][j].multiplicity = numY - 1;
			a[i][j].out = numY - 1;
		}
	}
}

void ReadMurra(Machine& a, ifstream& input)
{
	string lineS;
	string lineY;
	getline(input, lineS);

	getline(input, lineY);
	replace_if(lineY.begin(), lineY.end(), isalpha, ' ');
	stringstream strY(lineY);

	for (int i = 0; i < a[0].size(); i++)
	{
		int numY;
		strY >> numY;

		for (int j = 0; j < a.size(); j++)
		{
			a[j][i].multiplicity = numY - 1;
			a[j][i].out = numY - 1;
		}
	}

	for (int i = 0; i < a.size(); i++)
	{
		getline(input, lineS);
		replace_if(lineS.begin(), lineS.end(), isalpha, ' ');
		stringstream strS(lineS);

		for (int j = 0; j < a[0].size(); j++)
		{
			int numS;
			strS >> numS;
			a[i][j].to = numS - 1;
		}
	}
}

Machine ReadMachine(ifstream& input, string type)
{
	int x, y, r;
	input >> y >> x >> r;
	Machine machine(x, vector<Cell>(y));

	type == "Ml"
		? ReadMilli(machine, input)
		: ReadMurra(machine, input);
	return machine;
}

void WriteMilli(Result& result, ofstream& output)
{
	auto machine = result.resultMachine;
	auto indexes = result.resultIndexes;
	for (int i = 0; i < indexes.size(); i++)
	{
		output << setw(SETW_VALUE) << "S" + to_string(indexes[i] + 1);
	}
	output << endl;

	for (int i = 0; i < machine.size(); i++)
	{
		output << "X" << i + 1;
		for (int j = 0; j < machine[0].size(); j++)
		{
			output << setw(SETW_VALUE) << right << "[S" + to_string(machine[i][j].to + 1) + ", Y" + to_string(machine[i][j].out + 1) + "]";
		}
		output << endl;
	}
}

void WriteMurra(Result& result, ofstream& output)
{
	auto machine = result.resultMachine;
	auto indexes = result.resultIndexes;
	for (int i = 0; i < indexes.size(); i++)
	{
		output << setw(SETW_VALUE) << "Y" + to_string(machine[0][i].out + 1);
	}
	output << endl;

	for (int i = 0; i < indexes.size(); i++)
	{
		output << setw(SETW_VALUE) << "S" + to_string(indexes[i] + 1);
	}
	output << endl;

	for (int i = 0; i < machine.size(); i++)
	{
		output << "X" << i + 1;
		for (int j = 0; j < machine[0].size(); j++)
		{
			string out = "[S" + to_string(machine[i][j].to + 1) + "]";
			output << setw(SETW_VALUE) << right << out;
		}
		output << endl;
	}
}

void WriteMachine(Result& result, string type)
{
	ofstream output("output.txt");
	type == "Ml"
		? WriteMilli(result, output)
		: WriteMurra(result, output);
}

int main()
{
	string type;
	ifstream input("input.txt");
	getline(input, type);
	EMatrix matrix;

	auto machine = ReadMachine(input, type);

	int machineSize = machine[0].size();
	for (int i = 0; i < machineSize; i++)
	{
		matrix[0].push_back(i);
	}

	auto result = Minimize(machine, matrix, machineSize);
	WriteMachine(result, type);
}