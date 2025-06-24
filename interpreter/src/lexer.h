/**
 * @file lexer.h
 * @brief Header file for the lexer component of the Roscript interpreter.
 * This file contains the declaration of the lexer function.
 * @author Rares-Cosma & Vlad-Oprea
 * @date 2025-06-10
 */

#pragma once
#include <vector>
#include <string>

using namespace std;

/**
 * @brief Lexical analyzer function that reads a source file and tokenizes its content.
 * @param fn The file name to read.
 * @return A pair containing a vector of tokens (as pairs of type and value) and a vector of integers representing the number of tokens per line.
 */
pair<vector<pair<string,string>>,vector<int>> lexer(string fn);