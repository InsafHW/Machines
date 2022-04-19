#include <iostream>
#include <fstream>
#include <string>

#include "RegexMachine.h"

using namespace std;

int main()
{
    ifstream input("input.txt");
    string regex;
    input >> regex;

    RegexMachine regexMachine(regex);

    Machine machine = { regexMachine.GetPathsInMapFormat() };
    Determinization(machine);

    ofstream output("output.txt");
    WriteMachine(std::cout, machine);
}