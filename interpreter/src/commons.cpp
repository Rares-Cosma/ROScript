/**
 * @file commons.cpp
 * @brief Common file for the Roscript language, mainly used by the lexer
 * The file contains functions that help identify the type of a choosen keyword
 * 
 * @author Rares-Cosma & Vlad-Oprea
 * @date 2025-06-11 
 */

#include <string>

using namespace std;

/**
 * @class Type
 * @brief A class to identify the type of a keyword
 */

class Type {
	public:
		bool is_float_numeral(string kwd){ // returns true if the keyword is float
			/**
     		* @brief Checks if a keyword is a float numeral
     		* @param kwd The keyword to check
     		* @return true if the keyword is a float numeral, false otherwise.
     		*/
			bool found_comma=false;
			string acc="0123456789";
			for (char i:kwd){
				if (acc.find(i) == string::npos){
					if (i=='.' && !found_comma) found_comma=true;
					else return false;
				}
			}
			if (found_comma) return true;
			return false;
		}

		bool is_integer_numeral(string kwd){ // returns true if the keyword is integer
			/**
			 * @brief Checks if a keyword is an integer
			 * @param kwd The keyword to check
			 * @return true if the keyword is an integer, false otherwise
			 */
			string acc="0123456789";
			for (char i:kwd){
				if (acc.find(i) == string::npos){
					return false;
				}
			}
			return true;
		}

		bool is_string_literal(string kwd){ // returns true if the keyword is string literal
			/**
			 * @brief Checks if a keyword is a string literal
			 * @note It searches the heads of the keyword for quotation marks
			 * @return true if the keyword is a string literal, false otherwise
			 */
			if (kwd[0]=='"' && kwd[kwd.size()-1]=='"') return true;
			return false;
		}
		
		string get_type(string kwd){
			/**
			 * @brief Checks the type of the keyword, using the implemented functions
			 * @return the type of the keyword in a string literal
			 */
			if (is_integer_numeral(kwd)) return "int";
			else if (is_float_numeral(kwd)) return "float";
			else if (is_string_literal(kwd)) return "string";
			else return "NDT"; // non determined type
		}
};	
