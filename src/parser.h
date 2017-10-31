#ifndef _INTERGPRETER_H_
#define _INTERGPRETER_H_

#include "globals.h"
#include "ast.h"
#include "visitor.h"
#include "scan.h"
#include <iostream>
#include <string>
#include "log.h"

class Parser {
  Scan * scanner;
  TokenDetails * nextToken;
  void eat();
  void eat(TokenType::TOKENS tokenType);
  void error(std::string msg);
  AST * defStatement();
  AST * ifStatement();
  AST * assignmentStatement();
  AST * returnStatement();
  AST * statementList();
  AST * statement();
  AST * exp();
  AST * term();
  AST * factor();

public:
  Parser() {};
  virtual std::string process(std::string command);
};

#endif
