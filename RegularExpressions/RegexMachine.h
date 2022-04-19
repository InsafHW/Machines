#pragma once
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <optional>
#include "Determinization.h"

const char EMPTY_TRANSITION_SYMBOL = 0;

class RegexMachine
{
public:
    struct Path
    {
        size_t from;
        size_t to;
        std::string transition;
    };

    RegexMachine(const std::string& regex);

    std::vector<Path> GetPaths();
    Machine::Cells GetPathsInMapFormat();

private:
    struct State
    {
        using Transition = std::string;
        using To = size_t;
        using ClosedState = size_t;

        size_t index;
        std::set<ClosedState> initialClosedStates;
        std::set<ClosedState> closedStates;
        std::map<Transition, To> transitions;
    };

    void ConvertPlus(Path path);
    void ConvertMultiplication(Path path);
    void AddParallelTransitions(const Path& currentPath, std::vector<std::string> parallelTransitions);
    void AddConsecutiveTransitions(const Path& currentPath, std::vector<std::string> consecutiveTransitions);
    void DeletePath(const Path& path);

    std::optional<State> GetStateIfAlreadyExist(std::queue<State> statesQueue, const std::vector<State>& states, const State& currentState, const std::set<size_t>& closedStates);

    void SplitTransitions();
    void ClearEmptyTransitions();

    size_t m_stateCounter = 0;
    std::vector<Path> m_paths;
};