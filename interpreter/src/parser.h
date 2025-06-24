/**
 * @file parser.h
 * @brief Header file for the parser component of the Roscript interpreter.
 * This file contains the declaration of the AST nodes and the parser functions.
 * @author Rares-Cosma & Vlad-Oprea
 * @date 2025-06-24
 */

#pragma once
#include "stdlib.cpp"
#include <vector>
#include <iostream>
#include <algorithm>

inline vector<string> arithmetic_operators = {"+", "-", "*", "/", "%"};
inline vector<string> comparison_operators = {"==", "!=", "<", ">", "<=", ">="};

inline string variant_to_string(const Value& v) {
	/**
	 * @brief Converts a Value variant to a string representation.
	 * @param v The Value variant to convert.
	 * @return A string representation of the Value.
	 */
    if (holds_alternative<int>(v)) return to_string(get<int>(v));
    if (holds_alternative<float>(v)) return to_string(get<float>(v));
    if (holds_alternative<string>(v)) return get<string>(v);
	if (holds_alternative<bool>(v)) return to_string(get<bool>(v));
    return "NDT"; // handle cases where the type is unsupported
}

/**
 * @class ASTNode
 * @brief Abstract base class for all AST nodes.
 * @note This class defines the interface for all AST nodes, including a virtual destructor and a pure virtual function for evaluation.
 */
class ASTNode {
	public:
		virtual ~ASTNode() = default;
		virtual void get(int indent = 0) const = 0;
		virtual Value eval() {}; // pure virtual function for evaluation
};

/**
 * @class Expr
 * @brief Abstract base class for all expressions in the AST.
 * @note This class inherits from ASTNode and defines the interface for expressions, including a pure virtual function for evaluation and a pure virtual function for printing.
 */
class Expr: public ASTNode {
	public:
		Value eval() override { cout<<"Base Expression"; }; // pure virtual function for evaluation
		virtual void print() const = 0;
		virtual Expr* clone() const = 0;
		void get(int indent = 0) const override {}
};

// STATEMENTS

/**
 * @class PrintStatement
 * @brief Represents a print statement in the AST.
 * @note This class inherits from ASTNode and contains an expression to be printed.
 */
class PrintStatement : public ASTNode {
	public:
		Expr* expr;
	
		PrintStatement(Expr* e) : expr(e) {}
	
		void get(int indent=0) const override {
			cout << "Print Statement: ";
			expr->get();
			cout << endl;
		}
	
		~PrintStatement() {
			delete expr;
		}
};

/**
 * @class AssignStatement
 * @brief Represents an assignment statement in the AST.
 * @note This class inherits from ASTNode and contains an expression and a variable name for the assignment.
 */
class AssignStatement : public ASTNode {
	public:
		Expr* expr;
		string name;
		
		AssignStatement(Expr* e, string name) : expr(e), name(name) {}
		
		void get(int indent=0) const override {
			cout << "Assignment Statement: ";
			cout << name << " = ";
			expr->get();
			cout << endl;
		}
		
		~AssignStatement() {
			delete expr;
		}
};

/**
 * @class IfStatement
 * @brief Represents an if statement in the AST.
 * @note This class inherits from ASTNode and contains an expression for the condition, a block of statements for the if branch, optional else-if branches, and an optional else block.
 * @details The else-if branches are stored as pairs of expressions and blocks, allowing for multiple conditions to be checked sequentially.
 */
class IfStatement : public ASTNode {
public:
    Expr* expr;
    vector<ASTNode*> block;
    vector<pair<Expr*, vector<ASTNode*>>> elseIfBranches;
    vector<ASTNode*> elseBlock;

    IfStatement(Expr* e, vector<ASTNode*> block,
                vector<pair<Expr*, vector<ASTNode*>>> elseIfBranches = {},
                vector<ASTNode*> elseBlock = {})
        : expr(e), block(block),
          elseIfBranches(elseIfBranches), elseBlock(elseBlock) {}

