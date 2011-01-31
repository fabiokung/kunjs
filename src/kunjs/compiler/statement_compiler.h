#ifndef KUNJS_COMPILER_STATEMENTCOMPILER_H_
#define KUNJS_COMPILER_STATEMENTCOMPILER_H_

#if defined(_MSC_VER)
#pragma once
#endif

#include "kunjs/ast.h"
#include <boost/variant/static_visitor.hpp>

#include <llvm/Value.h>
#include <llvm/LLVMContext.h>

#include <string>

namespace kunjs { namespace compiler {

class StatementCompiler : public boost::static_visitor<llvm::Value*> {

 public:
  StatementCompiler(llvm::LLVMContext& context);
  llvm::Value* operator()(ast::Expression const& expression);
  llvm::Value* operator()(ast::Var const& var);
  llvm::Value* operator()(ast::VarDeclaration const& var);
  llvm::Value* operator()(ast::Noop const& noop);
  llvm::Value* operator()(ast::If const& conditional);
  llvm::Value* operator()(ast::DoWhile const& loop);
  llvm::Value* operator()(ast::While const& loop);
  llvm::Value* operator()(ast::For const& loop);
  llvm::Value* operator()(ast::ForWithVar const& loop);
  llvm::Value* operator()(ast::Foreach const& loop);
  llvm::Value* operator()(ast::ForeachWithVar const& loop);
  llvm::Value* operator()(ast::Continue const& node);
  llvm::Value* operator()(ast::Break const& node);
  llvm::Value* operator()(ast::Return const& node);
  llvm::Value* operator()(ast::With const& with);
  llvm::Value* operator()(ast::LabelledStatement const& labelled);
  llvm::Value* operator()(ast::Switch const& conditional);
  llvm::Value* operator()(ast::Case const& clause);
  llvm::Value* operator()(ast::Throw const& node);
  llvm::Value* operator()(ast::Try const& node);
  llvm::Value* operator()(std::string const& debugger);
  llvm::Value* operator()(std::vector<ast::Statement> const& list);

 private:
  llvm::LLVMContext& context;
};

} // namespace compiler
} // namespace kunjs

#endif // KUNJS_COMPILER_STATEMENTCOMPILER_H_

