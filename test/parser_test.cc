#include "parser.h"

#include <gtest/gtest.h>
#include <string>

TEST(Parser, SimpleAdd) {
  kungjs::Parser parser;
  bool result = parser.parse("1+2;");
  ASSERT_TRUE(result);
}

TEST(Parser, SimpleSubtract) {
  kungjs::Parser parser;
  bool result = parser.parse("11 - n;");
  ASSERT_TRUE(result);
}

TEST(Parser, SimpleDivision) {
  kungjs::Parser parser;
  bool result = parser.parse("x / 100.2;");
  ASSERT_TRUE(result);
}

TEST(Parser, SimpleMultiplication) {
  kungjs::Parser parser;
  bool result = parser.parse("23* 5;");
  ASSERT_TRUE(result);
}

TEST(Parser, CompositeArithmetic) {
  kungjs::Parser parser;
  bool result = parser.parse("1 + 23 * (5 + n/2) % 4 - 8;");
  ASSERT_TRUE(result);
}

TEST(Parser, NullProgram) {
  kungjs::Parser parser;
  bool result = parser.parse("null;");
  ASSERT_TRUE(result);
}

TEST(Parser, EmptyProgram) {
  kungjs::Parser parser;
  bool result = parser.parse("");
  ASSERT_TRUE(result);
}

TEST(Parser, NilProgram) {
  kungjs::Parser parser;
  bool result = parser.parse("\n");
  ASSERT_TRUE(result);
}

TEST(Parser, DoubleQuotedStringLiteral) {
  kungjs::Parser parser;
  bool result = parser.parse("\"the string lala\";");
  ASSERT_TRUE(result);
}

TEST(Parser, SingleQuotedStringLiteral) {
  kungjs::Parser parser;
  bool result = parser.parse("'the single';");
  ASSERT_TRUE(result);
}

TEST(Parser, Identifier) {
  kungjs::Parser parser;
  bool result = parser.parse("window;");
  ASSERT_TRUE(result);
}

TEST(Parser, This) {
  kungjs::Parser parser;
  bool result = parser.parse("this;");
  ASSERT_TRUE(result);
}

TEST(Parser, Booleans) {
  kungjs::Parser parser;
  bool result = parser.parse("true; false;");
  ASSERT_TRUE(result);
}

TEST(Parser, Assignment) {
  kungjs::Parser parser;
  bool result = parser.parse("var n  = 10;");
  ASSERT_TRUE(result);
}

TEST(Parser, MultipleAssignment) {
  kungjs::Parser parser;
  bool result = parser.parse("var n=10, i=20, s='some text';");
  ASSERT_TRUE(result);
}

TEST(Parser, SimpleForLoop) {
  kungjs::Parser parser;
  std::string code =
      "for(var i=0; i < 100; i++) {"
      "  100 - i;"
      "}";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, FunctionCall) {
  kungjs::Parser parser;
  std::string code = "veryUseful(10, \"arg2\");";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, FunctionDefinition) {
  kungjs::Parser parser;
  std::string code =
      "function veryUseful(arg1, arg2) {"
      "  return arg1 * arg2;"
      "}";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, If) {
  kungjs::Parser parser;
  std::string code =
      "if(n < 100) {"
      "  result = 'big';"
      "}";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, IfElse) {
  kungjs::Parser parser;
  std::string code =
      "if (t == 'some text') {"
      "  result -= 10;"
      "} else {"
      "  result += 20;"
      "}";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}


TEST(Parser, SimpleWith) {
  kungjs::Parser parser;
  std::string code =
      "with (self) {"
      "  call(10);"
      "  store('text');"
      "  someAttribute = value;"
      "}";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, WithDocument) {
  kungjs::Parser parser;
  std::string code =
      "with (document) {"
      "  write('the text');"
      "  write('more text');"
      "}";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, SimpleLabel) {
  kungjs::Parser parser;
  std::string code = "theLabel: var n = x + 10;";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, LabelledFor) {
  kungjs::Parser parser;
  std::string code =
      "start: for(var i=0; i < 100; i++) {"
      "  break start;"
      "}";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, Try) {
  kungjs::Parser parser;
  std::string code =
      "try {"
      "  some = 'text';"
      "  doSomethin();"
      "  someOne.action(true)"
      "}";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, TryCatch) {
  kungjs::Parser parser;
  std::string code =
      "try {"
      "  some = giveMeAnError('cfg');"
      "  neverExecuted();"
      "} catch (error) {"
      "  print(error.getMessage());"
      "}";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, TryFinally) {
  kungjs::Parser parser;
  std::string code =
      "try {"
      "  connection.open();"
      "  doAction();"
      "} finally {"
      "  connection.close();"
      "}";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, TryCatchFinally) {
  kungjs::Parser parser;
  std::string code =
      "try {"
      "  connection.begin();"
      "  doAction();"
      "  connection.commit();"
      "} catch (error) {"
      "  connection.rollback(error);"
      "} finally {"
      "  connection.close();"
      "}";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

