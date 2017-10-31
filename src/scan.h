#ifndef _SCAN_H_
#define _SCAN_H_

#include "globals.h"
#include "source_file.h"

typedef int (*checkFunction)(int);

class Scan {
  enum STATES { START, DONE, IS_NUMBER, IS_WORD, IS_COMMENT };

  int currentLine;
  SourceFile * sourceFile;
  TokenType::TOKENS lookup(std::string tokenString);
  void consumeComment();
  bool consumeIgnorables();
  void consumeWhile(char tokenString[], checkFunction func);

public:
  Scan(std::string command);
  TokenDetails * next();
};



#endif
