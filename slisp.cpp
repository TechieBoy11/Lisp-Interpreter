#include <cstdlib>
#include <iostream>
#include "interpreter.hpp"
#include <fstream>
#include <sstream>
#include <istream>


bool programHandler(std::istream& in, int type, std::string file) {
  Interpreter interpreter;
  bool error = false;
  std::istringstream expression;

  if (type == 1) {
   error = !interpreter.parse(in);
   if (!error) {
      Expression result = interpreter.eval();
      if (result.head.type == NumberType || result.head.type == SymbolType)
        std::cout  <<"(" << result.head.value.num_value << ")" << std::endl; 
      else 
        std::cout << "(" << result.head.value.sym_value << ")" << std::endl;
   }
  }

  else if (type == 2) {
    // reads code from file

    std::ifstream inputFile;
    inputFile.open(file);
    error = !(inputFile.is_open()); 
    if (error) {
      return EXIT_FAILURE;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
      std::istringstream fileIn(line);
      error = !interpreter.parse(fileIn);
      if (!error) {
        Expression result = interpreter.eval(); 
        std::cout << "(" << result.head.value.num_value << ")" << std::endl; 
        std::cout << result.head.type << std::endl;
      }
    }
    inputFile.close();
  }

  else if (type == 3) {
    while (true) {
      std::string tempIn;
      std::getline(std::cin, tempIn);;
      expression = std::istringstream(tempIn);
      error = interpreter.parse(expression);
      if (!error) {
        Expression result = interpreter.eval(); 
        std::cout << "(" << result << ")" << std::endl; 
      }
    }
  }
  else {
    std::cout << "Error distinguishing type";
    return EXIT_FAILURE;
  }
  return error;
}


int main(int argc, char **argv)
{
  ////////////////input haldler//////////////////
  
  std::string in;
  int type;
  std::string file;
  // the first value in the list is the output type
  // 1 = simple command line in
  // 2 = Exicuting program file
  // 3 = REPL

  if (argc == 1) {
    type = 3;
  }
  else if (std::string(argv[1]) == "-e") {
    type = 1;
    in = argv[2];
  }
  else {
    type = 2;
    file = argv[1];
  }

  std::istringstream inStream(in);
  ////////////////////opperations////////////////////
  return programHandler(inStream, type, file);
 
  
  return 0;
}


