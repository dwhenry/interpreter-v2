#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <string>

namespace TokenType {
  enum TOKENS {
    /* book-keeping tokens */
    ENDFILE, //ERROR,
    /* reserved words */
    DEF, IF, ELSE, END, RETURN, WHILE,
    /* multicharacter tokens */
    ID,NUM,
    /* special symbols */
    PLUS,MINUS,TIMES,DIVIDE,LT,LTEQ,GT,GTEQ,EQ,NOTEQ,ASSIGN,SEMI,COMMA,
    /* brackets */
    L_BR,R_BR,L_SQUARE_BR,R_SQUARE_BR,L_SQUIGGLE_BR,R_SQUIGGLE_BR
  };
}

typedef struct tokenDetails {
  TokenType::TOKENS type;
  std::string str;
  int lineNumber;
  int startPosition;
  bool newLine;
} TokenDetails;

#define TOKENS_MAPS 19
static struct
  {
    int current;
    int preview;
    TokenType::TOKENS token;
  } tokenMap[TOKENS_MAPS]
  = {
      {'+', '\0', TokenType::PLUS},
      {'-', '\0', TokenType::MINUS},
      {'*', '\0', TokenType::TIMES},
      {'/', '\0', TokenType::DIVIDE},
      {'<', '=', TokenType::LTEQ},
      {'<', '\0', TokenType::LT},
      {'>', '=',TokenType::GTEQ},
      {'>', '\0', TokenType::GT},
      {'=', '=', TokenType::EQ},
      {'=', '\0', TokenType::ASSIGN},
      {'!', '=', TokenType::NOTEQ},
      {';', '\0', TokenType::SEMI},
      {',', '\0', TokenType::COMMA},

      {'(', '\0', TokenType::L_BR},
      {')', '\0', TokenType::R_BR},
      {'[', '\0', TokenType::L_SQUARE_BR},
      {']', '\0', TokenType::R_SQUARE_BR},
      {'{', '\0', TokenType::L_SQUIGGLE_BR},
      {'}', '\0', TokenType::R_SQUIGGLE_BR}
    };

#define RESERVED_MAPS 6
static struct
  {
    std::string str;
    TokenType::TOKENS token;
  } reservedMap[RESERVED_MAPS]
  = {
      {"def", TokenType::DEF},
      {"if", TokenType::IF},
      {"else", TokenType::ELSE},
      {"end", TokenType::END},
      {"return", TokenType::RETURN},
      {"while", TokenType::WHILE},
    };

#define MAX_TOKEN_LENGTH 40

#endif
