#ifndef KUNJS_COMPILER_H_
#define KUNJS_COMPILER_H_

#if defined(_MSC_VER)
#pragma once
#endif

#include <llvm/Value.h>
#include <string>

namespace kunjs {

class Compiler {
 public:
  llvm::Value* compile(std::string code);
};

} // namespace kunjs

#endif // KUNJS_COMPILER_H_

