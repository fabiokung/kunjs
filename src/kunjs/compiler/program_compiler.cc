#include "kunjs/compiler/program_compiler.h"
#include "kunjs/ast.h"

#include <boost/variant.hpp>
#include <boost/variant/apply_visitor.hpp>

#include <llvm/Constants.h>
#include <llvm/Type.h>
#include <llvm/LLVMContext.h>

#include <string>

namespace kunjs { namespace compiler {

ProgramCompiler::ProgramCompiler(llvm::LLVMContext& context)
    : context(context) {}

llvm::Value* ProgramCompiler::operator()(std::vector<ast::Statement> const& list) const {
  return std::for_each(list.begin(), list.end(), *this).result;
}

llvm::Value* ProgramCompiler::operator()(ast::Program const& program) const {
  return std::for_each(program.begin(), program.end(), *this).result;
}

llvm::Value* ProgramCompiler::operator()(ast::SourceElement const& element) const {
  result = boost::apply_visitor(*this, element);
  return result;
}

llvm::Value* ProgramCompiler::operator()(ast::FunctionDeclaration const& function) const {
  result = llvm::ConstantPointerNull::get(llvm::Type::getInt64PtrTy(context, 0U));
  return result;
}

llvm::Value* ProgramCompiler::operator()(ast::Statement const& statement) const {
  //StatementCompiler statement_compiler();
  //boost::apply_visitor(statement_compiler, statement);
  result = llvm::ConstantPointerNull::get(llvm::Type::getInt64PtrTy(context, 0U));
  return result;
}

} // namespace compiler
} // namespace kunjs

