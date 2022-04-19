#include "ReadRegex.h"
#include <iostream>

using namespace std;

void AddConcatenation(vector<string>& transitions, const string& transition)
{
    if (transition == "")
    {
        return;
    }

    if (transitions.size() > 0)
    {
        string& lastTransition = transitions[transitions.size() - 1];
        if (transition[0] == '|')
        {
            lastTransition += transition;
            return;
        }

        if (lastTransition[lastTransition.size() - 1] == '|')
        {
            lastTransition += transition;
            return;
        }
    }

    transitions.push_back(transition);
}

vector<string> ConvertConcatenation(const string& regex)
{
    vector<string> transitions;

    size_t openParenthesis = 0;
    string transition;
    for (auto ch : regex)
    {
        if (ch == ' ')
        {
            continue;
        }

        if (openParenthesis > 0)
        {
            transition += ch;
        }

        switch (ch)
        {
        case '(':
            if (openParenthesis > 0)
            {
                openParenthesis++;
                break;
            }
            openParenthesis++;
            AddConcatenation(transitions, transition);
            transition = ch;
            break;
        case ')':
            openParenthesis--;
            break;
        case '|':
            if (openParenthesis == 0)
            {
                transition += ch;
            }
            break;
        case '*':
            if (openParenthesis > 0)
            {
                break;
            }
            AddConcatenation(transitions, transition + ch);
            transition = "";
            break;
        case '+':
            if (openParenthesis > 0)
            {
                break;
            }
            AddConcatenation(transitions, transition + ch);
            transition = "";
            break;
        default:
            if (openParenthesis > 0)
            {
                break;
            }
            AddConcatenation(transitions, transition);
            transition = ch;

            break;
        }
    }

    if (transition != "")
    {
        AddConcatenation(transitions, transition);
    }

    return transitions;
}

void ClearExtraBrackets(string& transition)
{
    if (transition == "")
    {
        return;
    }

    if (transition[0] == '(' && transition[transition.size() - 1] == ')')
    {
        using Nesting = size_t;
        Nesting nesting = 0;
        pair<char, Nesting> prevBracket = { transition[0], nesting++ };
        bool clear = true;
        for (size_t i = 1; i < transition.size(); i++)
        {
            char ch = transition[i];
            if (ch == '(')
            {
                if (prevBracket.first == ')' && prevBracket.second == 1 && nesting == 0)
                {
                    clear = false;
                    break;
                }
                prevBracket = { ch, nesting++ };
            }
            else if (ch == ')')
            {
                prevBracket = { ch, nesting-- };
            }
        }

        if (clear)
        {
            transition = transition.substr(1, transition.size() - 2);
            if (transition[0] == '(' && transition[transition.size() - 1] == ')')
            {
                ClearExtraBrackets(transition);
            }
        }
    }
}

void AddUnion(vector<string>& transitions, const string& transition)
{
    string newtransition = transition;
    ClearExtraBrackets(newtransition);

    if (newtransition == "")
    {
        return;
    }

    transitions.push_back(newtransition);
}

vector<string> ConvertUnion(string regex)
{
    ClearExtraBrackets(regex);
    vector<string> transitions;

    size_t openParenthesis = 0;
    string transition;
    for (size_t i = 0; i < regex.size(); i++)
    {
        char ch = regex[i];
        if (ch == ' ')
        {
            continue;
        }

        if (openParenthesis > 0)
        {
            transition += ch;
        }

        switch (ch)
        {
        case '(':
            openParenthesis++;
            if (openParenthesis == 1)
            {
                transition += ch;
            }
            break;
        case ')':
            openParenthesis--;
            break;
        case '|':
            if (openParenthesis > 0)
            {
                break;
            }
            AddUnion(transitions, transition);
            transition = "";
            break;
        default:
            if (openParenthesis > 0)
            {
                break;
            }
            transition += ch;
            break;
        }
    }

    AddUnion(transitions, transition);

    return transitions;
}

void DeleteSpecialChar(std::string& regex)
{
    if (regex[regex.size() - 1] != '+' && regex[regex.size() - 1] != '*')
    {
        return;
    }

    if (regex.size() == 1)
    {
        regex = "";
    }

    if (regex.size() == 2)
    {
        regex.pop_back();
        return;
    }

    if (regex[0] != '(' && regex[regex.size() - 2] != ')')
    {
        return;
    }

    using Nesting = size_t;
    Nesting nesting = 0;
    pair<char, Nesting> prevBracket = { regex[0], nesting++ };
    for (size_t i = 1; i < regex.size(); i++)
    {
        char ch = regex[i];
        if (ch == '(')
        {
            if (prevBracket.first == ')' && prevBracket.second == 1 && nesting == 0)
            {
                return;
            }
            prevBracket = { ch, nesting++ };
        }
        else if (ch == ')')
        {
            prevBracket = { ch, nesting-- };
        }
    }

    regex.pop_back();
    ClearExtraBrackets(regex);
}