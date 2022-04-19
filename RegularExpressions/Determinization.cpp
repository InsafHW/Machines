#include "Determinization.h"
#include <stack>
#include <queue>

using namespace std;

void Determinization(Machine& machine)
{
    Machine::Cells cells;

    Machine::PathsOfTo paths = machine.cells["0"];
    cells["0"] = paths;

    stack<Machine::To> nextStates;
    for (auto& path : paths)
    {
        string to;
        for (auto& state : path.second)
        {
            to += state;
        }
        nextStates.push(to);
    }

    while (!nextStates.empty())
    {
        auto from = nextStates.top();
        nextStates.pop();

        if (cells.find(from) != cells.end())
        {
            continue;
        }

        for (auto ch : from)
        {
            string state = string(1, ch);
            auto currentStatePathsIt = machine.cells.find(state);

            if (currentStatePathsIt == machine.cells.end())
            {
                continue;
            }

            for (auto currentStatePaths : currentStatePathsIt->second)
            {
                for (auto currentTo : currentStatePaths.second)
                {
                    cells[from][currentStatePaths.first].insert(currentTo);
                }
            }

            for (auto cell : cells[from])
            {
                string to;
                for (auto str : cell.second)
                {
                    to += str;
                }

                nextStates.push(to);
            }
        }
    }

    machine.cells = cells;
}

void WriteMachine(ostream& output, const Machine& machine)
{
    queue<Machine::Cells::const_iterator> queue;
    queue.push(machine.cells.find("0"));

    set<Machine::To> visitedStates;
    while (!queue.empty())
    {
        auto cell = queue.front();
        visitedStates.insert(cell->first);
        queue.pop();
        if (cell == machine.cells.end())
        {
            continue;
        }
        output << cell->first << " -> ";
        size_t count = 0;
        for (auto& to : cell->second)
        {
            count++;
            output << to.first;
            Machine::To stateTo;
            for (auto& state : to.second)
            {
                stateTo += state;
            }
            output << stateTo;
            if (cell->second.size() != count)
            {
                output << " | ";
            }
            auto newState = machine.cells.find(stateTo);
            if (newState == machine.cells.end())
            {
                continue;
            }
            if (visitedStates.find(newState->first) == visitedStates.end())
            {
                queue.push(machine.cells.find(stateTo));
                visitedStates.insert(stateTo);
            }
        }
        output << endl;
    }
}