#include "parser.h"
#include <sstream>

AST * Parser::factor() {
  Log::warn("Starting factor");
  AST * node;
  std::string tokenStr;

  switch(this->nextToken->type) {
  case TokenType::PLUS:
    // this can be ignored as it doesn't do anything
    this->eat();
    return this->factor();
  case TokenType::MINUS:
    this->eat();
    return new UniaryAST(this->factor(), TokenType::MINUS);
  case TokenType::NUM:
    int value;
    std::stringstream(this->nextToken->str) >> value;
    this->eat();
    return new NumAST(value);
  case TokenType::ID:
    tokenStr = this->nextToken->str;
    this->eat();
    if(this->nextToken->type == TokenType::ASSIGN) {
      Log::warn("Starting assignment");
      this->eat();
      return new AssignAST(tokenStr, this->exp());
    } else {
      return new VarAST(tokenStr);
    }
  case TokenType::L_BR:
    this->eat();
    node = this->exp();
    this->eat(TokenType::R_BR);
    return node;
  default:
    this->error("Invalid token: ");
    return NULL;
  }
}

AST * Parser::term() {
  Log::warn("Starting term");
  AST * node;
  bool process = true;
  std::stringstream result;

  node = this->factor();

  while(process) {

    switch(this->nextToken->type) {
    case TokenType::TIMES:
      this->eat();
      node = new BinAST(node, TokenType::TIMES, this->factor());
      break;
    case TokenType::DIVIDE:
      this->eat();
      node = new BinAST(node, TokenType::DIVIDE, this->factor());
      break;
    default:
      process = false;
      break;
    };
  }

  return node;
}

AST * Parser::exp() {
  Log::warn("Starting exp");
  AST * node = this->term();

  while(true) {
    switch(this->nextToken->type) {
    case TokenType::PLUS:
      this->eat();
      node = new BinAST(node, TokenType::PLUS, this->term());
      break;
    case TokenType::MINUS:
      this->eat();
      node = new BinAST(node, TokenType::MINUS, this->term());
      break;
    default:
      return node;
    };
  }
}

AST * Parser::defStatement() {
  Log::warn("Starting def");
  this->eat();
  std::string name = this->nextToken->str;
  this->eat(TokenType::ID);
  AST * node = new MethodAST(name, this->statementList());
  Log::info("Current token string:");
  Log::info(this->nextToken->str);
  this->eat(TokenType::END);
  return node;
}

AST * Parser::ifStatement() {
  Log::warn("Starting if");
  AST * cond;
  AST * trueBranch;
  AST * falseBranch;
  this->eat();
  cond = this->exp();
  trueBranch = this->statementList();
  if(this->nextToken->type == TokenType::ELSE) {
    this->eat();
    falseBranch = this->statementList();
  }
  this->eat(TokenType::END);
  return new ConditionAST(cond, trueBranch, falseBranch);
}

AST * Parser::returnStatement() {
  Log::warn("Starting return");
  Log::info("Current token string:");
  Log::info(this->nextToken->str);
  this->eat();
  Log::info("Current token string:");
  Log::info(this->nextToken->str);
  AST * node = new ReturnAST(this->exp());
  Log::info("expect end: Current token string:");
  Log::info(this->nextToken->str);

  return node;

}

AST * Parser::statement() {
  Log::warn("Starting Statement");
  switch(this->nextToken->type) {
  case TokenType::DEF:
    return this->defStatement();
    break;
  case TokenType::IF:
    return this->ifStatement();
    break;
  case TokenType::RETURN:
    return this->returnStatement();
    break;
  case TokenType::ID:
  case TokenType::NUM:
    return this->exp();
    break;
  default:
    return NULL;
  }
}

AST * Parser::statementList() {
  Log::warn("Starting StatementList");
  ListAST * list = new ListAST();
  AST * node;

  list->add(this->statement());

  while(true) {
    Log::error("Next statement: ", this->nextToken->str, " - ", this->nextToken->newLine);
    if(this->nextToken->type == TokenType::SEMI) {
      this->eat();
      list->add(this->statement());
    } else if(this->nextToken->newLine) {
      node = this->statement();
      if(node != NULL) {
        list->add(node);
      } else {
        return list;
      }
    } else {
      return list;
    }
  }

}

std::string Parser::process(std::string command) {
  std::stringstream result;

  this->scanner = new Scan(command);
  this->nextToken = this->scanner->next();
  AST * node;

  node = this->statementList();

  this->eat(TokenType::ENDFILE);

  Visitor v;
  node->accept(v);
  result << v.value();
  return result.str();
}

//=================================================================

void Parser::eat() {
  this->nextToken = this->scanner->next();
}

void Parser::eat(TokenType::TOKENS tokenType) {
  if(this->nextToken->type == tokenType) {
    this->nextToken = this->scanner->next();
  } else {
    std::stringstream token, msg;
    switch(tokenType) {
    case TokenType::ENDFILE:
      token << "EOF";
      break;
    default:
      for(int i=0; i < TOKENS_MAPS; i++) {
        if(tokenMap[i].token == tokenType) {
          token << std::string(1, tokenMap[i].current);
          if(tokenMap[i].preview != '\0') {
            token << std::string(1, tokenMap[i].preview);
          }
        }
      }
      for(int i = 0; i < RESERVED_MAPS; i++) {
        if(reservedMap[i].token == tokenType) {
          token << reservedMap[i].str;
        }
      }
    }
    msg << "Expected '" << token.str() << "' got: ";
    this->error(msg.str());
  }
}

void Parser::error(std::string msg) {
  std::stringstream result;
  result << msg << this->nextToken->str << " at:" << this->nextToken->lineNumber << ":" << this->nextToken->startPosition;
  throw result.str();
}
