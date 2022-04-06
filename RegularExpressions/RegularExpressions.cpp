#include <sstream> 
#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <fstream>
#include <tuple>
#include <set>
#include <queue>

using namespace std;

const char PLUS_SYMBOL = '+';
const char STAR_SYMBOl = '*';
const char OR_SYMBOL = '|';
const char OPEN_BRACKET_SYMBOL = '(';
const char CLOSE_BRACKET_SYMBOL = ')';
const char EMPTY_SIGNAL = '&';

struct cell
{
	int fromState = 0;
	int toState = 0;
	char signal = 0;
};

struct transitionMatrix
{
	int startState = 0;
	int endState = 0;
	vector<cell> matrix;
};

int stateCounter;

transitionMatrix buildNFA(string inputRegex, int& currentIndex, bool& orFound, transitionMatrix prevBlock = {})
{
	transitionMatrix resultMatrix;
	resultMatrix.startState = stateCounter;
	while (currentIndex < inputRegex.size())
	{
		if (inputRegex[currentIndex] == OPEN_BRACKET_SYMBOL)
		{
			currentIndex++;
			prevBlock = buildNFA(inputRegex, currentIndex, orFound, prevBlock);
			resultMatrix.endState = prevBlock.endState;
			if (orFound)
				resultMatrix.startState = prevBlock.startState;
			for (auto cell : prevBlock.matrix)
			{
				resultMatrix.matrix.push_back(cell);
			}
		}
		else if (inputRegex[currentIndex] == CLOSE_BRACKET_SYMBOL)
		{
			currentIndex++;
			return resultMatrix;
		}
		else if (inputRegex[currentIndex] == OR_SYMBOL)
		{
			currentIndex++;
			orFound = true;
			resultMatrix.startState = ++stateCounter;
			resultMatrix.matrix.push_back({ stateCounter , prevBlock.startState, EMPTY_SIGNAL });
			resultMatrix.matrix.push_back({ stateCounter , ++stateCounter, EMPTY_SIGNAL });
			prevBlock = resultMatrix;
		}
		else if (inputRegex[currentIndex] == STAR_SYMBOl)
		{
			currentIndex++;
			resultMatrix.matrix.push_back({ prevBlock.startState, stateCounter ,EMPTY_SIGNAL });
			resultMatrix.matrix.push_back({ prevBlock.endState, prevBlock.startState, EMPTY_SIGNAL });
			resultMatrix.endState = prevBlock.endState;
		}
		else if (inputRegex[currentIndex] == PLUS_SYMBOL)
		{
			currentIndex++;
			resultMatrix.matrix.push_back({ prevBlock.endState, prevBlock.startState, EMPTY_SIGNAL });
			resultMatrix.endState = prevBlock.endState;
		}
		else
		{
			cell newCell;
			transitionMatrix prevBlock2;
			prevBlock2.startState = stateCounter;

			newCell.fromState = stateCounter++;
			newCell.signal = inputRegex[currentIndex++];
			newCell.toState = stateCounter;
			resultMatrix.matrix.push_back(newCell);
			prevBlock2.matrix.push_back(newCell);
			resultMatrix.endState = stateCounter;
			prevBlock2.endState = stateCounter;
			prevBlock = prevBlock2;
		}
	}

	return resultMatrix;
}

void printDFA(transitionMatrix matrix)
{
	sort(
		matrix.matrix.begin(),
		matrix.matrix.end(),
		[](cell a, cell b) 
		{ 
			return a.fromState < b.fromState; 
		}
	);
	for (auto& cell : matrix.matrix)
	{
		cout << cell.fromState << " -> " << cell.signal << cell.toState << endl;
	}
}

int main()
{
	fstream input("input.txt");
	string line;
	getline(input, line);

	int i = 0;
	bool orFound = false;
	auto matrix = buildNFA(line, i, orFound);
	printDFA(matrix);
}