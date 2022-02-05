#pragma once
#include <map>
#include <string>
#include <optional>

using namespace std;

struct TokenData
{
	string tokenName;
};

class Tokens
{
public:
	Tokens()
	{
		m_tokens["break"] = { "break" };
		m_tokens["case"] = { "case" };
		m_tokens["class"] = { "class" };
		m_tokens["catch"] = { "catch" };
		m_tokens["const"] = { "const" };
		m_tokens["continue"] = { "continue" };
		m_tokens["debugger"] = { "debugger" };
		m_tokens["default"] = { "default" };
		m_tokens["delete"] = { "delete" };
		m_tokens["do"] = { "do" };
		m_tokens["else"] = { "else" };
		m_tokens["export"] = { "export" };
		m_tokens["extends"] = { "extends" };
		m_tokens["finally"] = { "finally" };
		m_tokens["for"] = { "for" };
		m_tokens["function"] = { "function" };
		m_tokens["if"] = { "if" };
		m_tokens["import"] = { "import" };
		m_tokens["in"] = { "in" };
		m_tokens["instanceof"] = { "instanceof" };
		m_tokens["let"] = { "let" };
		m_tokens["new"] = { "new" };
		m_tokens["return"] = { "return" };
		m_tokens["super"] = { "super" };
		m_tokens["switch"] = { "switch" };
		m_tokens["this"] = { "this" };
		m_tokens["throw"] = { "throw" };
		m_tokens["try"] = { "try" };
		m_tokens["typeof"] = { "typeof" };
		m_tokens["var"] = { "var" };
		m_tokens["void"] = { "void" };
		m_tokens["while"] = { "while" };
		m_tokens["("] = { "open_round_bracket" };
		m_tokens[")"] = { "close_round_bracket" };
		m_tokens["{"] = { "open_curly_brace" };
		m_tokens["}"] = { "close_curly_brace" };
		m_tokens["."] = { "dot" };
		m_tokens[","] = { "comma" };
		m_tokens["from"] = { "from" };
		m_tokens["="] = { "assignee" };
		m_tokens[";"] = { "semi_colon" };
		m_tokens[":"] = { "colon" };
		m_tokens[">"] = { "more" };
		m_tokens["<"] = { "less" };
		m_tokens["+"] = { "plus" };
		m_tokens["-"] = { "minus" };
		m_tokens["["] = { "open_square_bracket" };
		m_tokens["]"] = { "close_square_bracket" };
	};

	optional<TokenData> GetTokenData(string tokenName)
	{
		auto token = m_tokens.find(tokenName);
		if (token != m_tokens.end())
		{
			return token->second;
		};
		return nullopt;
	};

private:
	map<string, TokenData> m_tokens;
};