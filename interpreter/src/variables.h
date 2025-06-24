/**
 * @file variables.h
 * @brief Header file for the variables management in the Roscript interpreter.
 * This file contains the declaration of the variables map and the Value type used to store different types of values.
 * @author Rares-Cosma & Vlad-Oprea
 * @date 2025-06-23
 */

#pragma once
#include <unordered_map>
#include <string>
#include <variant>
using namespace std;
using Value = variant<int, float, string, bool>;

extern std::unordered_map<std::string, Value> variables;