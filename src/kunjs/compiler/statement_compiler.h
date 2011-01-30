#ifndef KUNJS_COMPILER_STATEMENTCOMPILER_H_
#define KUNJS_COMPILER_STATEMENTCOMPILER_H_

#if defined(_MSC_VER)
#pragma once
#endif

#include "kunjs/ast.h"
#include <boost/variant/static_visitor.hpp>
#include <string>

namespace kunjs { namespace compiler {

class StatementCompiler : public boost::static_visitor<> {

 public:
  StatementCompiler();
  void operator()(ast::Expression const& expression);
  void operator()(ast::Var const& var);
  void operator()(ast::VarDeclaration const& var);
  void operator()(ast::Noop const& noop);
  void operator()(ast::If const& conditional);
  void operator()(ast::DoWhile const& loop);
  void operator()(ast::While const& loop);
  void operator()(ast::For const& loop);
  void operator()(ast::ForWithVar const& loop);
  void operator()(ast::Foreach const& loop);
  void operator()(ast::ForeachWithVar const& loop);
  void operator()(ast::Continue const& node);
  void operator()(ast::Break const& node);
  void operator()(ast::Return const& node);
  void operator()(ast::With const& with);
  void operator()(ast::LabelledStatement const& labelled);
  void operator()(ast::Switch const& conditional);
  void operator()(ast::Case const& clause);
  void operator()(ast::Throw const& node);
  void operator()(ast::Try const& node);
  void operator()(std::string const& debugger);
  void operator()(std::vector<ast::Statement> const& list);

 private:

};

} // namespace compiler
} // namespace kunjs

#endif // KUNJS_COMPILER_STATEMENTCOMPILER_H_

