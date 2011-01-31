#include "kunjs/compiler/statement_compiler.h"
#include "kunjs/compiler/expression_compiler.h"
#include "kunjs/compiler/program_compiler.h"
#include "kunjs/ast.h"

#include <boost/variant.hpp>
#include <boost/variant/apply_visitor.hpp>

#include <llvm/Constants.h>
#include <llvm/DerivedTypes.h>
#include <llvm/LLVMContext.h>

#include <string>

namespace kunjs { namespace compiler {

StatementCompiler::StatementCompiler(llvm::LLVMContext& context)
    : context(context) {}

llvm::Value* StatementCompiler::operator()(ast::Expression const& expression) {
  ExpressionCompiler compile(context);
  llvm::Value* result;
  for (ast::Expression::const_iterator it = expression.begin(); it != expression.end(); ++it)
    result = compile(*it);

  return result;
}

llvm::Value* StatementCompiler::operator()(ast::Var const& var) {
  llvm::Value* result;
  for (ast::Var::const_iterator it = var.begin(); it != var.end(); ++it)
    result = (*this)(*it);

  return result;
}

llvm::Value* StatementCompiler::operator()(ast::VarDeclaration const& declaration) {
  //ExpressionCompiler compiler(context);
  //declaration.name
  //if (declaration.assignment) compiler(declaration.assignment.get());

  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* StatementCompiler::operator()(ast::Noop const& noop) {
  // does nothing
  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* StatementCompiler::operator()(ast::If const& conditional) {
  //ProgramCompiler compiler(context);
  // if condition
  //(*this)(conditional.condition);

  // then
  //compiler(conditional.true_clause);

  // else
  //if (conditional.false_clause) {
  //compiler(conditional.false_clause.get());
  //}

  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* StatementCompiler::operator()(ast::DoWhile const& loop) {
  //ProgramCompiler compiler(context);

  // do
  //compiler(loop.statement);

  // while
  //(*this)(loop.condition);

  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* StatementCompiler::operator()(ast::While const& loop) {
  //ProgramCompiler compiler(context);

  // while
  //(*this)(loop.condition);

  // do
  //compiler(loop.statement);

  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* StatementCompiler::operator()(ast::For const& loop) {
  //ProgramCompiler compiler(context);

  //if (loop.initialization) {
  //(*this)(loop.initialization.get());
  //}

  //if (loop.condition) {
  //(*this)(loop.condition.get());
  //}

  //if (loop.action) {
  //(*this)(loop.action.get());
  //}

  // body
  //compiler(loop.statement);

  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* StatementCompiler::operator()(ast::ForWithVar const& loop) {
  //ProgramCompiler compiler(context);

  // initialization
  //for (ast::Var::const_iterator it = loop.initialization.begin(); it != loop.initialization.end(); ++it) {
  //(*this)(*it);
  //}

  //if (loop.condition) {
  //(*this)(loop.condition.get());
  //}

  //if (loop.action) {
  //(*this)(loop.action.get());
  //}

  // body
  //compiler(loop.statement);

  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* StatementCompiler::operator()(ast::Foreach const& loop) {
  //ExpressionCompiler expression_compiler(contex);
  //ProgramCompiler compiler(context);

  // each item
  //expression_compiler(loop.item);

  // in list
  //(*this)(loop.list);

  // body
  //compiler(loop.statement);

  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* StatementCompiler::operator()(ast::ForeachWithVar const& loop) {
  //ProgramCompiler compiler(context);

  // each item
  //(*this)(loop.item);

  // in list
  //(*this)(loop.list);

  // body
  //compiler(loop.statement);

  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* StatementCompiler::operator()(ast::Continue const& node) {
  //if (node.label) {
  //} else {
  //}
  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* StatementCompiler::operator()(ast::Break const& node) {
  //if (node.label) {
  //} else {
  //}
  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* StatementCompiler::operator()(ast::Return const& node) {
  //if (node.expression) {
  //(*this)(node.expression.get());
  //} else {
  //}
  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* StatementCompiler::operator()(ast::With const& with) {
  //ProgramCompiler compiler(context);
  //(*this)(with.context);
  //compiler(with.statement);

  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* StatementCompiler::operator()(ast::LabelledStatement const& labelled) {
  //ProgramCompiler compiler(context);
  //labelled.label
  //compiler(labelled.statement);

  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* StatementCompiler::operator()(ast::Case const& clause) {
  //ProgramCompiler compiler(context);

  // case
  //(*this)(clause.match_clause);

  // then
  //compiler(clause.statements);

  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* StatementCompiler::operator()(ast::Switch const& conditional) {
  //ProgramCompiler compiler(context);

  // condition
  //(*this)(conditional.condition);

  // clauses
  //for (std::vector<ast::Case>::const_iterator it = conditional.clauses.begin();
  //it != conditional.clauses.end(); ++it) {
  //(*this)(*it);
  //}

  // default
  //if (conditional.default_clause) {
  //compiler(conditional.default_clause.get());
  //}

  // more clauses after default
  //for (std::vector<ast::Case>::const_iterator it = conditional.other_clauses.begin();
  //it != conditional.other_clauses.end(); ++it) {
  //(*this)(*it);
  //}

  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* StatementCompiler::operator()(ast::Throw const& node) {
  //(*this)(node.expression);

  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* StatementCompiler::operator()(ast::Try const& node) {
  //ProgramCompiler compiler(context);

  // try
  //compiler(node.statements);

  //if (node.catch_block) {
  //ast::Catch block = node.catch_block.get();
  //block.exception_name
  //compiler(block.statements);
  //}

  //if (node.finally_block) {
  //compiler(node.finally_block.get());
  //}

  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* StatementCompiler::operator()(std::string const& debugger) {
  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* StatementCompiler::operator()(std::vector<ast::Statement> const& list) {
  ProgramCompiler compile(context);
  return compile(list);
}

} // namespace compiler
} // namespace kunjs

