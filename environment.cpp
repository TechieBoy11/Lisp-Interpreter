#include "environment.hpp"
#include <iostream>

#include <cassert>
#include <cmath>

#include "interpreter_semantic_error.hpp"

Environment::Environment(){
  // TODO: implement your code ...
  // creat defulat enviroment
    envmap;
    envmap["pi"] = { ExpressionType, atan2(0, -1), nullptr };
  
}

void Environment::addSymbol(const Symbol& symbol, const Expression& expression) {
    EnvResult result;
    result.type = ExpressionType;
    result.exp = expression;
    envmap[symbol] = result;
}
bool Environment::hasSymbol(const Symbol& symbol) const {
    return envmap.find(symbol) != envmap.end();
}

bool Environment::isNumber(std::string token){
    try {
        double number = std::stod(token);
        return true;
    }
    catch (const std::invalid_argument&){
        return false; 
    }
}

Expression Environment::getExpression(const Symbol& symbol) const {
    auto it = envmap.find(symbol);
    if (it != envmap.end()) {
        return it->second.exp;
    } else {
        //addSymbol(symbol, Expression());
        return Expression();
    }
}

bool Environment::isSymbolDefined(const Symbol symbol) {
    return envmap.find(symbol) != envmap.end();
}

double Environment::symbolValue(const Symbol symbol) {
    return envmap.at(symbol).exp.head.value.num_value;
}


