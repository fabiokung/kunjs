#include "kunjs/compiler.h"
#include <llvm/Constants.h>
#include <llvm/DerivedTypes.h>

#include <gtest/gtest.h>
#include <iostream>
#include <string>

namespace {
void DumpValue(llvm::Value* value) {
  std::cout << "--> value: " << std::flush;
  value->dump();
}
}

TEST(Compiler, Int) {
  kunjs::Compiler compiler;
  llvm::Value* result = compiler.compile("1;");
  DumpValue(result);

  ASSERT_TRUE(llvm::isa<llvm::ConstantInt>(result));
  llvm::ConstantInt* r = llvm::cast<llvm::ConstantInt>(result);
  ASSERT_TRUE(r->equalsInt(1));
}

TEST(Compiler, Float) {
  kunjs::Compiler compiler;
  llvm::Value* result = compiler.compile("3.14;");
  DumpValue(result);

  ASSERT_TRUE(llvm::isa<llvm::ConstantFP>(result));
  llvm::ConstantFP* r = llvm::cast<llvm::ConstantFP>(result);
  ASSERT_TRUE(r->isExactlyValue(3.14));
}

TEST(Compiler, Null) {
  kunjs::Compiler compiler;
  llvm::Value* result = compiler.compile("null;");
  DumpValue(result);

  ASSERT_TRUE(llvm::isa<llvm::ConstantPointerNull>(result));
  llvm::ConstantPointerNull* r = llvm::cast<llvm::ConstantPointerNull>(result);
  ASSERT_TRUE(r->isNullValue());
}

TEST(Compiler, True) {
  kunjs::Compiler compiler;
  llvm::Value* result = compiler.compile("true;");
  DumpValue(result);

  ASSERT_TRUE(llvm::isa<llvm::ConstantInt>(result));
  llvm::ConstantInt* r = llvm::cast<llvm::ConstantInt>(result);
  ASSERT_TRUE(r->isOne());
}

TEST(Compiler, False) {
  kunjs::Compiler compiler;
  llvm::Value* result = compiler.compile("false;");
  DumpValue(result);

  ASSERT_TRUE(llvm::isa<llvm::ConstantInt>(result));
  llvm::ConstantInt* r = llvm::cast<llvm::ConstantInt>(result);
  ASSERT_TRUE(r->isZero());
}

TEST(Compiler, SimpleIntArithmetic) {
  kunjs::Compiler compiler;
  llvm::Value* result = compiler.compile("1+2;");
  DumpValue(result);

  ASSERT_TRUE(llvm::isa<llvm::ConstantInt>(result));
  llvm::ConstantInt* r = llvm::cast<llvm::ConstantInt>(result);
  ASSERT_TRUE(llvm::APInt(64, 3, true) == r->getValue());
}

TEST(Compiler, ComplexIntArithmetic) {
  kunjs::Compiler compiler;
  llvm::Value* result = compiler.compile("1+2-3+7-12;");
  DumpValue(result);

  ASSERT_TRUE(llvm::isa<llvm::ConstantInt>(result));
  llvm::ConstantInt* r = llvm::cast<llvm::ConstantInt>(result);
  ASSERT_TRUE(llvm::APInt(64, -5, true) == r->getValue());
}

TEST(Compiler, SimpleFloatArithmetic) {
  kunjs::Compiler compiler;
  llvm::Value* result = compiler.compile("2.72 + 7.145;");
  DumpValue(result);

  ASSERT_TRUE(llvm::isa<llvm::ConstantFP>(result));
  llvm::ConstantFP* r = llvm::cast<llvm::ConstantFP>(result);
  ASSERT_TRUE(r->isExactlyValue(9.865));
}

TEST(Compiler, ComplexFloatArithmetic) {
  kunjs::Compiler compiler;
  llvm::Value* result = compiler.compile("4.123 -20 + 62.145 - 108.2;");
  DumpValue(result);

  ASSERT_TRUE(llvm::isa<llvm::ConstantFP>(result));
  llvm::ConstantFP* r = llvm::cast<llvm::ConstantFP>(result);
  ASSERT_TRUE(r->isExactlyValue(-61.932));
}

TEST(Compiler, IntMultiplication) {
  kunjs::Compiler compiler;
  llvm::Value* result = compiler.compile("5*9;");
  DumpValue(result);

  ASSERT_TRUE(llvm::isa<llvm::ConstantInt>(result));
  llvm::ConstantInt* r = llvm::cast<llvm::ConstantInt>(result);
  ASSERT_TRUE(llvm::APInt(64, 45, true) == r->getValue());
}

TEST(Compiler, ComplexIntMultiplication) {
  kunjs::Compiler compiler;
  llvm::Value* result = compiler.compile("1/2+2*(3+7) - 12;");
  DumpValue(result);

  ASSERT_TRUE(llvm::isa<llvm::ConstantInt>(result));
  llvm::ConstantInt* r = llvm::cast<llvm::ConstantInt>(result);
  ASSERT_TRUE(llvm::APInt(64, 8, true) == r->getValue());
}

TEST(Compiler, FloatMultiplication) {
  kunjs::Compiler compiler;
  llvm::Value* result = compiler.compile("5.0 / 2;");
  DumpValue(result);

  ASSERT_TRUE(llvm::isa<llvm::ConstantFP>(result));
  llvm::ConstantFP* r = llvm::cast<llvm::ConstantFP>(result);
  ASSERT_TRUE(r->isExactlyValue(2.5));
}

TEST(Compiler, ComplexFloatMultiplication) {
  kunjs::Compiler compiler;
  llvm::Value* result = compiler.compile("10 % 3 + 7 * 3 / 4.0;");
  DumpValue(result);

  ASSERT_TRUE(llvm::isa<llvm::ConstantFP>(result));
  llvm::ConstantFP* r = llvm::cast<llvm::ConstantFP>(result);
  ASSERT_TRUE(r->isExactlyValue(6.25));
}

