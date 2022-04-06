#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include "Tokens.h"

using namespace std;

bool isNumber(const string& str)
{
	for (char const& c : str) {
		if (isdigit(c) == 0) return false;
	}
	return true;
}

void WriteToken(string& word, Tokens& tokens, ofstream& output)
{
	word.erase(remove_if(word.begin(), word.end(), isspace), word.end());
	auto data = tokens.GetTokenData(word);

	if (data)
	{
		output << "TOKEN=" << data->tokenName << " ; VALUE=" << word << endl;
		cout << "TOKEN=" << data->tokenName << " ; VALUE=" << word << endl;
	}
	else if (word.size())
	{
		if (isNumber(word))
		{
			output << "TOKEN=number" << " ; value=" << word << endl;
			cout << "TOKEN=number" << " ; value=" << word << endl;
		}
		else
		{
			output << "TOKEN=identifier" << " ; value=" << word << endl;
			cout << "TOKEN=identifier" << " ; value=" << word << endl;
		}
	}
	word = "";
};

int main()
{
	ifstream input("input.txt");
	ofstream output("output.txt");

	Tokens tokens = Tokens();
	string line;

	while (getline(input, line))
	{
		bool roundBracketOpened = false; // (
		bool curlyBraceOpened = false; // {
		bool squareBracketOpened = false; // [
		bool singleQuouteOpened = false; // '
		bool doubleQuoteOpened = false; // "

		string word;
		bool recentlyClosedStr = false;

		for (size_t i = 0; i < line.size(); i++)
		{
			const auto letter = line[i];

			if (letter == '/')
			{
				string copy = word;
				copy.erase(remove_if(copy.begin(), copy.end(), isspace), copy.end());
				if (copy == "/")
				{
					break;
				}
			}

			switch (letter)
			{
			case '(':
				word.erase(remove_if(word.begin(), word.end(), isspace), word.end());

				roundBracketOpened = true;

				if (word.size())
				{
					output << "TOKEN=identifier" << " ; VALUE=" << word << endl;
					cout << "TOKEN=identifier" << " ; VALUE=" << word << endl;
				}

				output << "TOKEN=" << tokens.GetTokenData("(")->tokenName << " ; VALUE=(" << endl;
				cout << "TOKEN=" << tokens.GetTokenData("(")->tokenName << " ; VALUE=(" << endl;

				word = "";
				continue;
				break;
			case '{':
				curlyBraceOpened = true;
				output << "TOKEN=" << tokens.GetTokenData("{")->tokenName << " ; VALUE={" << endl;
				cout << "TOKEN=" << tokens.GetTokenData("{")->tokenName << " ; VALUE={" << endl;
				continue;
				break;
			case '[':
				squareBracketOpened = true;
				WriteToken(word, tokens, output);

				output << "TOKEN=" << tokens.GetTokenData("[")->tokenName << " ; VALUE=[" << endl;
				cout << "TOKEN=" << tokens.GetTokenData("[")->tokenName << " ; VALUE=[" << endl;
				continue;
				break;
			case '\'':
				singleQuouteOpened = !singleQuouteOpened;
				recentlyClosedStr = !singleQuouteOpened;
				break;
			case '"':
				doubleQuoteOpened = !doubleQuoteOpened;
				recentlyClosedStr = !doubleQuoteOpened;
				break;

			case ')':
			{
				roundBracketOpened = false;
				WriteToken(word, tokens, output);

				output << "TOKEN=" << tokens.GetTokenData(")")->tokenName << " ; VALUE=)" << endl;
				cout << "TOKEN=" << tokens.GetTokenData(")")->tokenName << " ; VALUE=)" << endl;

				continue;
				break;
			}
			case '}':
				curlyBraceOpened = false;
				output << "TOKEN=" << tokens.GetTokenData("}")->tokenName << " ; VALUE=}" << endl;
				cout << "TOKEN=" << tokens.GetTokenData("}")->tokenName << " ; VALUE=}" << endl;
				continue;
				break;
			case ']':
				squareBracketOpened = false;
				WriteToken(word, tokens, output);

				output << "TOKEN=" << tokens.GetTokenData("]")->tokenName << " ; VALUE=]" << endl;
				cout << "TOKEN=" << tokens.GetTokenData("]")->tokenName << " ; VALUE=]" << endl;
				continue;
				break;
			case '.':
				if (!singleQuouteOpened && !doubleQuoteOpened)
				{
					word.erase(remove_if(word.begin(), word.end(), isspace), word.end());

					output << "TOKEN=identifier" << " ; VALUE=" << word << endl;
					cout << "TOKEN=identifier" << " ; VALUE=" << word << endl;

					output << "TOKEN=" << tokens.GetTokenData(".")->tokenName << " ; VALUE=." << endl;
					cout << "TOKEN=" << tokens.GetTokenData(".")->tokenName << " ; VALUE=." << endl;

					word = "";
					continue;
				}
				break;
			case ',':
				if (!singleQuouteOpened && !doubleQuoteOpened)
				{
					WriteToken(word, tokens, output);

					output << "TOKEN=" << tokens.GetTokenData(",")->tokenName << " ; VALUE=," << endl;
					cout << "TOKEN=" << tokens.GetTokenData(",")->tokenName << " ; VALUE=," << endl;
					continue;
				};
				break;
			case ';':
				if (!singleQuouteOpened && !doubleQuoteOpened)
				{
					WriteToken(word, tokens, output);

					output << "TOKEN=" << tokens.GetTokenData(";")->tokenName << " ; VALUE=;" << endl;
					cout << "TOKEN=" << tokens.GetTokenData(";")->tokenName << " ; VALUE=;" << endl;
					continue;
				};
				break;
			case '=':
				if (!singleQuouteOpened && !doubleQuoteOpened)
				{
					WriteToken(word, tokens, output);

					output << "TOKEN=" << tokens.GetTokenData("=")->tokenName << " ; VALUE==" << endl;
					cout << "TOKEN=" << tokens.GetTokenData("=")->tokenName << " ; VALUE==" << endl;
					continue;
				};
				break;
			case ':':
				if (!singleQuouteOpened && !doubleQuoteOpened)
				{
					WriteToken(word, tokens, output);

					output << "TOKEN=" << tokens.GetTokenData(":")->tokenName << " ; VALUE=:" << endl;
					cout << "TOKEN=" << tokens.GetTokenData(":")->tokenName << " ; VALUE=:" << endl;
					continue;
				};
				break;
			default:
				if (singleQuouteOpened || doubleQuoteOpened)
				{
					word += letter;
					continue;
				};
				if (recentlyClosedStr)
				{
					word.erase(0, 1);
					output << "TOKEN=string" << " ; value=" << word << endl;
					cout << "TOKEN=string" << " ; value=" << word << endl;
					recentlyClosedStr = false;
					word = "";
					continue;
				};

				if (letter == ' ')
				{
					WriteToken(word, tokens, output);
				}
				else
				{
					word += letter;
				}

				if (i == line.size() - 1 && word.size())
				{
					WriteToken(word, tokens, output);
				};
				break;
			}
		}

	}

	return 0;
}