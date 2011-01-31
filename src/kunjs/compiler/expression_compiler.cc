#include "kunjs/compiler/expression_compiler.h"
#include "kunjs/compiler/statement_compiler.h"
#include "kunjs/ast.h"

#include <boost/variant.hpp>
#include <boost/variant/apply_visitor.hpp>

#include <llvm/Support/IRBuilder.h>
#include <llvm/Constants.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Metadata.h>
#include <llvm/LLVMContext.h>

#include <iostream>
#include <string>

namespace kunjs { namespace compiler {

ExpressionCompiler::ExpressionCompiler(llvm::LLVMContext& ctx) :
    context(ctx), builder(llvm::IRBuilder<>(ctx)) {}

llvm::Value* ExpressionCompiler::operator()(ast::AssignmentExpression const& expression) {
  //for (std::vector<ast::AssignmentOperation>::const_iterator it = expression.assignments.begin();
  //it != expression.assignments.end(); ++it) {
  //(*this)(it->lhs);
  //it->operator_;
  //}

  return (*this)(expression.rhs);
}

llvm::Value* ExpressionCompiler::operator()(ast::ConditionalExpression const& expression) {
  return (*this)(expression.lhs);
  //if (expression.conditional_clauses) {
  //ast::ConditionalClauses ternary = expression.conditional_clauses.get();
  // true
  //(*this)(ternary.true_clause.get());
  // false
  //(*this)(ternary.false_clause.get());
  //}
}

llvm::Value* ExpressionCompiler::operator()(ast::LogicalOrExpression const& expression) {
  return (*this)(expression.lhs);
  //std::for_each(expression.operations.begin(), expression.operations.end(), *this);
}

llvm::Value* ExpressionCompiler::operator()(ast::LogicalAndExpression const& expression) {
  return (*this)(expression.lhs);
  //std::for_each(expression.operations.begin(), expression.operations.end(), *this);
}

llvm::Value* ExpressionCompiler::operator()(ast::BitwiseOrExpression const& expression) {
  return (*this)(expression.lhs);
  //std::for_each(expression.operations.begin(), expression.operations.end(), *this);
}

llvm::Value* ExpressionCompiler::operator()(ast::BitwiseXorExpression const& expression) {
  return (*this)(expression.lhs);
  //std::for_each(expression.operations.begin(), expression.operations.end(), *this);
}

llvm::Value* ExpressionCompiler::operator()(ast::BitwiseAndExpression const& expression) {
  return (*this)(expression.lhs);
  //std::for_each(expression.operations.begin(), expression.operations.end(), *this);
}

llvm::Value* ExpressionCompiler::operator()(ast::EqualityExpression const& expression) {
  return (*this)(expression.lhs);
  //for (std::vector<ast::EqualityOperation>::const_iterator it = expression.operations.begin();
  //it != expression.operations.end(); ++it) {
  //it->operator_;
  //(*this)(it->rhs);
  //}
}

llvm::Value* ExpressionCompiler::operator()(ast::RelationalExpression const& expression) {
  return (*this)(expression.lhs);

  //for (std::vector<ast::RelationalOperation>::const_iterator it = expression.operations.begin();
  //it != expression.operations.end(); ++it) {
  //it->operator_;
  //(*this)(it->rhs);
  //}
}

llvm::Value* ExpressionCompiler::operator()(ast::ShiftExpression const& expression) {
  return (*this)(expression.lhs);

  //for (std::vector<ast::ShiftOperation>::const_iterator it = expression.operations.begin();
  //it != expression.operations.end(); ++it) {
  //it->operator_;
  //(*this)(it->rhs);
  //}
}

llvm::Value* ExpressionCompiler::CreateAddInstruction(llvm::Value* lhs, llvm::Value* rhs) {
  if (lhs->getType()->isDoubleTy() || rhs->getType()->isDoubleTy()) {
    return builder.CreateFAdd(builder.CreateSIToFP(lhs, llvm::Type::getDoubleTy(context)),
                              builder.CreateSIToFP(rhs, llvm::Type::getDoubleTy(context)),
                              "add_double");
  } else {
    return builder.CreateAdd(lhs, rhs, "add_int");
  }
}

llvm::Value* ExpressionCompiler::CreateSubInstruction(llvm::Value* lhs, llvm::Value* rhs) {
  if (lhs->getType()->isDoubleTy() || rhs->getType()->isDoubleTy()) {
    return builder.CreateFSub(builder.CreateSIToFP(lhs, llvm::Type::getDoubleTy(context)),
                              builder.CreateSIToFP(rhs, llvm::Type::getDoubleTy(context)),
                              "sub_double");
  } else {
    return builder.CreateSub(lhs, rhs, "sub_int");
  }
}

llvm::Value* ExpressionCompiler::operator()(ast::AdditiveExpression const& expression) {
  llvm::Value* result = (*this)(expression.lhs);

  for (std::vector<ast::AdditiveOperation>::const_iterator it = expression.operations.begin();
       it != expression.operations.end(); ++it) {
    llvm::Value* rhs = (*this)(it->rhs);
    if (it->operator_ == "+") {
      result = CreateAddInstruction(result, rhs);
    }
    else if (it->operator_ == "-") {
      result = CreateSubInstruction(result, rhs);
    } // TODO: else throw error
  }

  return result;
}

llvm::Value* ExpressionCompiler::CreateMulInstruction(llvm::Value* lhs, llvm::Value* rhs) {
  if (lhs->getType()->isDoubleTy() || rhs->getType()->isDoubleTy()) {
    return builder.CreateFMul(builder.CreateSIToFP(lhs, llvm::Type::getDoubleTy(context)),
                              builder.CreateSIToFP(rhs, llvm::Type::getDoubleTy(context)),
                              "mul_double");
  } else {
    return builder.CreateMul(lhs, rhs, "mul_int");
  }
}

llvm::Value* ExpressionCompiler::CreateDivInstruction(llvm::Value* lhs, llvm::Value* rhs) {
  if (lhs->getType()->isDoubleTy() || rhs->getType()->isDoubleTy()) {
    return builder.CreateFDiv(builder.CreateSIToFP(lhs, llvm::Type::getDoubleTy(context)),
                              builder.CreateSIToFP(rhs, llvm::Type::getDoubleTy(context)),
                              "div_double");
  } else {
    return builder.CreateSDiv(lhs, rhs, "div_int");
  }
}

llvm::Value* ExpressionCompiler::CreateRemInstruction(llvm::Value* lhs, llvm::Value* rhs) {
  if (lhs->getType()->isDoubleTy() || rhs->getType()->isDoubleTy()) {
    return builder.CreateFRem(builder.CreateSIToFP(lhs, llvm::Type::getDoubleTy(context)),
                              builder.CreateSIToFP(rhs, llvm::Type::getDoubleTy(context)),
                              "rem_double");
  } else {
    return builder.CreateSRem(lhs, rhs, "rem_int");
  }
}

llvm::Value* ExpressionCompiler::operator()(ast::MultiplicativeExpression const& expression) {
  llvm::Value* result = (*this)(expression.lhs);

  for (std::vector<ast::MultiplicativeOperation>::const_iterator it = expression.operations.begin();
       it != expression.operations.end(); ++it) {
    llvm::Value* rhs = (*this)(it->rhs);
    if (it->operator_ == "*") {
      result = CreateMulInstruction(result, rhs);
    } else if (it->operator_ == "/") {
      result = CreateDivInstruction(result, rhs);
    } else if (it->operator_ == "%") {
      result = CreateRemInstruction(result, rhs);
    } // TODO: else throw error
  }

  return result;
}

llvm::Value* ExpressionCompiler::operator()(ast::UnaryExpression const& expression) {
  //for (std::vector<std::string>::const_iterator it = expression.operators.begin();
  //it != expression.operators.end(); ++it) {
  ////unary operation
  //*it;
  //}

  return (*this)(expression.rhs);
}

llvm::Value* ExpressionCompiler::operator()(ast::PostfixExpression const& expression) {
  return (*this)(expression.lhs);

  //if (expression.operator_) {
  //expression.operator_.get();
  //}
}

llvm::Value* ExpressionCompiler::operator()(ast::LhsExpression const& expression) {
  return boost::apply_visitor(*this, expression);
}

llvm::Value* ExpressionCompiler::operator()(ast::CallExpression const& expression) {
  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* ExpressionCompiler::operator()(ast::NewExpression const& expression) {
  //for (std::vector<std::string>::const_iterator it = expression.operators.begin();
  //it != expression.operators.end(); ++it) {
  //// new operations
  //*it;
  //}

  return (*this)(expression.member);
}

llvm::Value* ExpressionCompiler::operator()(ast::MemberExpression const& expression) {
  return boost::apply_visitor(*this, expression);
}

llvm::Value* ExpressionCompiler::operator()(ast::MemberAccess const& expression) {
  return (*this)(expression.member);

  //if (!expression.modifiers.empty()) {
  //std::for_each(expression.modifiers.begin(), expression.modifiers.end(), *this);
  //}
}

llvm::Value* ExpressionCompiler::operator()(ast::MemberOptions const& expression) {
  return boost::apply_visitor(*this, expression);
}

llvm::Value* ExpressionCompiler::operator()(ast::MemberModifier const& expression) {
  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* ExpressionCompiler::operator()(ast::Instantiation const& expression) {
  //StatementCompiler compile_statement(context);

  // instantiation
  //(*this)(expression.member);

  // arguments
  //compile_statement(expression.arguments);

  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* ExpressionCompiler::operator()(ast::PrimaryExpression const& expression) {
  PrimaryExpressionCompiler compiler = PrimaryExpressionCompiler(context);
  return boost::apply_visitor(compiler, expression);
}

llvm::Value* ExpressionCompiler::operator()(ast::FunctionExpression const& expression) {
  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}


PrimaryExpressionCompiler::PrimaryExpressionCompiler(llvm::LLVMContext& context)
  : context(context) {}

llvm::Value* PrimaryExpressionCompiler::operator()(ast::This const& node) {
  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* PrimaryExpressionCompiler::operator()(std::string const& identifier) {
  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* PrimaryExpressionCompiler::operator()(ast::Literal const& literal) {
  LiteralCompiler compiler = LiteralCompiler(context);
  return boost::apply_visitor(compiler, literal);
}

llvm::Value* PrimaryExpressionCompiler::operator()(ast::Expression const& expression) {
  StatementCompiler compile(context);
  return compile(expression);
}


LiteralCompiler::LiteralCompiler(llvm::LLVMContext& context)
  : context(context) {}

llvm::Value* LiteralCompiler::operator()(ast::Null const& literal) {
  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* LiteralCompiler::operator()(bool literal) {
  return literal ? llvm::ConstantInt::getTrue(context) : llvm::ConstantInt::getFalse(context);
}

llvm::Value* LiteralCompiler::operator()(ast::Numeric const& numeric) {
  return boost::apply_visitor(*this, numeric);
}

llvm::Value* LiteralCompiler::operator()(int literal) {
  return llvm::ConstantInt::get(context, llvm::APInt(64, literal, true));
}

llvm::Value* LiteralCompiler::operator()(double literal) {
  return llvm::ConstantFP::get(context, llvm::APFloat(literal));
}

llvm::Value* LiteralCompiler::operator()(std::string const& literal) {
  return llvm::MDString::get(context, literal);
}


} // namespace compiler
} // namespace kunjs

