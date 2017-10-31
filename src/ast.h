#ifndef _AST_H_
#define _AST_H_

#include "globals.h"
#include <iostream>
#include <vector>

class AST {
public:
  virtual void accept(class Visitor &i) = 0;

};

class BinAST : public AST {
public:
  AST * left;
  TokenType::TOKENS op;
  AST * right;
  BinAST(AST * left, TokenType::TOKENS op, AST * right);
  virtual void accept(class Visitor &i);
};

class NumAST : public AST {
public:
  int value;
  NumAST(int value);
  virtual void accept(class Visitor &i);
};

class UniaryAST : public AST {
public:
  AST * node;
  TokenType::TOKENS op;
  UniaryAST(AST * node, TokenType::TOKENS op);
  virtual void accept(class Visitor &i);
};

class ListAST : public AST {
public:
  std::vector<AST *> statements;
  ListAST();
  void add(AST * node);
  virtual void accept(class Visitor &i);
};

class AssignAST : public AST {
public:
  std::string id;
  AST * value;
  AssignAST(std::string id, AST * value);
  virtual void accept(class Visitor &i);
};

class ReturnAST : public AST {
public:
  AST * value;
  ReturnAST(AST * value);
  virtual void accept(class Visitor &i);
};

class VarAST : public AST {
public:
  std::string id;
  VarAST(std::string id);
  virtual void accept(class Visitor &i);
};


class ConditionAST : public AST {
public:
  AST * cond;
  AST * trueBranch;
  AST * falseBranch;
  ConditionAST(AST * cond, AST * trueBranch, AST * FalseBranch);
  virtual void accept(class Visitor &i);
};

class MethodAST : public AST {
public:
  std::string name;
  AST * statementList;
  MethodAST(std::string name, AST * statementList);
  virtual void accept(class Visitor &i);
};

#endif
