#include "kunjs/parser.h"

#include <gtest/gtest.h>
#include <string>

TEST(Parser, SimpleAdd) {
  kunjs::Parser parser;
  bool result = parser.parse("1+2;");
  ASSERT_TRUE(result);
}

TEST(Parser, SimpleSubtract) {
  kunjs::Parser parser;
  bool result = parser.parse("11 - n;");
  ASSERT_TRUE(result);
}

TEST(Parser, SimpleDivision) {
  kunjs::Parser parser;
  bool result = parser.parse("x / 100.2;");
  ASSERT_TRUE(result);
}

TEST(Parser, SimpleMultiplication) {
  kunjs::Parser parser;
  bool result = parser.parse("23* 5;");
  ASSERT_TRUE(result);
}

TEST(Parser, CompositeArithmetic) {
  kunjs::Parser parser;
  bool result = parser.parse("1 + 23 * (5 + n/2) % 4 - 8;");
  ASSERT_TRUE(result);
}

TEST(Parser, NullProgram) {
  kunjs::Parser parser;
  bool result = parser.parse("null;");
  ASSERT_TRUE(result);
}

TEST(Parser, EmptyProgram) {
  kunjs::Parser parser;
  bool result = parser.parse("");
  ASSERT_TRUE(result);
}

TEST(Parser, NilProgram) {
  kunjs::Parser parser;
  bool result = parser.parse("\n");
  ASSERT_TRUE(result);
}

TEST(Parser, DoubleQuotedStringLiteral) {
  kunjs::Parser parser;
  bool result = parser.parse("\"the string lala\";");
  ASSERT_TRUE(result);
}

TEST(Parser, SingleQuotedStringLiteral) {
  kunjs::Parser parser;
  bool result = parser.parse("'the single';");
  ASSERT_TRUE(result);
}

TEST(Parser, Identifier) {
  kunjs::Parser parser;
  bool result = parser.parse("window;");
  ASSERT_TRUE(result);
}

TEST(Parser, This) {
  kunjs::Parser parser;
  bool result = parser.parse("this;");
  ASSERT_TRUE(result);
}

TEST(Parser, Booleans) {
  kunjs::Parser parser;
  bool result = parser.parse("true; false;");
  ASSERT_TRUE(result);
}

TEST(Parser, Assignment) {
  kunjs::Parser parser;
  bool result = parser.parse("var n  = 10;");
  ASSERT_TRUE(result);
}

TEST(Parser, MultipleAssignment) {
  kunjs::Parser parser;
  bool result = parser.parse("var n=10, i=20, s='some text';");
  ASSERT_TRUE(result);
}

TEST(Parser, Ternary) {
  kunjs::Parser parser;
  bool result = parser.parse("var n = i == 10 ? 'yep' : 'no';");
  ASSERT_TRUE(result);
}

TEST(Parser, BitwiseOr) {
  kunjs::Parser parser;
  bool result = parser.parse("var n = 1234 | 1;");
  ASSERT_TRUE(result);
}

TEST(Parser, BitwiseXor) {
  kunjs::Parser parser;
  bool result = parser.parse("var n = 1234 ^ 143;");
  ASSERT_TRUE(result);
}

TEST(Parser, BitwiseAnd) {
  kunjs::Parser parser;
  bool result = parser.parse("var n = 1234 & 143;");
  ASSERT_TRUE(result);
}

TEST(Parser, ComplexLogicalExpression) {
  kunjs::Parser parser;
  bool result = parser.parse("var n = 1234 & 143 || truth() ^ 45 && 10 == 43;");
  ASSERT_TRUE(result);
}

TEST(Parser, DoWhile) {
  kunjs::Parser parser;
  std::string code =
      "do {"
      "  n += 10;"
      "} while (n <= 100);";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, While) {
  kunjs::Parser parser;
  std::string code =
      "while(n < 10) {"
      "  doSomething();"
      "}";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, EmptyForLoop) {
  kunjs::Parser parser;
  std::string code =
      "for(;;) {"
      "  noop();"
      "}";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, SimpleForLoop) {
  kunjs::Parser parser;
  std::string code =
      "for(var i=0; i < 100; i++) {"
      "  100 - i;"
      "}";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, FunctionCall) {
  kunjs::Parser parser;
  std::string code = "veryUseful(10, \"arg2\");";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, FunctionDefinition) {
  kunjs::Parser parser;
  std::string code =
      "function veryUseful(arg1, arg2) {"
      "  return arg1 * arg2;"
      "}";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, If) {
  kunjs::Parser parser;
  std::string code =
      "if(n < 100 || big) {"
      "  result = 'big';"
      "}";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, IfElse) {
  kunjs::Parser parser;
  std::string code =
      "if (t == 'some text' && valid) {"
      "  result -= 10;"
      "} else {"
      "  result += 20;"
      "}";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}


TEST(Parser, SimpleWith) {
  kunjs::Parser parser;
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
  kunjs::Parser parser;
  std::string code =
      "with (document) {"
      "  write('the text');"
      "  write('more text');"
      "}";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, SimpleLabel) {
  kunjs::Parser parser;
  std::string code = "theLabel: var n = x + 10;";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, LabelledFor) {
  kunjs::Parser parser;
  std::string code =
      "start: for(var i=0; i < 100; i++) {"
      "  break start;"
      "}";
  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, Try) {
  kunjs::Parser parser;
  std::string code =
      "try {"
      "  some = 'text';"
      "  doSomething();"
      "  someOne.action(true);"
      "}";
  bool result = parser.parse(code);
  ASSERT_FALSE(result);
}

TEST(Parser, TryCatch) {
  kunjs::Parser parser;
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
  kunjs::Parser parser;
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
  kunjs::Parser parser;
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

TEST(Parser, SwitchCaseDefault) {
  kunjs::Parser parser;
  // from http://www.w3schools.com/js/js_switch.asp
  std::string code =
      "var d = new Date();"
      "var theDay = d.getDay();"
      "switch (theDay) {"
      "case 5:"
      "  document.write('Finally Friday');"
      "  break;"
      "case 6:"
      "  document.write('Super Saturday');"
      "  break;"
      "case 0:"
      "  document.write('Sleepy Sunday');"
      "  break;"
      "default:"
      "  document.write('I am looking forward to this weekend!');"
      "}";

  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, SwitchDefaultCase) {
  kunjs::Parser parser;
  std::string code =
      "switch (theDay) {"
      "case 5:"
      "  document.write('Finally Friday');"
      "  break;"
      "default:"
      "  document.write('I am looking forward to this weekend!');"
      "case 6:"
      "  document.write('Super Saturday');"
      "  break;"
      "case 0:"
      "  document.write('Sleepy Sunday');"
      "  break;"
      "}";

  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, SwitchCase) {
  kunjs::Parser parser;
  std::string code =
      "switch (theDay) {"
      "case 5:"
      "  document.write('Finally Friday');"
      "  break;"
      "case 6:"
      "  document.write('Super Saturday');"
      "  break;"
      "case 0:"
      "  document.write('Sleepy Sunday');"
      "  break;"
      "}";

  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

TEST(Parser, SwitchDefault) {
  kunjs::Parser parser;
  std::string code =
      "switch (theDay) {"
      "default:"
      "  document.write('I am looking forward to this weekend!');"
      "}";

  bool result = parser.parse(code);
  ASSERT_TRUE(result);
}

