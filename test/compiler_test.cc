#include "kunjs/compiler.h"
#include <llvm/Constants.h>
#include <llvm/Type.h>

#include <gtest/gtest.h>
#include <iostream>
#include <string>

TEST(Compiler, SimpleArithmetic) {
  kunjs::Compiler compiler;
  llvm::Value* result = compiler.compile("1+2;");
  ASSERT_TRUE(llvm::isa<llvm::ConstantInt>(result));
  llvm::ConstantInt* r = llvm::cast<llvm::ConstantInt>(result);
  ASSERT_TRUE(llvm::APInt(64, 4, false) == r->getValue());
}

