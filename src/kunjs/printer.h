#ifndef KUNJS_PRINTER_H_
#define KUNJS_PRINTER_H_

#if defined(_MSC_VER)
#pragma once
#endif

#include "ast.h"
#include <boost/variant/static_visitor.hpp>
#include <string>

namespace kunjs {

class ASTPrinter : public boost::static_visitor<> {

 public:
  ASTPrinter(int indent);
  void operator()(ast::Program const& node) const;
  void operator()(ast::SourceElement const& node) const;
  void operator()(ast::FunctionDeclaration const& node) const;
  void operator()(std::vector<ast::Statement> const& list) const;
  void operator()(ast::Statement const& node) const;

 private:
  mutable int indentation;
};

class StatementPrinter : public boost::static_visitor<> {

 public:
  StatementPrinter(int indent);
  void operator()(ast::Expression const& expression);
  void operator()(ast::Var const& var);
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
  void operator()(ast::LabelledStatement const& labeled);
  void operator()(ast::Switch const& conditional);
  void operator()(ast::Throw const& node);
  void operator()(ast::Try const& node);
  void operator()(std::string const& debugger);
  void operator()(std::vector<ast::Statement> const& list);

 private:
  mutable int indentation;
};


class ExpressionPrinter : public boost::static_visitor<> {
 public:
  ExpressionPrinter(int indent);
  void operator()(ast::AssignmentExpression const& expression);


 private:
  mutable int indentation;
};

} // namespace kunjs

#endif // KUNJS_PRINTER_H_