    void get(int indent = 0) const override {
        cout << string(indent, ' ') << "If Statement:\n";

        cout << string(indent + 2, ' ') << "Condition: ";
        expr->get();

        cout << string(indent + 2, ' ') << "Block:\n";
        for (const auto& node : block) {
            node->get(indent + 4);
        }

        for (const auto& branch : elseIfBranches) {
            cout << string(indent + 2, ' ') << "Else If Condition: ";
            branch.first->get();
            cout << string(indent + 2, ' ') << "Block:\n";
            for (const auto& node : branch.second) {
                node->get(indent + 4);
            }
        }

        if (!elseBlock.empty()) {
            cout << string(indent + 2, ' ') << "Else Block:\n";
            for (const auto& node : elseBlock) {
                node->get(indent + 4);
            }
        }
        cout << endl;
    }

    ~IfStatement() {
        delete expr;
        for (auto& branch : elseIfBranches) {
            delete branch.first;
            for (auto node : branch.second)
                delete node;
        }
        for (auto node : block)
            delete node;
        for (auto node : elseBlock)
            delete node;
    }
};

/**
 * @class WhileStatement
 * @brief Represents a while statement in the AST.
 * @note This class inherits from ASTNode and contains an expression for the condition and a block of statements to be executed while the condition is true.
 */
class WhileStatement : public ASTNode {
	public:
		Expr* expr;
		vector<ASTNode*> block;
		
		WhileStatement(Expr* e, vector<ASTNode*> block) : expr(e), block(block) {}
		
		void get(int indent=0) const override {
			cout << "While Statement: ";
			cout << string(indent + 2, ' ') << "Block:\n";
        	for (const auto& node : block) {
            	node->get(indent + 4);
        	}
			cout << endl;
		}
		
		~WhileStatement() {
			delete expr;
		}
};

/**
 * @class DoWhileStatement
 * @brief Represents a do-while statement in the AST.
 * @note This class inherits from ASTNode and contains an expression for the condition and a block of statements to be executed at least once before checking the condition.
 */
class DoWhileStatement : public ASTNode {
	public:
		Expr* expr;
		vector<ASTNode*> block;
		
		DoWhileStatement(Expr* e, vector<ASTNode*> block) : expr(e), block(block) {}
		
		void get(int indent=0) const override {
			cout << "Do while Statement: ";
			cout << string(indent + 2, ' ') << "Block:\n";
        	for (const auto& node : block) {
            	node->get(indent + 4);
        	}
			cout << endl;
		}
		
		~DoWhileStatement() {
			delete expr;
		}
};

/**
 * @class DoUntilStatement
 * @brief Represents a do-until statement in the AST.
 * @note This class inherits from ASTNode and contains an expression for the condition and a block of statements to be executed at least once before checking if the condition is false.
 */
class DoUntilStatement : public ASTNode {
	public:
		Expr* expr;
		vector<ASTNode*> block;
		
		DoUntilStatement(Expr* e, vector<ASTNode*> block) : expr(e), block(block) {}
		
		void get(int indent=0) const override {
			cout << "Do until Statement: ";
			cout << string(indent + 2, ' ') << "Block:\n";
        	for (const auto& node : block) {
            	node->get(indent + 4);
        	}
			cout << endl;
		}
		
		~DoUntilStatement() {
			delete expr;
		}
};

/**
* @class ForStatement
* @brief Represents a for statement in the AST.
* @note This class inherits from ASTNode and contains an initialization block, an expression for the condition, a block of statements to be executed, and an assignment block for the increment/decrement operation.
*/
class ForStatement : public ASTNode {
	public:
		Expr* expr;
		vector<ASTNode*> block;
		ASTNode *init_block,*assign_block;
		
		ForStatement(ASTNode* ib, Expr* e, vector<ASTNode*> block, ASTNode* ab) : init_block(ib), expr(e), block(block), assign_block(ab) {}
		
		void get(int indent=0) const override {
			cout << "For Statement: ";
			cout << string(indent + 2, ' ') << "Block:\n";
        	for (const auto& node : block) {
            	node->get(indent + 4);
        	}
			cout << endl;
		}
		
		~ForStatement() {
			delete expr;
		}
};

/**
 * @class InputStatement
 * @brief Represents an input statement in the AST.
 * @note This class inherits from ASTNode and contains a string representing the variable name to which the input will be assigned.
 */
class InputStatement : public ASTNode {
	public:
		string name;
		
		InputStatement(string e) : name(e) {}
		
		void get(int indent=0) const override {
			cout << "Input Statement: ";
			cout<<name;
			cout << endl;
		}
};

