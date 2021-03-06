#ifndef KUNJS_AST_H_
#define KUNJS_AST_H_

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/optional.hpp>
#include <boost/spirit/home/support/attributes.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <vector>
#include <string>

namespace kunjs { namespace ast {

struct Null {};

typedef boost::variant<int, double> Numeric;
typedef boost::variant<ast::Null, bool, Numeric, std::string> Literal;

struct This {};

struct AssignmentExpression;
typedef std::vector<AssignmentExpression> Expression;
typedef std::vector<AssignmentExpression> ArrayLiteral;

typedef boost::variant<This, std::string, Literal, Expression> PrimaryExpression;

typedef std::vector<AssignmentExpression> Arguments;

struct FunctionExpression;

typedef boost::variant<
          PrimaryExpression,
          boost::recursive_wrapper<FunctionExpression>
        > MemberOptions;

typedef boost::variant<Expression, std::string> MemberModifier;

struct MemberAccess {
  MemberOptions member;
  std::vector<MemberModifier> modifiers;
};

struct Instantiation;
typedef boost::variant<
          MemberAccess,
          boost::recursive_wrapper<Instantiation>
        > MemberExpression;

struct Instantiation {
  MemberExpression member;
  Arguments arguments;
};

struct NewExpression {
  std::vector<std::string> operators;
  MemberExpression member;
};

typedef boost::variant<Arguments, Expression, std::string> CallModifiers;
struct CallExpression {
  MemberExpression target;
  Arguments arguments;
  std::vector<CallModifiers> modifiers;
};

typedef boost::variant<CallExpression, NewExpression> LhsExpression;

struct PostfixExpression {
  LhsExpression lhs;
  boost::optional<std::string> operator_;
};

struct UnaryExpression {
  std::vector<std::string> operators;
  PostfixExpression rhs;
};

struct MultiplicativeOperation {
  std::string operator_;
  UnaryExpression rhs;
};
struct MultiplicativeExpression {
  UnaryExpression lhs;
  std::vector<MultiplicativeOperation> operations;
};

struct AdditiveOperation {
  std::string operator_;
  MultiplicativeExpression rhs;
};
struct AdditiveExpression {
  MultiplicativeExpression lhs;
  std::vector<AdditiveOperation> operations;
};

struct ShiftOperation{
  std::string operator_;
  AdditiveExpression rhs;
};
struct ShiftExpression {
  AdditiveExpression lhs;
  std::vector<ShiftOperation> operations;
};

struct RelationalOperation {
  std::string operator_;
  ShiftExpression rhs;
};
struct RelationalExpression {
  ShiftExpression lhs;
  std::vector<RelationalOperation> operations;
};

struct EqualityOperation {
  std::string operator_;
  RelationalExpression rhs;
};
struct EqualityExpression {
  RelationalExpression lhs;
  std::vector<EqualityOperation> operations;
};

struct BitwiseAndExpression {
  EqualityExpression lhs;
  std::vector<EqualityExpression> operations;
};

struct BitwiseXorExpression {
  BitwiseAndExpression lhs;
  std::vector<BitwiseAndExpression> operations;
};

struct BitwiseOrExpression {
  BitwiseXorExpression lhs;
  std::vector<BitwiseXorExpression> operations;
};

struct LogicalAndExpression {
  BitwiseOrExpression lhs;
  std::vector<BitwiseOrExpression> operations;
};

struct LogicalOrExpression {
  LogicalAndExpression lhs;
  std::vector<LogicalAndExpression> operations;
};

struct ConditionalClauses {
  boost::recursive_wrapper<AssignmentExpression> true_clause;
  boost::recursive_wrapper<AssignmentExpression> false_clause;
};

struct ConditionalExpression {
  LogicalOrExpression lhs;
  boost::optional<ConditionalClauses> conditional_clauses;
};

struct AssignmentOperation {
  LhsExpression lhs;
  std::string operator_;
};

struct AssignmentExpression {
  std::vector<AssignmentOperation> assignments;
  ConditionalExpression rhs;
};

struct VarDeclaration {
  std::string name;
  boost::optional<AssignmentExpression> assignment;
};

typedef std::vector<VarDeclaration> Var;

struct Noop {};

struct If;

struct DoWhile;
struct While;
struct For;
struct ForWithVar;
struct Foreach;
struct ForeachWithVar;

struct Continue {
  boost::optional<std::string> label;
};

struct Break {
  boost::optional<std::string> label;
};

struct Return {
  boost::optional<Expression> expression;
};

struct With;

struct Case;
struct Switch;

struct LabelledStatement;

struct Throw {
  Throw() {}
  explicit Throw(Expression expression)
      : expression(expression) {}
  Expression expression;
};

struct Try;

typedef boost::make_recursive_variant<
          Expression,
          Var,
          Noop,
          boost::recursive_wrapper<If>,
          boost::recursive_wrapper<DoWhile>,
          boost::recursive_wrapper<While>,
          boost::recursive_wrapper<For>,
          boost::recursive_wrapper<ForWithVar>,
          boost::recursive_wrapper<Foreach>,
          boost::recursive_wrapper<ForeachWithVar>,
          Continue,
          Break,
          Return,
          boost::recursive_wrapper<With>,
          boost::recursive_wrapper<LabelledStatement>,
          boost::recursive_wrapper<Switch>,
          Throw,
          boost::recursive_wrapper<Try>,
          std::string,
          std::vector<boost::recursive_variant_>
        >::type Statement;

struct If {
  Expression condition;
  Statement true_clause;
  boost::optional<Statement> false_clause;
};

struct DoWhile {
  Statement statement;
  Expression condition;
};

struct While {
  Expression condition;
  Statement statement;
};

struct For {
  boost::optional<Expression> initialization;
  boost::optional<Expression> condition;
  boost::optional<Expression> action;
  Statement statement;
};

struct ForWithVar {
  Var initialization;
  boost::optional<Expression> condition;
  boost::optional<Expression> action;
  Statement statement;
};


struct Foreach {
  LhsExpression item;
  Expression list;
  Statement statement;
};

struct ForeachWithVar {
  VarDeclaration item;
  Expression list;
  Statement statement;
};

struct With {
  Expression context;
  Statement statement;
};

struct LabelledStatement {
  std::string label;
  Statement statement;
};

struct Case {
  Expression match_clause;
  std::vector<Statement> statements;
};

typedef std::vector<Statement> Default;

struct Switch {
  Expression condition;
  std::vector<Case> clauses;
  boost::optional<Default> default_clause;
  std::vector<Case> other_clauses;
};

typedef std::vector<Statement> Finally;

struct Catch {
  std::string exception_name;
  std::vector<Statement> statements;
};

struct Try {
  std::vector<Statement> statements;
  boost::optional<Catch> catch_block;
  boost::optional<Finally> finally_block;
};

struct FunctionDeclaration;

typedef boost::variant<
          boost::recursive_wrapper<FunctionDeclaration>,
          Statement
        > SourceElement;

typedef std::vector<SourceElement> FunctionBody;

struct FunctionDeclaration {
  std::string name;
  std::vector<std::string> parameters;
  FunctionBody body;
};

struct FunctionExpression {
  boost::optional<std::string> name;
  std::vector<std::string> parameters;
  FunctionBody body;
};

typedef std::vector<SourceElement> Program;

} // namespace ast
} // namespace kunjs

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::FunctionDeclaration,
    (std::string, name)
    (std::vector<std::string>, parameters)
    (kunjs::ast::FunctionBody, body)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::FunctionExpression,
    (boost::optional<std::string>, name)
    (std::vector<std::string>, parameters)
    (kunjs::ast::FunctionBody, body)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::MemberAccess,
    (kunjs::ast::MemberOptions, member)
    (std::vector<kunjs::ast::MemberModifier>, modifiers)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::Instantiation,
    (kunjs::ast::MemberExpression, member)
    (kunjs::ast::Arguments, arguments)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::NewExpression,
    (std::vector<std::string>, operators)
    (kunjs::ast::MemberExpression, member)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::CallExpression,
    (kunjs::ast::MemberExpression, target)
    (kunjs::ast::Arguments, arguments)
    (std::vector<kunjs::ast::CallModifiers>, modifiers)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::PostfixExpression,
    (kunjs::ast::LhsExpression, lhs)
    (boost::optional<std::string>, operator_)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::UnaryExpression,
    (std::vector<std::string>, operators)
    (kunjs::ast::PostfixExpression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::MultiplicativeExpression,
    (kunjs::ast::UnaryExpression, lhs)
    (std::vector<kunjs::ast::MultiplicativeOperation>, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::MultiplicativeOperation,
    (std::string, operator_)
    (kunjs::ast::UnaryExpression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::AdditiveExpression,
    (kunjs::ast::MultiplicativeExpression, lhs)
    (std::vector<kunjs::ast::AdditiveOperation>, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::AdditiveOperation,
    (std::string, operator_)
    (kunjs::ast::MultiplicativeExpression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::ShiftExpression,
    (kunjs::ast::AdditiveExpression, lhs)
    (std::vector<kunjs::ast::ShiftOperation>, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::ShiftOperation,
    (std::string, operator_)
    (kunjs::ast::AdditiveExpression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::RelationalExpression,
    (kunjs::ast::ShiftExpression, lhs)
    (std::vector<kunjs::ast::RelationalOperation>, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::RelationalOperation,
    (std::string, operator_)
    (kunjs::ast::ShiftExpression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::EqualityExpression,
    (kunjs::ast::RelationalExpression, lhs)
    (std::vector<kunjs::ast::EqualityOperation>, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::EqualityOperation,
    (std::string, operator_)
    (kunjs::ast::RelationalExpression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::BitwiseAndExpression,
    (kunjs::ast::EqualityExpression, lhs)
    (std::vector<kunjs::ast::EqualityExpression>, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::BitwiseXorExpression,
    (kunjs::ast::BitwiseAndExpression, lhs)
    (std::vector<kunjs::ast::BitwiseAndExpression>, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::BitwiseOrExpression,
    (kunjs::ast::BitwiseXorExpression, lhs)
    (std::vector<kunjs::ast::BitwiseXorExpression>, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::LogicalAndExpression,
    (kunjs::ast::BitwiseOrExpression, lhs)
    (std::vector<kunjs::ast::BitwiseOrExpression>, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::LogicalOrExpression,
    (kunjs::ast::LogicalAndExpression, lhs)
    (std::vector<kunjs::ast::LogicalAndExpression>, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::ConditionalClauses,
    (boost::recursive_wrapper<kunjs::ast::AssignmentExpression>, true_clause)
    (boost::recursive_wrapper<kunjs::ast::AssignmentExpression>, false_clause)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::ConditionalExpression,
    (kunjs::ast::LogicalOrExpression, lhs)
    (boost::optional<kunjs::ast::ConditionalClauses>, conditional_clauses)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::AssignmentOperation,
    (kunjs::ast::LhsExpression, lhs)
    (std::string, operator_)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::AssignmentExpression,
    (std::vector<kunjs::ast::AssignmentOperation>, assignments)
    (kunjs::ast::ConditionalExpression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::Catch,
    (std::string, exception_name)
    (std::vector<kunjs::ast::Statement>, statements)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::Try,
    (std::vector<kunjs::ast::Statement>, statements)
    (boost::optional<kunjs::ast::Catch>, catch_block)
    (boost::optional<kunjs::ast::Finally>, finally_block)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::Throw,
    (kunjs::ast::Expression, expression)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::With,
    (kunjs::ast::Expression, context)
    (kunjs::ast::Statement, statement)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::Case,
    (kunjs::ast::Expression, match_clause)
    (std::vector<kunjs::ast::Statement>, statements)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::Switch,
    (kunjs::ast::Expression, condition)
    (std::vector<kunjs::ast::Case>, clauses)
    (boost::optional<kunjs::ast::Default>, default_clause)
    (std::vector<kunjs::ast::Case>, other_clauses)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::LabelledStatement,
    (std::string, label)
    (kunjs::ast::Statement, statement)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::Return,
    (boost::optional<kunjs::ast::Expression>, expression)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::Break,
    (boost::optional<std::string>, label)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::Continue,
    (boost::optional<std::string>, label)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::DoWhile,
    (kunjs::ast::Statement, statement)
    (kunjs::ast::Expression, condition)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::While,
    (kunjs::ast::Expression, condition)
    (kunjs::ast::Statement, statement)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::For,
    (boost::optional<kunjs::ast::Expression>, initialization)
    (boost::optional<kunjs::ast::Expression>, condition)
    (boost::optional<kunjs::ast::Expression>, action)
    (kunjs::ast::Statement, statement)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::ForWithVar,
    (kunjs::ast::Var, initialization)
    (boost::optional<kunjs::ast::Expression>, condition)
    (boost::optional<kunjs::ast::Expression>, action)
    (kunjs::ast::Statement, statement)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::Foreach,
    (kunjs::ast::LhsExpression, item)
    (kunjs::ast::Expression, list)
    (kunjs::ast::Statement, statement)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::ForeachWithVar,
    (kunjs::ast::VarDeclaration, item)
    (kunjs::ast::Expression, list)
    (kunjs::ast::Statement, statement)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::If,
    (kunjs::ast::Expression, condition)
    (kunjs::ast::Statement, true_clause)
    (boost::optional<kunjs::ast::Statement>, false_clause)
)

BOOST_FUSION_ADAPT_STRUCT(
    kunjs::ast::VarDeclaration,
    (std::string, name)
    (boost::optional<kunjs::ast::AssignmentExpression>, assignment)
)

namespace boost { namespace spirit { namespace traits {

template <typename Out>
struct print_attribute_debug<Out, kunjs::ast::Null> {
  static void call(Out& out, kunjs::ast::Null const& val) {
      spirit::traits::print_attribute(out, "(Null)");
  }
};

template <typename Out>
struct print_attribute_debug<Out, kunjs::ast::This> {
  static void call(Out& out, kunjs::ast::This const& val) {
      spirit::traits::print_attribute(out, "(This)");
  }
};

template <typename Out>
struct print_attribute_debug<Out, kunjs::ast::Noop> {
  static void call(Out& out, kunjs::ast::Noop const& val) {
      spirit::traits::print_attribute(out, "(Noop)");
  }
};

template <typename Out, typename T>
struct print_attribute_debug<Out, boost::recursive_wrapper<T> > {
  static void call(Out& out, boost::recursive_wrapper<T> const& val) {
      spirit::traits::print_attribute(out, val.get());
  }
};

} } }

#endif // KUNJS_AST_H_

