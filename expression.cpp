#include "expression.hpp"

#include <cmath>
#include <limits>
#include <cctype>
#include <deque>

// system includes
#include <sstream>
#include <iostream>


Expression::Expression(bool tf){
  head.type = BooleanType;
  head.value.bool_value = tf;
}

Expression::Expression(double num){
  head.type = NumberType;
  head.value.num_value = num;
}

Expression::Expression(const std::string & sym){
  head.type = SymbolType;
  head.value.sym_value = sym;
}

bool Expression::operator==(const Expression & exp) const noexcept{
    if (head.type != exp.head.type) {
        return false;
    }
    switch (head.type) {
    case NoneType:
        return true;
    case BooleanType:
        return head.value.bool_value == exp.head.value.bool_value;
    case NumberType:
        return head.value.num_value == exp.head.value.num_value;
    case SymbolType:
        return head.value.sym_value == exp.head.value.sym_value;
    default:
        return false;
    }
  return false;
}

std::ostream & operator<<(std::ostream & out, const Expression & exp){
  switch (exp.head.type) {
    case NoneType:
        out << "None";
        break;
    case BooleanType:
        out << (exp.head.value.bool_value ? "True" : "False");
        break;
    case NumberType:
        out << exp.head.value.num_value;
        break;
    case SymbolType:
        out << exp.head.value.sym_value;
        break;
    default:
        out << "Unknown type";
    }

    if (!exp.tail.empty()) {
        out << " (";
        for (size_t i = 0; i < exp.tail.size(); ++i) {
            out << exp.tail[i];
            if (i != exp.tail.size() - 1) {
                out << " ";
            }
        }
        out << ")";
    }

    return out;
}

bool token_to_atom(const std::string & token, Atom & atom){
    if (token == "True") {
        atom.value.bool_value = true;
        atom.type = BooleanType;
        atom.value.sym_value = token;
        return true;
    } else if (token == "False") {
        atom.type = BooleanType;
        atom.value.bool_value = false;
        atom.value.sym_value = token;
        return true;
    }
    else if (isProc(token)) { 
        atom.type = ListType;
        atom.value.sym_value = token;
        return true;
    } else if (isNumber(token)) {
        atom.type = NumberType;
        atom.value.num_value = findNumber(token);
        atom.value.sym_value = token;
        return true;
    } else if (isSymbol(token)){
        atom.type = SymbolType;
        atom.value.sym_value = token;
        return true;
    } else 
        return false;
}

bool isProc(const Symbol &token){
  const std::deque<std::string> expressions = {"define", "begin", "if", "*", "/", "+", "-", "log10", "pow", 
                                                "not", "and", "or", "<", "<=", ">", ">=", "=",};
  for (const std::string& i : expressions) {
    if (i == token) {
        return true;
    }
  }
  return false;
}
bool isBool(const Symbol &token){
    const std::deque<std::string> expressions = {"not", "and", "or", "<", "<=", ">", ">=", "="};
    for (const std::string& i : expressions) {
        if (i == token) {
            return true; 
        }
    }
  return false;
}
bool isSymbol(const Symbol &token){
    if (!std::isalpha(token[0])) 
        return false;
    return true;
}
bool isNumber(const Symbol &token){
    bool sign = false;
    bool e = false;
    bool dec = false;

    for (size_t i = 0; i < token.size(); ++i) {
        char c = token[i];
        if (!std::isdigit(c) && c != '.' && c != 'e' && c != '-' && c != '+') 
            return false;
        if (c == '-' || c == '+') {
            if (i != 0 && token[i - 1] != 'e') 
                return false;
            if (sign) 
                return false;
            sign = true;
        } else if (c == 'e') {
            if (e) 
                return false;
            e = true;
            sign = false; 
            dec = false;
        } else if (c == '.') {
            if (dec) 
                return false;
            dec = true;
        }
    }
    return true;
}
double findNumber(const Symbol &token){
    double num = 0;
    double exponent = 1;
    bool power = false;

    int size = token.size();

    for (int i = 0; i < size; ++i) {
        char c = token[i];
        if (power){
            if  (c == '-') {
                exponent = std::stod(token.substr(i+1, size+1));
                exponent *= -1;
            } else if (c == '+')
                exponent = std::stod(token.substr(i+1, size+1));
            else 
                exponent = std::stod(token.substr(i, size+1));
            break;
        } else if (c == 'e')
            power = true;
        else 
            continue;
    }
    if (power) {
        if (token[0] == '+')
            num = std::stod(token.substr(1, token.find('e')-1));
        else if (token[0] == '-')
            num = std::stod(token.substr(1, token.find('e')-1)) * -1;
        else 
            num = std::stod(token.substr(0, token.find('e')));
        return num * pow(10, exponent);
    }  else {
        if (token[0] == '+')
            num = std::stod(token.substr(1, size-1));
        else if (token[0] == '-')
            num = std::stod(token.substr(1, size-1)) * -1;
        else 
            num = std::stod(token);
        return num;
    }
}