/**
 * @class VariableDeclaration
 * @brief Represents a variable declaration in the AST.
 * @note This class inherits from ASTNode and contains a string for the variable name, a string for the type, and an optional expression for the initial value.
 * @details If no initial value is provided, it defaults to a "No Default Type" (NDT) value.
 */
class VariableDeclaration : public ASTNode {
	public:
		string name, type;
		Expr* value;
		
	
		VariableDeclaration(string t, string n, Expr* v) : name(n), type(t), value(v) {}
	
		void get(int indent=0) const override {
			cout << "Variable Declaration: " << type << " " << name << " = ";
			if (value) {
				value->get();
			} else {
				cout << "NDT"; // No default type
			}
			cout << endl;
		}
	
		~VariableDeclaration() {
			delete value;
		}
};

// LITERALS

/**
 * @class IntLiteral
 * @brief Represents an integer literal in the AST, derrived from Expr.
 */
class IntLiteral : public Expr {
    int value;
	public:
    IntLiteral(int v) : value(v) {}
    Value eval() override { return value; }
	void get(int indent = 0) const override {}
	Expr* clone() const override {
        return new IntLiteral(value);
    }
	void print() const override {
		cout << value;
	}
};

/**
 * @class BoolLiteral
 * @brief Represents a boolean literal in the AST, derrived from Expr.
 */
class BoolLiteral : public Expr {
    bool value;
	public:
    BoolLiteral(bool v) : value(v) {}
    Value eval() override { return value; }
	void get(int indent = 0) const override {}
	Expr* clone() const override {
        return new BoolLiteral(value);
    }
	void print() const override {
		cout << value;
	}
};

/**
 * @class FloatLiteral
 * @brief Represents a float literal in the AST, derrived from Expr.
 */
class FloatLiteral : public Expr {
    float value;
	public:
    FloatLiteral(float v) : value(v) {}
    Value eval() override { return value; }
	void get(int indent = 0) const override {}
	Expr* clone() const override {
        return new FloatLiteral(value);
    }
	void print() const override {
		cout << value;
	}
};

/**
 * @class StringLiteral
 * @brief Represents a string literal in the AST, derrived from Expr.
 */
class StringLiteral : public Expr {
    string value;
	public:
    StringLiteral(string v) : value(move(v)) {}
    Value eval() override { return value; }
	void get(int indent = 0) const override {}
	Expr* clone() const override {
        return new StringLiteral(value);
    }
	void print() const override {
		cout << value;
	}
};

/**
 * @class Refrence
 * @brief Represents a variable reference in the AST, derrived from Expr.
 * @note This class allows access to variables defined in the global scope.
 * @details It retrieves the value of a variable from the global `variables` map.
 */
class Refrence : public Expr {
    string name;
	public:
	Refrence(string v) : name(move(v)) {}
    Value eval() override { return variables[name]; }
	void get(int indent = 0) const override {}
	Expr* clone() const override {
        return new Refrence(name);
    }
	void print() const override {
		cout << name;
	}
};

/**
 * @brief Calls a function from the standard library or user-defined functions.
 * @param name The name of the function to call.
 * @param args The arguments to pass to the function.
 */
inline Value callFunction(const string& name, const vector<Value>& args){
	auto it = stdlib.find(name);
    if (it == stdlib.end()) {
        throw std::runtime_error("Undefined function: " + name);
    }

	return it->second(args);
}

/**
 * @class FunctionCall
 * @brief Represents a function call in the AST, derrived from Expr.
 * @note This class allows calling functions defined in the standard library or user-defined functions.
 */
class FunctionCall : public Expr {
	public:
	string name;
	vector<Expr*> args;
	FunctionCall(string v, vector<Expr*> a) : name(move(v)), args(move(a)) {}
    Value eval() override {
		vector<Value> argValues;
		for (auto* arg : args) {
			argValues.push_back(arg->eval());
		}

		return callFunction(name, argValues);
	}
	void get(int indent = 0) const override {}
	Expr* clone() const override {
    	vector<Expr*> clonedArgs;
    	for (auto* arg : args) {
        	clonedArgs.push_back(arg->clone());
    	}
    	return new FunctionCall(name, move(clonedArgs));
	}
	void print() const override {
	}
};

/**
 * @class BinaryExpr
 * @brief Represents a binary expression in the AST, derrived from Expr.
 * @note This class handles binary operations between two expressions, such as arithmetic and comparison operations.
 * @details It evaluates the left and right expressions based on the operator provided.
 */
