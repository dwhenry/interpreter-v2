#include "source_file.h"
#include <sstream>
#include "log.h"

SourceFile::SourceFile(std::string command) {
  // std::ofstream myOutStream(myCurrentLogName.c_str(), std::ios::app);
  this->lineNumber = 0;
  this->position = 0;

  std::stringstream tmp;
  tmp << command;
  std::string line;

  while(getline(tmp, line, '\n')) {
    this->commands.push(line);
  }
  this->currentLine = this->commands.front();
  this->commands.pop();
  Log::error("Processing: ", this->currentLine);
};

int SourceFile::nextChar() {
  if(this->position >= this->currentLine.size()) {
    if(this->commands.empty()) {
      return EOF;
    } else {
      this->currentLine = this->commands.front();
      this->commands.pop();
      Log::error("Processing: ", this->currentLine);
      this->lineNumber++;
      this->position = 0;
    }
  }
  // std::cout << "char at: " << this->position << " of: " << this->currentLine.size() << " is: " << this->currentLine[this->position] << std::endl;
  return this->currentLine[this->position++];
};

int SourceFile::previewChar() {
  return this->currentLine[this->position];
};

void SourceFile::restoreChar() {
  if(this->position > 0)
    this->position--;;
};
