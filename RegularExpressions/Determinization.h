#pragma once
#include <ostream>
#include <string>
#include <map>
#include <set>

struct Machine
{
    using To = std::string;
    using From = std::string;
    using Transition = char;

    using PathsOfTo = std::map<Transition, std::set<To>>;
    using Cells = std::map<From, PathsOfTo>;

    Cells cells;
};

void Determinization(Machine& machine);
void WriteMachine(std::ostream& output, const Machine& machine);