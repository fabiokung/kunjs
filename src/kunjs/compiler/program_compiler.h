#ifndef KUNJS_COMPILER_PROGRAMCOMPILER_H_
#define KUNJS_COMPILER_PROGRAMCOMPILER_H_

#if defined(_MSC_VER)
#pragma once
#endif

#include "kunjs/ast.h"
#include <boost/variant/static_visitor.hpp>
#include <string>

namespace kunjs { namespace compiler {

class ProgramCompiler : public boost::static_visitor<> {

 public:
  ProgramCompiler();
  void operator()(ast::Program const& node) const;
  void operator()(ast::SourceElement const& node) const;
  void operator()(ast::FunctionDeclaration const& node) const;
  void operator()(std::vector<ast::Statement> const& list) const;
  void operator()(ast::Statement const& node) const;

 private:
};

} // namespace compiler
} // namespace kunjs

#endif // KUNJS_COMPILER_PROGRAMCOMPILER_H_

