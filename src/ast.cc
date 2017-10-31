#include "ast.h"
#include "visitor.h"


BinAST::BinAST(AST * left, TokenType::TOKENS op, AST * right) {
  this->left = left;
  this->op = op;
  this->right = right;
}
void BinAST::accept(Visitor &i)
{
  i.visit(this);
}

NumAST::NumAST(int value) {
  this->value = value;
}
void NumAST::accept(Visitor &i)
{
  i.visit(this);
}

UniaryAST::UniaryAST(AST * node, TokenType::TOKENS op) {
  this->op = op;
  this->node = node;
}
void UniaryAST::accept(Visitor &i)
{
  i.visit(this);
}

ListAST::ListAST() {}
void ListAST::add(AST * node) {
  if(node != NULL) {
    statements.push_back(node);
  }
}
void ListAST::accept(class Visitor &i) {
  i.visit(this);
}

AssignAST::AssignAST(std::string id, AST * value) {
  this->id = id;
  this->value = value;
}
void AssignAST::accept(class Visitor &i) {
  i.visit(this);
}

ReturnAST::ReturnAST(AST * value) {
  this->value = value;
}
void ReturnAST::accept(class Visitor &i) {
  i.visit(this);
}

VarAST::VarAST(std::string id) {
  this->id = id;
}
void VarAST::accept(class Visitor &i) {
  i.visit(this);
}

ConditionAST::ConditionAST(AST * cond, AST * trueBranch, AST * falseBranch) {
  this->cond = cond;
  this->trueBranch = trueBranch;
  this->falseBranch = falseBranch;
}
void ConditionAST::accept(class Visitor &i) {
  i.visit(this);
}

MethodAST::MethodAST(std::string name, AST * statementList) {
  this->name = name;
  this->statementList = statementList;
}

void MethodAST::accept(class Visitor &i) {
  i.visit(this);
}

