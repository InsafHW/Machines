#include "RegexMachine.h"
#include "ReadRegex.h"

#include <iostream>

using namespace std;

RegexMachine::RegexMachine(const string& regex)
{
	m_paths.push_back({ m_stateCounter , ++m_stateCounter, regex });

	SplitTransitions();
	ClearEmptyTransitions();
}

void RegexMachine::SplitTransitions()
{
	size_t initialPaths;
	do
	{
		initialPaths = m_paths.size();
		auto paths = m_paths;
		for (auto& path : paths)
		{
			auto consecutiveTransitions = ConvertConcatenation(path.transition);
			if (consecutiveTransitions.size() > 1)
			{
				AddConsecutiveTransitions(path, consecutiveTransitions);
			}
		}

		paths = m_paths;
		for (auto path : paths)
		{
			auto parallelTransitions = ConvertUnion(path.transition);
			if (parallelTransitions.size() > 1)
			{
				AddParallelTransitions(path, parallelTransitions);
			}
		}

		paths = m_paths;
		for (auto path : paths)
		{
			auto transition = path.transition;
			if (transition[transition.size() - 1] == '+')
			{
				ConvertPlus(path);
			}
			else if (transition[transition.size() - 1] == '*')
			{
				ConvertMultiplication(path);
			}
		}

	} while (initialPaths != m_paths.size());
}

void RegexMachine::AddConsecutiveTransitions(const Path& currentPath, vector<string> consecutiveTransitions)
{
	size_t from = currentPath.from;
	size_t to = currentPath.to;
	DeletePath(currentPath);

	std::vector<Path> newPaths;
	auto stateFrom = from;
	size_t stateTo;
	for (size_t i = 0; i < consecutiveTransitions.size(); i++)
	{
		if (i == consecutiveTransitions.size() - 1)
		{
			stateTo = to;
		}
		else
		{
			stateTo = ++m_stateCounter;
		}
		newPaths.push_back({ stateFrom, stateTo, consecutiveTransitions[i] });
		stateFrom = stateTo;
	}
	m_paths.insert(m_paths.end(), newPaths.begin(), newPaths.end());
}

void RegexMachine::AddParallelTransitions(const Path& currentPath, vector<string> parallelTransitions)
{
	size_t from = currentPath.from;
	size_t to = currentPath.to;
	DeletePath(currentPath);

	std::vector<Path> newPaths;
	for (size_t i = 0; i < parallelTransitions.size(); i++)
	{
		newPaths.push_back({ from, to, parallelTransitions[i] });
	}
	m_paths.insert(m_paths.end(), newPaths.begin(), newPaths.end());
}

void RegexMachine::DeletePath(const Path& path)
{
	size_t index = 0;
	for (; index < m_paths.size(); index++)
	{
		auto currentPath = m_paths[index];
		if (currentPath.from == path.from && currentPath.to == path.to && currentPath.transition == path.transition)
		{
			break;
		}
	}
	m_paths.erase(m_paths.begin() + index);
}

void RegexMachine::ConvertPlus(Path path)
{
	size_t from = path.from;
	size_t to = path.to;
	std::string transition = path.transition;
	DeleteSpecialChar(transition);
	if (transition.size() == path.transition.size())
	{
		return;
	}
	DeletePath(path);

	m_paths.push_back({ from, ++m_stateCounter, transition });
	m_paths.push_back({ m_stateCounter, m_stateCounter, transition });
	m_paths.push_back({ m_stateCounter, to, string(1, EMPTY_TRANSITION_SYMBOL) });
}

void RegexMachine::ConvertMultiplication(Path path)
{
	size_t from = path.from;
	size_t to = path.to;
	std::string transition = path.transition;
	DeleteSpecialChar(transition);
	if (transition.size() == path.transition.size())
	{
		return;
	}
	DeletePath(path);

	m_paths.push_back({ from, ++m_stateCounter, std::string(1, EMPTY_TRANSITION_SYMBOL) });
	m_paths.push_back({ m_stateCounter, m_stateCounter, transition });
	m_paths.push_back({ m_stateCounter, to, std::string(1, EMPTY_TRANSITION_SYMBOL) });
}

void RegexMachine::ClearEmptyTransitions()
{
	const size_t FIRST_CLOSED_STATE = 0;
	size_t stateCounter = 0;

	vector<State> newStates;
	queue<State> statesQueue;
	statesQueue.push({ stateCounter, {FIRST_CLOSED_STATE}, {FIRST_CLOSED_STATE} });

	while (!statesQueue.empty())
	{
		auto state = statesQueue.front();
		statesQueue.pop();

		map<string, set<size_t>> closedStatesTransitions;
		queue<State::ClosedState> closedStates(deque<State::ClosedState>(state.closedStates.begin(), state.closedStates.end()));
		set<State::ClosedState> visitedClosedStates = state.closedStates;

		while (!closedStates.empty())
		{
			auto closedState = closedStates.front();
			closedStates.pop();

			for (auto& path : m_paths)
			{
				if (path.from != closedState)
				{
					continue;
				}

				if (path.transition == string(1, EMPTY_TRANSITION_SYMBOL))
				{
					if (visitedClosedStates.find(path.to) == visitedClosedStates.end())
					{
						closedStates.push(path.to);
						visitedClosedStates.insert(path.to);
					}
					continue;
				}
				closedStatesTransitions[path.transition].insert(path.to);
			}
		}
		state.closedStates = visitedClosedStates;
		for (auto transition : closedStatesTransitions)
		{
			optional<State> newState = GetStateIfAlreadyExist(statesQueue, newStates, state, transition.second);
			if (!newState)
			{
				newState = { ++stateCounter, transition.second, transition.second };
				statesQueue.push(*newState);
			}
			state.transitions[transition.first] = (*newState).index;
		}
		newStates.push_back(state);
	}

	m_paths.clear();
	for (auto state : newStates)
	{
		for (auto transition : state.transitions)
		{
			m_paths.push_back({ state.index, transition.second, transition.first });
		}
	}
}

optional<RegexMachine::State> RegexMachine::GetStateIfAlreadyExist(queue<State> statesQueue, const vector<State>& states, const State& currentState, const set<size_t>& closedStates)
{
	if (currentState.initialClosedStates == closedStates)
	{
		return currentState;
	}

	while (!statesQueue.empty())
	{
		auto state = statesQueue.front();
		statesQueue.pop();

		if (state.initialClosedStates == closedStates)
		{
			return state;
		}
	}

	for (auto state : states)
	{
		if (state.initialClosedStates == closedStates)
		{
			return state;
		}
	}

	return nullopt;
}

std::vector<RegexMachine::Path> RegexMachine::GetPaths()
{
	return m_paths;
}

Machine::Cells RegexMachine::GetPathsInMapFormat()
{
	Machine::Cells cells;

	for (const auto& path : m_paths)
	{
		cells[to_string(path.from)][path.transition.at(0)].insert(to_string(path.to));
	}

	return cells;
}