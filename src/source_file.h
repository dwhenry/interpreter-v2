#ifndef _SOURCE_FILE_H_
#define _SOURCE_FILE_H_

#include <fstream>
#include <iostream>
#include <string>
#include <queue>

class SourceFile {
private:
  std::queue<std::string> commands;
  std::string currentLine;
public:
  int lineNumber;
  int position;
  SourceFile() {}; // for testing
  SourceFile(std::string command);
  virtual int nextChar();
  virtual int previewChar();
  virtual void restoreChar();
};

#endif
