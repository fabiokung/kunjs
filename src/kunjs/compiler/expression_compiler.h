#ifndef KUNJS_COMPILER_EXPRESSIONCOMPILER_H_
#define KUNJS_COMPILER_EXPRESSIONCOMPILER_H_

#if defined(_MSC_VER)
#pragma once
#endif

#include "kunjs/ast.h"
#include <boost/variant/static_visitor.hpp>
#include <string>

namespace kunjs { namespace compiler {

class ExpressionCompiler : public boost::static_visitor<> {

 public:
  ExpressionCompiler();
  void operator()(ast::AssignmentExpression const& expression);
  void operator()(ast::ConditionalExpression const& expression);
  void operator()(ast::LogicalOrExpression const& expression);
  void operator()(ast::LogicalAndExpression const& expression);
  void operator()(ast::BitwiseOrExpression const& expression);
  void operator()(ast::BitwiseXorExpression const& expression);
  void operator()(ast::BitwiseAndExpression const& expression);
  void operator()(ast::EqualityExpression const& expression);
  void operator()(ast::RelationalExpression const& expression);
  void operator()(ast::ShiftExpression const& expression);
  void operator()(ast::AdditiveExpression const& expression);
  void operator()(ast::MultiplicativeExpression const& expression);
  void operator()(ast::UnaryExpression const& expression);
  void operator()(ast::PostfixExpression const& expression);
  void operator()(ast::LhsExpression const& expression);
  void operator()(ast::CallExpression const& expression);
  void operator()(ast::NewExpression const& expression);
  void operator()(ast::MemberExpression const& expression);
  void operator()(ast::MemberAccess const& expression);
  void operator()(ast::MemberOptions const& expression);
  void operator()(ast::MemberModifier const& expression);
  void operator()(ast::Instantiation const& expression);
  void operator()(ast::PrimaryExpression const& expression);
  void operator()(ast::FunctionExpression const& expression);


 private:

};


class PrimaryExpressionCompiler : public boost::static_visitor<> {
 public:
  PrimaryExpressionCompiler();
  void operator()(ast::This const& node);
  void operator()(std::string const& identifier);
  void operator()(ast::Literal const& literal);
  void operator()(ast::Expression const& expression);

 private:

};

} // namespace compiler
} // namespace kunjs

#endif // KUNJS_COMPILER_EXPRESSIONCOMPILER_H_

