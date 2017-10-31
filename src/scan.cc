#include "globals.h"
#include "scan.h"
#include <sstream>
#include "log.h"

void Scan::consumeComment() {
  int c;
  this->sourceFile->nextChar(); // consume the previewed character
  while(true) {
    c = this->sourceFile->nextChar();
    if(c == '*' && this->sourceFile->previewChar() == '/') {
      this->sourceFile->nextChar(); // consume the previewed character
      return;
    } else if(c == EOF) {
      throw "Unclosed comment.";
    }
  }
};

bool Scan::consumeIgnorables() {
  int c;
  while(true) {
    c = this->sourceFile->nextChar();
    if ((c == ' ') || (c == '\t') || (c == '\n')) {
      // noop
    } else if(c == EOF) {
      // std::cout << "hit EOF";
      return false;
    } else if(c == '/' && this->sourceFile->previewChar() == '*') {
      consumeComment();
    } else {
      this->sourceFile->restoreChar();
      return true;
    }
  }
}

void Scan::consumeWhile(char tokenString[], checkFunction func) {
  int pos = 1;
  int c;
  while(func(c = this->sourceFile->nextChar())) {
    tokenString[pos++] = c;
  }
  tokenString[pos] = '\0';
  if(c != EOF)
    this->sourceFile->restoreChar();
}

TokenType::TOKENS Scan::lookup(std::string tokenString) {
  for(int i = 0; i < RESERVED_MAPS; i++) {
    if(reservedMap[i].str == tokenString) {
      return reservedMap[i].token;
    }
  }
  return TokenType::ID;
}

TokenDetails * Scan::next() {
  int c;
  char tokenString[MAX_TOKEN_LENGTH];

  TokenDetails * token = new TokenDetails();
  token->newLine = false;

  // consume comments and spacing
  if(!consumeIgnorables()) {
    // std::cout << "EOF standard";
    token->type = TokenType::ENDFILE;
    return token;
  }

  token->lineNumber = this->sourceFile->lineNumber;
  token->startPosition = this->sourceFile->position;

  tokenString[0] = c = this->sourceFile->nextChar();

  if(isdigit(c)) {
    // std::cout << "number start" << c << std::endl;
    consumeWhile(tokenString, &isdigit);
    token->type = TokenType::NUM;
  } else if (isalpha(c)) {
    consumeWhile(tokenString, &isalpha);
    token->type = lookup((std::string)tokenString);
  } else {
    int p = this->sourceFile->previewChar();
    for(int i=0; i < TOKENS_MAPS; i++) {
      if(tokenMap[i].current == c && (tokenMap[i].preview == '\0' || tokenMap[i].preview == p)) {
        tokenString[1] = tokenMap[i].preview;
        tokenString[2] = '\0';
        if(tokenString[1] != '\0') this->sourceFile->nextChar();
        token->type = tokenMap[i].token;
        break;
      }
    }
    if(!token->type) {
      tokenString[1] = '\0';
      std::cout << "Parse error on string: " << (std::string)tokenString << c << std::endl;
      throw "Parse error, invalid character";
    }
  }

  token->str = tokenString;
  token->newLine = this->currentLine != token->lineNumber;
  this->currentLine = token->lineNumber;
  return token;
};

Scan::Scan(std::string command) {
  currentLine = 0;
  this->sourceFile = new SourceFile(command);
};
