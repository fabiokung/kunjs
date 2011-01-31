#include "kunjs/compiler.h"
#include "kunjs/compiler/program_compiler.h"
#include "kunjs/parser.h"

#include <llvm/LLVMContext.h>
#include <llvm/Constants.h>

#include <string>

namespace kunjs {

llvm::Value* Compiler::compile(std::string code) {
  ast::Program ast;
  Parser parser;
  compiler::ProgramCompiler compile(llvm::getGlobalContext());

  parser.parse(code, ast);
  llvm::Value* result = compile(ast);
  result->dump();
  return result;
}

} // namespace kunjs

