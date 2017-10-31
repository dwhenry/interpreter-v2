#ifndef _VISITOR_H_
#define _VISITOR_H_

#include "globals.h"
#include <sstream>
#include <stack>
#include <map>

class Visitor {
  std::stack<int> stack;
  std::map<std::string, int> memory;
  std::map<std::string, AST *> methods;
  bool exit = false;
  int lastValue;

public:
  int value() {
    return lastValue;
  }

  void visit(BinAST * node) {
    if(this->exit) return;
    node->left->accept(*this);
    if(this->exit) return;
    node->right->accept(*this);

    int right = this->stack.top();
    this->stack.pop();
    int left = this->stack.top();
    this->stack.pop();

    switch(node->op) {
    case TokenType::PLUS:
      this->stack.push(left + right);
      break;
    case TokenType::MINUS:
      this->stack.push(left - right);
      break;
    case TokenType::TIMES:
      this->stack.push(left * right);
      break;
    case TokenType::DIVIDE:
      this->stack.push(left / right);
      break;
    default:
      std::stringstream result;
      result << "Unexpected operation: " << node->op << "valid '+' = " << TokenType::PLUS;
      throw result.str();
    }
  }

  void visit(NumAST * node) {
    this->stack.push(node->value);
  }

  void visit(UniaryAST * node) {
    if(this->exit) return;
    node->node->accept(*this);

    if(node->op == TokenType::MINUS) {
      int value = this->stack.top();
      this->stack.pop();
      this->stack.push(-1 * value);
    }
  }

  void visit(ListAST * node) {
    for(AST * n : node->statements) {
      if(this->exit) return;
      n->accept(*this);
      if(this->stack.size() == 1) {
        this->lastValue = this->stack.top();
        this->stack.pop();
      } else if(this->stack.size() > 1) {
        std::stringstream result;
        result << "Elements left on the stack: " << this->stack.size();
        throw result.str();
      }
    }
  }

  void visit(AssignAST * node) {
    if(this->exit) return;
    node->value->accept(*this);

    this->memory[node->id] = this->stack.top();
    this->stack.pop();
  }

  void visit(ReturnAST * node) {
    if(this->exit) return;
    node->value->accept(*this);
    this->exit = true;
    // TODO: need to break out of the call chain
  }

  void visit(VarAST * node) {
    if(this->memory.count(node->id) != 0) {
      this->stack.push(this->memory[node->id]);
    } else if(this->methods.count(node->id) != 0) {
      Visitor methodVisitor;
      this->methods[node->id]->accept(methodVisitor);
      this->stack.push(methodVisitor.value());
    } else {
      std::stringstream result;
      result << "Uninitialised variable: " << node->id;
      throw result.str();
    }
  }

  void visit(ConditionAST * node) {
    if(this->exit) return;
    node->cond->accept(*this);
    int check = this->stack.top();
    this->stack.pop();

    if(this->exit) return;
    if(check > 0) {
      node->trueBranch->accept(*this);
    } else if(node->falseBranch != NULL) {
      node->falseBranch->accept(*this);
    }
  }

  void visit(MethodAST * node) {
    if(this->exit) return;
    this->methods[node->name] = node->statementList;
  }
};

#endif
