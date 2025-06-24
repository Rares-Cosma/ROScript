/**
 * @file lexer.cpp
 * @brief Lexer implementation for the Roscript interpreter.
 * This file contains the implementation of the lexer for the Roscript interpreter. It contains functions to tokenize the input source code, identifing keywords, operators, literals, and handling string literals with escape sequences.
 * It's header contains the lexer function.
 * @see lexer.h
 * 
 * @author Rares-Cosma & Vlad-Oprea
 * @date 2025-06-10
 */
#include "lexer.h"
#include "commons.cpp"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

bool isnotsep(char letter) {
    /**
     * @brief Checks if a character is not a separator.
     * @param letter The character to check.
     * @return true if the character is not a separator, false otherwise.
     */
    string sep = " =;+-*/(){}[],<>!%\n\r";
    return sep.find(letter) == string::npos;
}

bool iskeyword(const string& word) {
    /**
     * @brief Checks if a word is a keyword in the Roscript language.
     * @param word The word to check.
     * @return true if the word is a keyword, false otherwise.
     */
    vector<string> keywords = {"var","daca","atunci","altfel","executa","cat","timp","pentru","pana","cand","fiecare","repeta"};
    for (const string& kw : keywords) {
        if (word == kw) return true;
    }
    return false;
}

pair<vector<pair<string, string>>,vector<int>> lexer(string fn) {
    vector<pair<string, string>> tokens;
    ifstream file(fn); // opens the file with the source code
    Type checker;

    vector<int> tpl; // to store the number of tokens per line

    /**
     * @brief Lexical analyzer function that reads a source file and tokenizes its content.
     * @param fn The file name to read.
     * @param tokens A vector to store the tokens found in the file.
     * @param tpl A vector to store the number of tokens per line.
     * @return A pair containing the vector of tokens and the vector of token counts per line
     */

    int ct=0; // counter representing the number of tokens on a line

    if (!file) {
        cout << "File not found" << endl;
        return {tokens, tpl};
    }

    char current_char;
    string keyword;
    while (file.get(current_char)) { // read character by character until EOF
        if (current_char == '\r') {
            continue;
        }
        if (current_char == '\n'){ // send the ct to the vector and reset it
            tpl.push_back(ct);
            ct=0;
            continue;
        }

        if (current_char == '"') { // string literals
            string str_literal = "";
            while (file.get(current_char) && current_char != '"') { 
            // the next character has been read successfully and it is not '"'
                if (current_char == '\\') {
                    // handle escape sequence
                    if (file.get(current_char)) {
                        switch (current_char) {
                            case 'n': str_literal += '\n'; break;
                            case 't': str_literal += '\t'; break;
                            case '\\': str_literal += '\\'; break;
                            case '"': str_literal += '\"'; break;
                            default: str_literal += current_char; break;
                        }
                    }
                } else {
                    str_literal += current_char; // adds the current character to the string
                }
            }
            tokens.push_back({"STRING", str_literal}); // inserts the new pair in the vector with tokens
            ct++;
            continue;
        }

        if (isnotsep(current_char)) {
            keyword += current_char; // add the current character to keyword until a separator is meet
        } else {
            if (!keyword.empty()) { // if characters were added to keyword
                if (iskeyword(keyword)) {tokens.push_back({"KEYWORD", keyword});ct++;} // KEYWORD
		        else if (checker.is_float_numeral(keyword)) {tokens.push_back({"FLOAT", keyword});ct++;} // FLOAT
                else if (checker.is_integer_numeral(keyword)) {tokens.push_back({"INT", keyword});ct++;} // INT
		        else {tokens.push_back({"ID", keyword});ct++;} // ID
                keyword = ""; // reset the keyword
            }

            // operators
            if (current_char == '=' || current_char == '!' || current_char == '<' || current_char == '>' || current_char == '+' || current_char == '-' || current_char == '*' || current_char == '/') {
                if (file.peek() == '=') {
                    char next_char;
                    file.get(next_char);
                    string op = string(1, current_char) + "=";
                    tokens.push_back({"OP", op});
                    ct++;
                } else {
                    if (current_char == '+' && file.peek() == '+') {
                        char next_char;
                        file.get(next_char);
                        tokens.push_back({"OP", "++"});
                        ct++;
                    } else if (current_char == '-' && file.peek() == '-') {
                        char next_char;
                        file.get(next_char);
                        tokens.push_back({"OP", "--"});
                        ct++;
                    } else {
                        tokens.push_back({"OP", string(1, current_char)});
                        ct++;
                    }
                }
            }
            
            // separators
            else if (current_char == ';') {tokens.push_back({"NLINE", ";"});ct++;}
            else if (current_char == '%') {tokens.push_back({"OP", "%"});ct++;}
            else if (current_char == '[') {tokens.push_back({"LBRACKET", "["});ct++;}
            else if (current_char == ']') {tokens.push_back({"RBRACKET", "]"});ct++;}
            else if (current_char == '(') {tokens.push_back({"LPAREN", "("});ct++;}
            else if (current_char == ')') {tokens.push_back({"RPAREN", ")"});ct++;}
            else if (current_char == '{') {tokens.push_back({"LBRACE", "{"});ct++;}
            else if (current_char == '}') {tokens.push_back({"RBRACE", "}"});ct++;}
            else if (current_char == ',') {tokens.push_back({"COMMA", ","});ct++;}
        }
    }

    tpl.push_back(ct); // add the last line token count
    return {tokens, tpl}; // returns the pair
}
