#pragma once
#include <vector>

using namespace std;

struct Rule
{
	char m_term;
	vector<char> m_guidSimbol;
	bool m_shift;
	bool m_error;
	int m_ptrNext;
	bool m_isNextInStack;
	bool m_isEnd;
};

class Rules
{
public:
	Rules()
	{
		m_rules.push_back({ 'S', { '9', 'a', '-', '(' }, false, true, 1, false, false });
		m_rules.push_back({ 'E', { '9', 'a', '-', '(' }, false, true, 3, true, false });
		m_rules.push_back({ '\0', { '\0' }, true, true, -1, false, true });
		m_rules.push_back({ 'E', { '9', 'a', '-', '(' }, false, true, 4, false, false });
		m_rules.push_back({ 'T', { '9', 'a', '-', '(' }, false, true, 14, true, false });
		m_rules.push_back({ 'A', { ')', '+', '\0' }, false, true, 6, false, false });
		m_rules.push_back({ 'A', { '+' }, false, false, 11, false, false });
		m_rules.push_back({ 'A', { ')' }, false, false, 10, false, false });
		m_rules.push_back({ 'A', { '\0' }, false, true, 9, false, false });
		m_rules.push_back({ '\0', { '\0' }, false, true, -1, false, false });
		m_rules.push_back({ ')', { ')' }, false, true, -1, false, false });
		m_rules.push_back({ '+', { '+' }, true, true, 12, false, false });
		m_rules.push_back({ 'T', { '9', 'a', '-', '(' }, false, true, 14, true, false });
		m_rules.push_back({ 'A', { ')', '+', '\0' }, false, true, 6, false, false });
		m_rules.push_back({ 'T', { '9', 'a', '-', '(' }, false, true, 15, false, false });
		m_rules.push_back({ 'F', { '9', 'a', '-', '(' }, false, true, 27, true, false });
		m_rules.push_back({ 'B', { '*', '+', ')', '\0' }, false, true, 17, false, false });
		m_rules.push_back({ 'B', { '*' }, false, false, 24, false, false });
		m_rules.push_back({ 'B', { '+' }, false, false, 23, false, false });
		m_rules.push_back({ 'B', { ')' }, false, false, 22, false, false });
		m_rules.push_back({ 'B', { '\0' }, false, true, 21, false, false });
		m_rules.push_back({ '\0', { '\0' }, false, true, -1, false, false });
		m_rules.push_back({ ')', { ')' }, false, true, -1, false, false });
		m_rules.push_back({ '+', { '+' }, false, true, -1, false, false });
		m_rules.push_back({ '*', { '*' }, true, true, 25, false, false });
		m_rules.push_back({ 'F', { '9', 'a', '-', '(' }, false, true, 27, true, false });
		m_rules.push_back({ 'B', { '*', '+', ')', '\0' }, false, true, 17, false, false });
		m_rules.push_back({ 'F', { '9' }, false, false, 31, false, false });
		m_rules.push_back({ 'F', { 'a' }, false, false, 32, false, false });
		m_rules.push_back({ 'F', { '-' }, false, false, 33, false, false });
		m_rules.push_back({ 'F', { '(' }, false, true, 35, false, false });
		m_rules.push_back({ '9', { '9' }, true, true, -1, false, false });
		m_rules.push_back({ 'a', { 'a' }, true, true, -1, false, false });
		m_rules.push_back({ '-', { '-' }, true, true, 34, false, false });
		m_rules.push_back({ 'F', { '9', 'a', '-', '(' }, false, true, 27, false, false });
		m_rules.push_back({ '(', { '(' }, true, true, 36, false, false });
		m_rules.push_back({ 'E', { '9', 'a', '-', '(' }, false, true, 3, true, false });
		m_rules.push_back({ ')', { ')' }, true, true, -1, false, false });
	};

	vector<Rule> GetRules()
	{
		return m_rules;
	}
private:
	vector<Rule> m_rules;
};

