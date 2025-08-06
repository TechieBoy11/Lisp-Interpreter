#include "tokenize.hpp"
#include <cctype>

#include <iostream>

TokenSequenceType tokenize(std::istream & seq){
  TokenSequenceType tokens;

  // TODO: implement your code here
 std::string token = "";      // used to creat current token
 char i;                      // place holder for iterating char
 bool read = true;            // returns false when ";" is reached meaning end of content


// itterates through "seq" and add tokens to tokens (list of all tokens)
 while (seq.get(i) && read) {
    if (token.size() != 0) { // when token is not empty
      // handles special cases where  token is done
      if (i == ' ' || i == OPEN || i == CLOSE || i == COMMENT) { 
        tokens.push_back(token);
        token = "";
      }
    }

    // bracket cases
    if (i == OPEN || i == CLOSE) {
      token = i;
      tokens.push_back(token);
      token = "";
    }
    // end of line case
    else if (i == COMMENT) {
      read = false;
    }
    else if (i != ' ') {
      // continue adding to current token
      token.push_back(i);
    }

  }
  if (token.size() > 0) {
    tokens.push_back(token);
  }
  
  return tokens;
}

