#include "interpreter.hpp"

// system includes
#include <stack>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <sstream>

// module includes
#include "tokenize.hpp"
#include "expression.hpp"
#include "environment.hpp"
#include "interpreter_semantic_error.hpp"

bool Interpreter::parse(std::istream & expression) noexcept{
  // sets up the interpreter 
  error = false;
  TokenSequenceType tokens = tokenize(expression);
  int open = 0;
  int close = 0;
  int count = 0;
  int i = 1;
  for (std::string token : tokens) {
    if (token == "(") {
        open += 1;
        count += 1;
    }else if (token == ")" ){
        close += 1;
        count -= 1;
    } 
    if (count == 0 && (tokens.size() != i)){

        return false;
    }
    i++;         
  }
  if (open != close || tokens[0] != "(")
    return false;     

  ast = token_to_AST(tokens);
  return !error;
};

Expression Interpreter::token_to_AST(TokenSequenceType& tokens) {
    std::string token = tokens.front();
    tokens.pop_front();
    if (tokens.size() == 1) {
        //std::string token = tokens.front(); // check declaration
        Atom atm;
        if (token_to_atom(token, atm))
            return Expression(atm);
    } if (token == "(") {
        Expression exp;
        token = tokens.front();
        Atom atm;
        if (token_to_atom(token, atm)) {
            exp.head = atm;
            tokens.pop_front();
        } else {
            error = true;
            return exp;
        }
        while (!tokens.empty() && tokens.front() != ")" ){
            exp.tail.push_back(token_to_AST(tokens));
        }
        tokens.pop_front();
        return exp;
    }
    Atom atm;
    if (token_to_atom(token, atm)){
        Expression exp(atm);
        return exp;
    }
        
}

Expression Interpreter::eval(){
  // turn tree into map envmap
  // Ensure that the AST is not empty
  addVar(var, "pi", atan2(0, -1));
    if (ast.tail.empty() && (ast.head.type == ListType || ast.head.type == NoneType)){
    throw InterpreterSemanticError("Empty AST");
    }
    if (ast.head.type == ListType) 
        return evalExpression(ast, env);
    else if (ast.head.type == SymbolType) {
        if (isVar(var, ast.head.value.sym_value)) {
            ast.head.value.num_value = var[ast.head.value.sym_value];
            ast.head.type = NumberType;
            return ast;
        } else 
            throw InterpreterSemanticError("Error: Symbol not found");
    } else 
        return ast;
}

