#include "kunjs/compiler/program_compiler.h"
#include "kunjs/compiler/statement_compiler.h"
#include "kunjs/ast.h"

#include <boost/variant.hpp>
#include <boost/variant/apply_visitor.hpp>

#include <llvm/Constants.h>
#include <llvm/DerivedTypes.h>
#include <llvm/LLVMContext.h>

#include <string>

namespace kunjs { namespace compiler {

ProgramCompiler::ProgramCompiler(llvm::LLVMContext& context)
    : context(context) {}

llvm::Value* ProgramCompiler::operator()(std::vector<ast::Statement> const& list) const {
  llvm::Value* result;
  for (std::vector<ast::Statement>::const_iterator it = list.begin(); it != list.end(); ++it)
    result = (*this)(*it);

  return result;
}

llvm::Value* ProgramCompiler::operator()(ast::Program const& program) const {
  llvm::Value* result;
  for (ast::Program::const_iterator it = program.begin(); it != program.end(); ++it)
    result = (*this)(*it);

  return result;
}

llvm::Value* ProgramCompiler::operator()(ast::SourceElement const& element) const {
  return boost::apply_visitor(*this, element);
}

llvm::Value* ProgramCompiler::operator()(ast::FunctionDeclaration const& function) const {
  return llvm::ConstantPointerNull::get(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
}

llvm::Value* ProgramCompiler::operator()(ast::Statement const& statement) const {
  StatementCompiler statement_compiler(context);
  return boost::apply_visitor(statement_compiler, statement);
}

} // namespace compiler
} // namespace kunjs

