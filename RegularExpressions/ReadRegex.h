#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>

void AddConcatenation(std::vector<std::string>& transitions, const std::string& transition);

std::vector<std::string> ConvertConcatenation(const std::string& regex);

void ClearExtraBrackets(std::string& transition);

void AddUnion(std::vector<std::string>& transitions, const std::string& transition);

std::vector<std::string> ConvertUnion(std::string regex);

void DeleteSpecialChar(std::string& regex);