Expression Interpreter::evalExpression( Expression& exp, Environment& env) {
    Expression result = exp.tail[1];
    Type type = exp.head.type;
    /*
    // enum Type {NoneType, BooleanType, NumberType, ListType, SymbolType};
    for (const auto& entry : var) {
        std::cout << "Variable: " << entry.first << ", Value: " << entry.second << std::endl;
    }
    
    std::cout << "Type: " << type << std::endl;
    std::cout << "Head Type: " << exp.head.type << std::endl;
    std::cout << "Head Symbol Value: " << exp.head.value.sym_value << std::endl << std::endl;

    for (int i = 0; i < exp.tail.size(); i++) {
        std::cout << "Tail Head Type " << i << ": " << exp.tail[i].head.type << std::endl;
        std::cout << "Tail Head Symbol Value " << i << ": " << exp.tail[i].head.value.sym_value << std::endl;
        std::cout << "Tail Head num Value " << i << ": " << exp.tail[i].head.value.num_value << std::endl;
        std::cout << std::endl;
    }
    */

    // assigns symbol expression to enviroment value
    for (int i = 0; i < exp.tail.size(); i++) {
        if (exp.tail[i].head.type == SymbolType && isVar(var, exp.tail[i].head.value.sym_value))
            exp.tail[i].head.value.num_value = var[exp.tail[i].head.value.sym_value];
    }

    if (type == NoneType) {
        return exp;
    } else if (type == NumberType) {
        if (exp.tail.size() != 0) 
            throw InterpreterSemanticError("Error: invalid expression");
        exp.head.value.bool_value = false;
        return exp;
    } else if (type == SymbolType) {
        if (var.find(exp.head.value.sym_value) == var.end()) 
            exp.head.value.num_value = var[exp.head.value.sym_value];
        return exp;
    } else if (type == BooleanType) {
        Symbol opperation = exp.head.value.sym_value;  
        if (type == BooleanType && (opperation == "True" || opperation == "False") ) {
            exp.head.value.num_value = (opperation == "True") ? 1 : 0;
            return exp; 
        }

    } else if (type == ListType) {
        Symbol opperation = exp.head.value.sym_value;
        if (opperation == "begin"){
            if (exp.tail[0].head.type == NoneType) 
                throw InterpreterSemanticError("Error: invalid begin expression");
            else {
                for (int i = 0; i < exp.tail.size(); i++){
                    if (exp.tail[i].head.type == ListType || (exp.tail[i].head.type == SymbolType && !isVar(var, exp.tail[i].head.value.sym_value))) {
                        evalExpression(exp.tail[i], env);
                    }                 
                }
                if (exp.tail[exp.tail.size()-1].head.type == SymbolType && isVar(var, exp.tail[exp.tail.size()-1].head.value.sym_value)) {
                    exp.head.value.num_value = var[exp.tail[exp.tail.size()-1].head.value.sym_value];
                    exp.head.type = NumberType;
                    return exp;
                } else
                    return exp.tail[exp.tail.size()-1];
            }
        }
        else if (opperation == "define") {
            if (exp.tail.size() != 2 || exp.tail[0].head.type != SymbolType) 
                throw InterpreterSemanticError("Error: invalid define expression");
            else if (exp.tail[1].head.type == ListType) 
                evalExpression(exp.tail[1], env);
            addVar(var, exp.tail[0].head.value.sym_value, exp.tail[1].head.value.num_value);
            exp.head.type = SymbolType;
            exp.head.value.sym_value = exp.tail[0].head.value.sym_value;
            exp.head.value.num_value = exp.tail[1].head.value.num_value;
            return exp;
        }
        else if (opperation == "if") {
            if (exp.tail.size() != 3) 
                throw InterpreterSemanticError("Error: invalid if expression");
            if (exp.tail[0].head.type != BooleanType) {
                if (exp.tail[0].head.type == ListType) 
                    evalExpression(exp.tail[0], env);
                if (exp.tail[0].head.type != BooleanType)
                    throw InterpreterSemanticError("Error: invalid boolean expression");
            } 
            for (int i = 1; i < 3; i++) {
                if (exp.tail[i].head.type != NumberType) 
                    if (exp.tail[i].head.type == ListType || (exp.tail[i].head.type == SymbolType && !isVar(var, exp.tail[i].head.value.sym_value))) 
                        evalExpression(exp.tail[i], env);
                    if (exp.tail[i].head.type != NumberType && exp.tail[i].head.type != SymbolType)
                        throw InterpreterSemanticError("Error: invalid number expression");
            }      
            if (exp.tail[0].head.value.bool_value) 
                exp.head.value.num_value = exp.tail[1].head.value.num_value;
            else 
                exp.head.value.num_value = exp.tail[2].head.value.num_value;
            exp.head.type = NumberType;
            return exp;
        } else if (isBool(opperation)){
            if (exp.tail.size() == 1) { 
                if (opperation == "not") {
                    if (exp.tail[0].head.type != BooleanType) {
                        if (exp.tail[0].head.type == ListType) 
                            evalExpression(exp.tail[0], env);
                        if (exp.tail[0].head.type != BooleanType)
                            throw InterpreterSemanticError("Error: invalid boolean expression");
                    }      
                    exp.head.value.bool_value = !exp.tail[0].head.value.bool_value;
                    return exp;
                    } else
                        throw InterpreterSemanticError("Error: invalid boolean expression");

            } else if (exp.tail.size() != 2)
                throw InterpreterSemanticError("Error: invalid boolean expression");
            else if (opperation == "and" || opperation == "or"){
                if (exp.tail[0].head.type != BooleanType ){
                    if (exp.tail[0].head.type == ListType) 
                        evalExpression(exp.tail[0], env);
                    if (exp.tail[0].head.type != BooleanType)
                        throw InterpreterSemanticError("Error: invalid boolean expression");
                }   else if (!exp.tail[1].head.type != BooleanType) {
                        if (exp.tail[1].head.type == ListType) 
                            evalExpression(exp.tail[1], env);
                        if (!exp.tail[1].head.type == BooleanType)
                            throw InterpreterSemanticError("Error: invalid boolean expression");
                    }
                if (!exp.tail[0].head.type == BooleanType || !exp.tail[1].head.type == BooleanType)
                    throw InterpreterSemanticError("Error: invalid boolean expression");

                bool logic1 = exp.tail[0].head.value.bool_value;
                bool logic2 = exp.tail[1].head.value.bool_value;
                if (opperation == "and") 
                    exp.head.value.bool_value = logic1 && logic2;
                else if (opperation == "or") 
                    exp.head.value.bool_value = logic1 || logic2;
                return exp;
            } else {
                for (int i=0; i<2; i++) {
                    if (exp.tail[i].head.type == ListType || (exp.tail[i].head.type == SymbolType && !isVar(var, exp.tail[i].head.value.sym_value))) 
                        evalExpression(exp.tail[i], env);
                    if (exp.tail[i].head.type != NumberType || exp.tail[i].head.type != SymbolType) 
                        throw InterpreterSemanticError("Error: invalid boolean expression");
                }
                exp.head.value.bool_value = calculateBool(exp.tail[0].head.value.num_value, exp.tail[2].head.value.num_value, opperation);
                return exp;
            }
        } else {
            Symbol opperation =  exp.head.value.sym_value; 
            std::vector<double> op;
            if ((opperation == "-" || opperation == "/" || opperation == "pow") && exp.tail.size() != 2) 
                throw InterpreterSemanticError("Error: invalid expression");
            else if (opperation == "log10" && exp.tail.size() != 1) 
                throw InterpreterSemanticError("Error: invalid expression");
            for (int i = 0; i < exp.tail.size(); i++) {
                if (exp.tail[i].head.type == NumberType ||(exp.tail[i].head.type == SymbolType && isVar(var, exp.tail[i].head.value.sym_value))) 
                    op.push_back(exp.tail[i].head.value.num_value);
                else if (exp.tail[i].head.type == ListType || exp.tail[i].head.type == SymbolType) {
                    evalExpression(exp.tail[i], env);
                    if (exp.tail[i].head.type != NumberType || exp.tail[i].head.type != SymbolType)
                        

                        //throw InterpreterSemanticError("Error: 1 invalid boolean expression");
                    op.push_back(exp.tail[i].head.value.num_value);
                } else 
                    throw InterpreterSemanticError("Error: invalid expression");
            }
            double result = calculateProcedure(op, opperation);
            exp.head.value.num_value = result;
            exp.head.type = NumberType;
            return exp;
        }   
    } else 
        throw InterpreterSemanticError("Error: invalid expression");
}