class BinaryExpr : public Expr {
	public:
	Expr* left;
    Expr* right;
    std::string op;
	BinaryExpr(Expr* l, string o, Expr* r) : left(l), op(move(o)), right(r) {}

	void get(int indent = 0) const override {}

	void print() const override {
		cout << "BinaryExpr(";
		left->print();
		cout << " " << op << " ";
		right->print();
		cout << ")";
	}

	Expr* clone() const override {
        return new BinaryExpr(left->clone(), op, right->clone());
    }

	Value eval() override
	{
		Value lval = left->eval();
		Value rval = right->eval();

		if (std::holds_alternative<int>(lval) && std::holds_alternative<int>(rval))
		{
			if (op == "+")
				return std::get<int>(lval) + std::get<int>(rval);
			if (op == "-")
				return std::get<int>(lval) - std::get<int>(rval);
			if (op == "*")
				return std::get<int>(lval) * std::get<int>(rval);
			if (op == "/")
				return std::get<int>(lval) / std::get<int>(rval);
			if (op == "%")
				return std::get<int>(lval) % std::get<int>(rval);
			if (op == "==")
				return std::get<int>(lval) == std::get<int>(rval);
			if (op == "!=")
				return std::get<int>(lval) != std::get<int>(rval);
			if (op == "<")
				return std::get<int>(lval) < std::get<int>(rval);
			if (op == ">")
				return std::get<int>(lval) > std::get<int>(rval);
			if (op == "<=")
				return std::get<int>(lval) <= std::get<int>(rval);
			if (op == ">=")
				return std::get<int>(lval) >= std::get<int>(rval);
		}

		if (std::holds_alternative<float>(lval) && std::holds_alternative<float>(rval))
		{
			if (op == "+")
				return std::get<float>(lval) + std::get<float>(rval);
			if (op == "-")
				return std::get<float>(lval) - std::get<float>(rval);
			if (op == "*")
				return std::get<float>(lval) * std::get<float>(rval);
			if (op == "/")
				return std::get<float>(lval) / std::get<float>(rval);
			if (op == "==")
				return std::get<float>(lval) == std::get<float>(rval);
			if (op == "!=")
				return std::get<float>(lval) != std::get<float>(rval);
			if (op == "<")
				return std::get<float>(lval) < std::get<float>(rval);
			if (op == ">")
				return std::get<float>(lval) > std::get<float>(rval);
			if (op == "<=")
				return std::get<float>(lval) <= std::get<float>(rval);
			if (op == ">=")
				return std::get<float>(lval) >= std::get<float>(rval);
		}

		if ((std::holds_alternative<int>(lval) && std::holds_alternative<float>(rval)) ||
			(std::holds_alternative<float>(lval) && std::holds_alternative<int>(rval)))
		{
			float lval_f = std::holds_alternative<int>(lval) ? static_cast<float>(std::get<int>(lval)) : std::get<float>(lval);
			float rval_f = std::holds_alternative<int>(rval) ? static_cast<float>(std::get<int>(rval)) : std::get<float>(rval);

			if (op == "+")
				return lval_f + rval_f;
			if (op == "-")
				return lval_f - rval_f;
			if (op == "*")
				return lval_f * rval_f;
			if (op == "/")
				return lval_f / rval_f;
			if (op == "==")
				return lval_f == rval_f;
			if (op == "!=")
				return lval_f != rval_f;
			if (op == "<")
				return lval_f < rval_f;
			if (op == ">")
				return lval_f > rval_f;
			if (op == "<=")
				return lval_f <= rval_f;
			if (op == ">=")
				return lval_f >= rval_f;
		}

		if (std::holds_alternative<string>(lval) && std::holds_alternative<string>(rval))
		{
			if (op=="+")
				return std::get<string>(lval) + std::get<string>(rval);
			if (op == "==")
				return std::get<string>(lval) == std::get<string>(rval);
			if (op == "!=")
				return std::get<string>(lval) != std::get<string>(rval);
		}

		throw std::runtime_error("Unsupported operation or mismatched types");
	}

	~BinaryExpr() {
        delete left;
        delete right;
    }
};

vector<ASTNode*> parse(vector<pair<string, string>> tokens, vector<int> tokens_per_line);
