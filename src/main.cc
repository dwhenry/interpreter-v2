#include "parser.h"
#include <iostream>
#include <string>

int main( int argc, char * argv[] ) {
  std::string command;
  Parser * parser = new Parser();

  while(true) {
    try {
      std::cout << "Enter command to process (exit to quit): ";
      getline(std::cin, command);
      if(command == "exit") return 0;
      std::cout << "Result of " << command << " is: " << parser->process(command) << "\n";
    } catch(std::string e) {
      std::cout << "An error occurred: " << e << std::endl;
    } catch(char const* e) {
      std::cout << "An error occurred: " << e << std::endl;
    }
  }
}