void Interpreter::addVar (std::map<Symbol, double> &var, Symbol sym, double num) {
    var[sym] = num;
}
bool Interpreter::isVar (std::map<Symbol, double> &var, Symbol sym) {
    if (var.find(sym) == var.end()) 
        return false;
    return true;
}

double Interpreter::calculateProcedure(std::vector<double> op, Symbol opperation) {
    double result = 0;
    if (opperation == "+") {
        for (int i = 0; i < op.size(); i++) {
            result += op[i];
        }
        return result;
    } else if (opperation == "-") 
        return op[0] - op[1];
    else if (opperation == "*") {
        result = 1;
        for (int i = 0; i < op.size(); i++) {
            result *= op[i];
        }
        return result;
    } else if (opperation == "/")
        return op[0] / op[1];
    else if (opperation == "log10") 
        return log10(op[0]);
    else if (opperation == "pow") 
        return pow(op[0], op[1]);
    else 
        throw InterpreterSemanticError("Error: invalid expression");
}

bool Interpreter::calculateBool(double op1, double op2, Symbol opperation) {
    if (opperation == "<")
        return op1 < op2;
    else if (opperation == "<=")
        return op1 <= op2;
    else if (opperation == ">")
        return op1 > op2;
    else if (opperation == ">=")
        return op1 >= op2;
    else if (opperation == "=")
        return op1 == op2;
    else 
        throw InterpreterSemanticError("Error: invalid boolean expression");
}
