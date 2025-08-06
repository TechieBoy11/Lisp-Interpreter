#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

// system includes
#include <map>

// module includes
#include "expression.hpp"

class Environment{
public:
  Environment();
  // Function to add a symbol-expression mapping to the environment
  void addSymbol(const Symbol& symbol, const Expression& expression);

  // Function to get the expression associated with a symbol from the environment
  Expression getExpression(const Symbol& symbol) const;
  bool hasSymbol(const Symbol& symbol) const;
  bool isNumber(std::string token);

  bool isSymbolDefined(const Symbol Symbol);
  double symbolValue(const Symbol symbol);
  
private:

  // Environment is a mapping from symbols to expressions or procedures
  enum EnvResultType {ExpressionType, ProcedureType};
  struct EnvResult{
    EnvResultType type;
    Expression exp;
    Procedure proc;
  };

  std::map<Symbol,EnvResult> envmap;

};

#endif
