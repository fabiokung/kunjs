#ifndef KUNJS_COMPILER_EXPRESSIONCOMPILER_H_
#define KUNJS_COMPILER_EXPRESSIONCOMPILER_H_

#if defined(_MSC_VER)
#pragma once
#endif

#include "kunjs/ast.h"
#include <boost/variant/static_visitor.hpp>

#include <llvm/Value.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/LLVMContext.h>

#include <string>

namespace kunjs { namespace compiler {

class ExpressionCompiler : public boost::static_visitor<llvm::Value*> {

 public:
  ExpressionCompiler(llvm::LLVMContext& context);
  llvm::Value* operator()(ast::AssignmentExpression const& expression);
  llvm::Value* operator()(ast::ConditionalExpression const& expression);
  llvm::Value* operator()(ast::LogicalOrExpression const& expression);
  llvm::Value* operator()(ast::LogicalAndExpression const& expression);
  llvm::Value* operator()(ast::BitwiseOrExpression const& expression);
  llvm::Value* operator()(ast::BitwiseXorExpression const& expression);
  llvm::Value* operator()(ast::BitwiseAndExpression const& expression);
  llvm::Value* operator()(ast::EqualityExpression const& expression);
  llvm::Value* operator()(ast::RelationalExpression const& expression);
  llvm::Value* operator()(ast::ShiftExpression const& expression);
  llvm::Value* operator()(ast::AdditiveExpression const& expression);
  llvm::Value* operator()(ast::MultiplicativeExpression const& expression);
  llvm::Value* operator()(ast::UnaryExpression const& expression);
  llvm::Value* operator()(ast::PostfixExpression const& expression);
  llvm::Value* operator()(ast::LhsExpression const& expression);
  llvm::Value* operator()(ast::CallExpression const& expression);
  llvm::Value* operator()(ast::NewExpression const& expression);
  llvm::Value* operator()(ast::MemberExpression const& expression);
  llvm::Value* operator()(ast::MemberAccess const& expression);
  llvm::Value* operator()(ast::MemberOptions const& expression);
  llvm::Value* operator()(ast::MemberModifier const& expression);
  llvm::Value* operator()(ast::Instantiation const& expression);
  llvm::Value* operator()(ast::PrimaryExpression const& expression);
  llvm::Value* operator()(ast::FunctionExpression const& expression);

 private:
  llvm::Value* CreateShlInstruction(llvm::Value* lhs, llvm::Value* rhs);
  llvm::Value* CreateAShrInstruction(llvm::Value* lhs, llvm::Value* rhs);
  llvm::Value* CreateLShrInstruction(llvm::Value* lhs, llvm::Value* rhs);
  llvm::Value* CreateAddInstruction(llvm::Value* lhs, llvm::Value* rhs);
  llvm::Value* CreateSubInstruction(llvm::Value* lhs, llvm::Value* rhs);
  llvm::Value* CreateMulInstruction(llvm::Value* lhs, llvm::Value* rhs);
  llvm::Value* CreateDivInstruction(llvm::Value* lhs, llvm::Value* rhs);
  llvm::Value* CreateRemInstruction(llvm::Value* lhs, llvm::Value* rhs);

  llvm::LLVMContext& context;
  llvm::IRBuilder<> builder;

};


class PrimaryExpressionCompiler : public boost::static_visitor<llvm::Value*> {
 public:
  PrimaryExpressionCompiler(llvm::LLVMContext& context);
  llvm::Value* operator()(ast::This const& node);
  llvm::Value* operator()(std::string const& identifier);
  llvm::Value* operator()(ast::Literal const& literal);
  llvm::Value* operator()(ast::Expression const& expression);

 private:
  llvm::LLVMContext& context;

};


class LiteralCompiler : public boost::static_visitor<llvm::Value*> {
 public:
  LiteralCompiler(llvm::LLVMContext& context);
  llvm::Value* operator()(ast::Null const& literal);
  llvm::Value* operator()(bool literal);
  llvm::Value* operator()(ast::Numeric const& numeric);
  llvm::Value* operator()(int literal);
  llvm::Value* operator()(double literal);
  llvm::Value* operator()(std::string const& literal);

 private:
  llvm::LLVMContext& context;

};

} // namespace compiler
} // namespace kunjs

#endif // KUNJS_COMPILER_EXPRESSIONCOMPILER_H_

