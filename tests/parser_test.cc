#include "catch.hpp"
#include "../src/parser.h"

TEST_CASE("Simple parser", "[int]") {
  Parser * parser = new Parser();

  SECTION("Addition") {
    CHECK( parser->process("return 1+2") == "3" );
    CHECK( parser->process("return 1 + 2") == "3" );
    CHECK( parser->process("return 12 +24") == "36" );
    CHECK_THROWS_WITH(
      parser->process("return 1 1"),
      Catch::Contains( "Expected 'EOF' got: 1" )
    );
    CHECK( parser->process("12 +24") == "36" );
  }

  SECTION("Negation") {
    CHECK( parser->process("return 1-2") == "-1" );
    CHECK( parser->process("return 2 - 1") == "1" );
    CHECK( parser->process("return 12 -24") == "-12" );
  }

  SECTION("Multiplication") {
    CHECK( parser->process("return 3*2") == "6" );
  }

  SECTION("Division") {
    CHECK( parser->process("return 6/2") == "3" );
    CHECK( parser->process("return 7/2") == "3" );
  }

  SECTION("Precedence of multiplication/division over addition/subtraction") {
    CHECK( parser->process("return 1 + 6 * 2") == "13" );
    CHECK( parser->process("return 10 - 20 / 4") == "5" );
    CHECK_THROWS_WITH(
      parser->process("return 1 *"),
      Catch::Contains( "Invalid token: " )
    );
  }

  SECTION("Brackets and stuff") {
    CHECK( parser->process("return (1 + 6)") == "7" );
    CHECK( parser->process("return (1+ 6) * 2") == "14" );
    CHECK_THROWS_WITH(
      parser->process("return (1+ 6"),
      Catch::Contains( "Expected ')' got: " )
    );
  }

  SECTION("Unairy operators") {
    CHECK( parser->process("return + 1") == "1" );
    CHECK( parser->process("return - 1") == "-1" );
    CHECK( parser->process("return -- 1") == "1" );
    CHECK( parser->process("return 1 + - 1") == "0" );
    CHECK( parser->process("return 1+--(2 * 3)") == "7" );
  }

  SECTION("Multi settlement") {
    CHECK( parser->process(R"(
      a = 1 + 3;
      return a
    )") == "4" );
    CHECK( parser->process(R"(
      a = 3;
      a = a + 3;
      return a
    )") == "6" );
    CHECK( parser->process(R"(
      a = 3;
      return a;
      return 3 + a
    )") == "3" );
    CHECK( parser->process(R"(
      a = 3;
      b = 5;
      a
    )") == "3" );
    CHECK( parser->process(R"(
      a = 3;
      b = 5;
      a;
      b
    )") == "5" );
    CHECK( parser->process(R"(
      a = 3;
      a + 3;
    )") == "6" );
    CHECK( parser->process(R"(
      a = 3;
      a = a + 3;
    )") == "6" );
    CHECK_THROWS_WITH(
      parser->process("return b"),
      Catch::Contains("Uninitialised variable: b")
    );
  }

  SECTION("if conditional") {
    CHECK( parser->process(R"(
      if 1
        3
      else
        5
      end
    )") == "3" );
    CHECK( parser->process(R"(
      if 0
        3
      else
        5
      end
    )") == "5" );
  }

  SECTION("Methods") {
    CHECK( parser->process(R"(
      def a
        3
      end
      5
    )") == "5" );
    CHECK( parser->process(R"(
      def a
        3
      end
      5
      a
    )") == "3" );
    CHECK( parser->process(R"(
      def a
        return 3
      end
      5
    )") == "5" );
    CHECK( parser->process(R"(
      b = 5
      def a
        b = 10
      end
      b
    )") == "5" );
  }
}
