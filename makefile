cc = gcc

CFLAGS = -std=c++11

all: main app_test

ast.o: src/ast.cc src/ast.h src/visitor.h
	g++ $(CFLAGS) -g -c src/ast.cc

scan.o: src/scan.h src/scan.cc source_file.o src/log.h
	g++ $(CFLAGS) -g -c src/scan.cc

source_file.o: src/source_file.cc src/source_file.h src/globals.h src/log.h
	g++ $(CFLAGS) -g -c src/source_file.cc

parser.o: src/parser.cc scan.o ast.o src/log.h
	g++ $(CFLAGS) -g -c src/parser.cc

main.o: src/main.cc parser.o
	g++ $(CFLAGS) -g -c src/main.cc

main: main.o
	g++ $(CFLAGS) -g -o main main.o source_file.o scan.o parser.o ast.o

parser_test.o: tests/parser_test.cc parser.o
	g++ $(CFLAGS) -g -c tests/parser_test.cc

test.o: tests/test.cc parser_test.o
	g++ $(CFLAGS) -g -c tests/test.cc

app_test: test.o
	g++ $(CFLAGS) -g -o app_test test.o parser_test.o parser.o scan.o source_file.o ast.o

clean:
	rm -f app_test main ./*.o

