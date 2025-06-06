#pragma once
#include <variant>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;
using Value = variant<int, float, string, bool>;

inline vector<string> arithmetic_operators = {"+", "-", "*", "/"};
inline vector<string> comparison_operators = {"==", "!=", "<", ">", "<=", ">="};

inline string variant_to_string(const Value& v) {
    if (holds_alternative<int>(v)) return to_string(get<int>(v));
    if (holds_alternative<float>(v)) return to_string(get<float>(v));
    if (holds_alternative<string>(v)) return get<string>(v);
	if (holds_alternative<bool>(v)) return to_string(get<bool>(v));
    return "NDT"; // handle cases where the type is unsupported
}

class ASTNode {
	public:
		virtual ~ASTNode() = default;
		virtual void get(int indent = 0) const = 0;
		virtual Value eval() {}; // pure virtual function for evaluation
};

class Expr: public ASTNode {
	public:
		Value eval() override { cout<<"Base Expression"; }; // pure virtual function for evaluation
		virtual void print() const = 0;
		void get(int indent = 0) const override {}
};

// STATEMENTS

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

class IfStatement : public ASTNode {
	public:
		Expr* expr;
		vector<ASTNode*> block;
		
		IfStatement(Expr* e, vector<ASTNode*> block) : expr(e), block(block) {}
		
		void get(int indent=0) const override {
			cout << "If Statement: "<< endl;
			cout << string(indent+2, ' ') << "Condition: ";
			expr->get();
			cout << string(indent+2, ' ') << "Block: " << endl;
			for (const auto& node : block) {
				cout << string(indent+4, ' ');
				node->get(indent + 4);
			}
			cout << endl;
		}
		
		~IfStatement() {
			delete expr;
		}
	};

class InputStatement : public ASTNode {
	public:
		Expr* expr;
		
		InputStatement(Expr* e) : expr(e) {}
		
		void get(int indent=0) const override {
			cout << "Input Statement: ";
			expr->get();
			cout << endl;
		}
		
		~InputStatement() {
			delete expr;
		}
	};

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

class IntLiteral : public Expr {
    int value;
	public:
    IntLiteral(int v) : value(v) {}
    Value eval() override { return value; }
	void get(int indent = 0) const override {}
	void print() const override {
		cout << value;
	}
};

class BoolLiteral : public Expr {
    bool value;
	public:
    BoolLiteral(bool v) : value(v) {}
    Value eval() override { return value; }
	void get(int indent = 0) const override {}
	void print() const override {
		cout << value;
	}
};

class FloatLiteral : public Expr {
    float value;
	public:
    FloatLiteral(float v) : value(v) {}
    Value eval() override { return value; }
	void get(int indent = 0) const override {}
	void print() const override {
		cout << value;
	}
};

class StringLiteral : public Expr {
    string value;
	public:
    StringLiteral(string v) : value(move(v)) {}
    Value eval() override { return value; }
	void get(int indent = 0) const override {}
	void print() const override {
		cout << value;
	}
};

class Refrence : public Expr {
    string name;
	public:
	Refrence(string v) : name(move(v)) {}
    Value eval() override { return name; }
	void get(int indent = 0) const override {}
	void print() const override {
		cout << name;
	}
};

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

vector<ASTNode*> parse(vector<pair<string, string>> tokens);
