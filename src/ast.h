#ifndef KUNGJS_AST_H_
#define KUNGJS_AST_H_

#include <boost/optional.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <vector>
#include <string>

namespace kungjs { namespace ast {

struct FunctionExpression {
  boost::optional<std::string> name;
  std::vector<std::string> parameters;
  // TODO body
};

struct Null {
  Null() {}
  explicit Null(std::string value) : value(value) {}
  std::string value;
};

typedef boost::variant<int, double> Numeric;
typedef boost::variant<ast::Null, bool, Numeric, std::string> Literal;

struct This {
  This() {}
  explicit This(std::string value) : value(value) {}
  std::string value;
};

struct AssignmentExpression;
typedef std::vector<AssignmentExpression> Expression;
typedef std::vector<AssignmentExpression> ArrayLiteral;

typedef boost::variant<This, std::string, Literal, Expression> PrimaryExpression;

typedef boost::variant<PrimaryExpression, FunctionExpression> MemberOptions;
typedef boost::variant<Expression, std::string> MemberModifiers;
struct MemberExpression {
  MemberOptions member;
  std::vector<MemberModifiers> modifiers;
};

struct NewExpression {
  std::vector<std::string> operators;
  MemberExpression member;
};

typedef std::vector<AssignmentExpression> Arguments;

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
  std::vector<std::string> indecrements;
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

struct If;

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
          boost::recursive_wrapper<If>,
          Continue,
          Break,
          Return,
          boost::recursive_wrapper<With>,
          boost::recursive_wrapper<LabelledStatement>,
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

struct With {
  Expression expression;
  Statement statement;
};

struct LabelledStatement {
  std::string label;
  Statement statement;
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

//typedef boost::variant<FunctionDeclaration, Statement> SourceElement;
typedef Statement SourceElement;
typedef std::vector<SourceElement> Program;

} // namespace ast
} // namespace kungjs

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::FunctionExpression,
    (boost::optional<std::string>, name)
    (std::vector<std::string>, parameters)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::This,
    (std::string, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::Null,
    (std::string, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::MemberExpression,
    (kungjs::ast::MemberOptions, member)
    (std::vector<kungjs::ast::MemberModifiers>, modifiers)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::NewExpression,
    (std::vector<std::string>, operators)
    (kungjs::ast::MemberExpression, member)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::CallExpression,
    (kungjs::ast::MemberExpression, target)
    (kungjs::ast::Arguments, arguments)
    (std::vector<kungjs::ast::CallModifiers>, modifiers)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::PostfixExpression,
    (kungjs::ast::LhsExpression, lhs)
    (boost::optional<std::string>, operator_)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::UnaryExpression,
    (std::vector<std::string>, indecrements)
    (kungjs::ast::PostfixExpression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::MultiplicativeExpression,
    (kungjs::ast::UnaryExpression, lhs)
    (std::vector<kungjs::ast::MultiplicativeOperation>, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::MultiplicativeOperation,
    (std::string, operator_)
    (kungjs::ast::UnaryExpression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::AdditiveExpression,
    (kungjs::ast::MultiplicativeExpression, lhs)
    (std::vector<kungjs::ast::AdditiveOperation>, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::AdditiveOperation,
    (std::string, operator_)
    (kungjs::ast::MultiplicativeExpression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::ShiftExpression,
    (kungjs::ast::AdditiveExpression, lhs)
    (std::vector<kungjs::ast::ShiftOperation>, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::ShiftOperation,
    (std::string, operator_)
    (kungjs::ast::AdditiveExpression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::RelationalExpression,
    (kungjs::ast::ShiftExpression, lhs)
    (std::vector<kungjs::ast::RelationalOperation>, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::RelationalOperation,
    (std::string, operator_)
    (kungjs::ast::ShiftExpression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::EqualityExpression,
    (kungjs::ast::RelationalExpression, lhs)
    (std::vector<kungjs::ast::EqualityOperation>, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::EqualityOperation,
    (std::string, operator_)
    (kungjs::ast::RelationalExpression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::BitwiseAndExpression,
    (kungjs::ast::EqualityExpression, lhs)
    (std::vector<kungjs::ast::EqualityExpression>, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::BitwiseXorExpression,
    (kungjs::ast::BitwiseAndExpression, lhs)
    (std::vector<kungjs::ast::BitwiseAndExpression>, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::BitwiseOrExpression,
    (kungjs::ast::BitwiseXorExpression, lhs)
    (std::vector<kungjs::ast::BitwiseXorExpression>, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::LogicalAndExpression,
    (kungjs::ast::BitwiseOrExpression, lhs)
    (std::vector<kungjs::ast::BitwiseOrExpression>, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::LogicalOrExpression,
    (kungjs::ast::LogicalAndExpression, lhs)
    (std::vector<kungjs::ast::LogicalAndExpression>, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::ConditionalClauses,
    (boost::recursive_wrapper<kungjs::ast::AssignmentExpression>, true_clause)
    (boost::recursive_wrapper<kungjs::ast::AssignmentExpression>, false_clause)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::ConditionalExpression,
    (kungjs::ast::LogicalOrExpression, lhs)
    (boost::optional<kungjs::ast::ConditionalClauses>, conditional_clauses)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::AssignmentOperation,
    (kungjs::ast::LhsExpression, lhs)
    (std::string, operator_)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::AssignmentExpression,
    (std::vector<kungjs::ast::AssignmentOperation>, assignments)
    (kungjs::ast::ConditionalExpression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::Catch,
    (std::string, exception_name)
    (std::vector<kungjs::ast::Statement>, statements)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::Try,
    (std::vector<kungjs::ast::Statement>, statements)
    (boost::optional<kungjs::ast::Catch>, catch_block)
    (boost::optional<kungjs::ast::Finally>, finally_block)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::Throw,
    (kungjs::ast::Expression, expression)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::With,
    (kungjs::ast::Expression, expression)
    (kungjs::ast::Statement, statement)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::LabelledStatement,
    (std::string, label)
    (kungjs::ast::Statement, statement)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::Return,
    (boost::optional<kungjs::ast::Expression>, expression)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::Break,
    (boost::optional<std::string>, label)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::Continue,
    (boost::optional<std::string>, label)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::If,
    (kungjs::ast::Expression, condition)
    (kungjs::ast::Statement, true_clause)
    (boost::optional<kungjs::ast::Statement>, false_clause)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::VarDeclaration,
    (std::string, name)
    (boost::optional<kungjs::ast::AssignmentExpression>, assignment)
)


namespace std {

template <typename T>
std::ostream& operator<<(std::ostream& stream, const boost::recursive_wrapper<T>& wrapper) {
  stream << "recursive wrapper" << "\n";
  return stream;
}

}
#endif // KUNGJS_AST_H_

