#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

// system includes
#include <string>
#include <istream>
#include <vector>


// module includes
#include "expression.hpp"
#include "environment.hpp"
#include "tokenize.hpp"

// Interpreter has
// Environment, which starts at a default
// parse method, builds an internal AST
// eval method, updates Environment, returns last result
class Interpreter{
public:
  bool parse(std::istream & expression) noexcept;
  Expression eval();

  Expression token_to_AST(TokenSequenceType& tokens);
  Expression evalExpression(Expression &exp, Environment &env);

private:
  Environment env;
  Expression ast;
  std::map<Symbol, double> var;
  void addVar (std::map<Symbol, double> &var, Symbol sym, double num);
  bool isVar (std::map<Symbol, double> &var, Symbol sym);
  
  bool error;

  bool calculateBool(double op1, double op2, std::string opperation);
  double calculateProcedure(std::vector<double> op, Symbol opperation);
};


#endif
