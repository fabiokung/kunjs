#include "kungjs.h"
#include "parser.h"
#include "ast.h"

#include <boost/tr1/memory.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <string>

TEST(ArithmeticExpressions, SimpleAdd) {
  kungjs::Runner runner;
  std::string result = runner.run("1 + 2");
  ASSERT_EQ("3", result);
}

TEST(Parser, SimpleArithmetic) {
  kungjs::Parser parser;
  std::tr1::shared_ptr<kungjs::ast::Node> ast = parser.parse("1 + 2");
  std::cout << ast;
}

