#ifndef KUNJS_COMPILER_PROGRAMCOMPILER_H_
#define KUNJS_COMPILER_PROGRAMCOMPILER_H_

#if defined(_MSC_VER)
#pragma once
#endif

#include "kunjs/ast.h"

#include <boost/variant/static_visitor.hpp>

#include <llvm/Value.h>
#include <llvm/LLVMContext.h>

#include <string>

namespace kunjs { namespace compiler {

class ProgramCompiler : public boost::static_visitor<llvm::Value*> {

 public:
  ProgramCompiler(llvm::LLVMContext& context);
  llvm::Value* operator()(ast::Program const& node) const;
  llvm::Value* operator()(ast::SourceElement const& node) const;
  llvm::Value* operator()(ast::FunctionDeclaration const& node) const;
  llvm::Value* operator()(std::vector<ast::Statement> const& list) const;
  llvm::Value* operator()(ast::Statement const& node) const;

 private:
  llvm::LLVMContext& context;
};

} // namespace compiler
} // namespace kunjs

#endif // KUNJS_COMPILER_PROGRAMCOMPILER_H_

