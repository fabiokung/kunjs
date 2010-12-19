#ifndef KUNGJS_AST_H_
#define KUNGJS_AST_H_

#include <boost/config/warning_disable.hpp>
#include <boost/tr1/memory.hpp>
#include <boost/optional.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <vector>
#include <string>

namespace kungjs { namespace ast {

struct Null {
  Null() {}
  Null(std::string value) : value(value) {}
  std::string value;
};

typedef boost::variant<int, double> Numeric;
typedef boost::variant<ast::Null, bool, double, std::string> Literal;
struct Identifier {
  Identifier() {}
  Identifier(std::string value) : name(value) {}
  std::string name;
};
struct This {
  This() {}
  This(std::string value) : value(value) {}
  std::string value;
};
struct ExpressionHolder;

typedef boost::variant<This, Identifier, Literal, boost::recursive_wrapper<ExpressionHolder> > PrimaryExpression;

typedef PrimaryExpression MemberExpression;

struct NewOperation;
typedef boost::variant<MemberExpression, boost::recursive_wrapper<NewOperation> > NewExpression;
struct NewOperation {
  NewExpression rhs;
};

typedef NewExpression LhsExpression;

struct PostfixOperation;
typedef boost::variant<LhsExpression, boost::recursive_wrapper<PostfixOperation> > PostfixExpression;
struct PostfixOperation {
  LhsExpression lhs;
  std::string operator_;
};

struct UnaryOperation;
typedef boost::variant<PostfixExpression, boost::recursive_wrapper<UnaryOperation> > UnaryExpression;
struct UnaryOperation {
  std::string operator_;
  UnaryExpression rhs;
};

struct MultiplicativeOperation;
typedef boost::variant<UnaryExpression, boost::recursive_wrapper<MultiplicativeOperation> > MultiplicativeExpression;
struct MultiplicativeOperation {
  MultiplicativeExpression lhs;
  std::string operator_;
  UnaryExpression rhs;
};

struct AdditiveOperation;
typedef boost::variant<MultiplicativeExpression, boost::recursive_wrapper<AdditiveOperation> > AdditiveExpression;
struct AdditiveOperation {
  AdditiveExpression lhs;
  std::string operator_;
  MultiplicativeExpression rhs;
};

struct ShiftOperation;
typedef boost::variant<AdditiveExpression, boost::recursive_wrapper<ShiftOperation> > ShiftExpression;
struct ShiftOperation {
  ShiftExpression lhs;
  std::string operator_;
  AdditiveExpression rhs;
};

struct RelationalOperation;
typedef boost::variant<ShiftExpression, boost::recursive_wrapper<RelationalOperation> > RelationalExpression;
struct RelationalOperation {
  RelationalExpression lhs;
  std::string operator_;
  ShiftExpression rhs;
};

struct EqualityOperation;
typedef boost::variant<RelationalExpression, boost::recursive_wrapper<EqualityOperation> > EqualityExpression;
struct EqualityOperation {
  EqualityExpression lhs;
  std::string operator_;
  RelationalExpression rhs;
};

struct BitwiseAndOperation;
typedef boost::variant<EqualityExpression, boost::recursive_wrapper<BitwiseAndOperation> > BitwiseAndExpression;
struct BitwiseAndOperation {
  BitwiseAndExpression lhs;
  EqualityExpression rhs;
};

struct BitwiseXorOperation;
typedef boost::variant<BitwiseAndExpression, boost::recursive_wrapper<BitwiseXorOperation> > BitwiseXorExpression;
struct BitwiseXorOperation {
  BitwiseXorExpression lhs;
  BitwiseAndExpression rhs;
};

struct BitwiseOrOperation;
typedef boost::variant<BitwiseXorExpression, boost::recursive_wrapper<BitwiseOrOperation> > BitwiseOrExpression;
struct BitwiseOrOperation {
  BitwiseOrExpression lhs;
  BitwiseXorExpression rhs;
};

struct AndOperation;
typedef boost::variant<BitwiseOrExpression, boost::recursive_wrapper<AndOperation> > LogicalAndExpression;
struct AndOperation {
  LogicalAndExpression lhs;
  BitwiseOrExpression rhs;
};

struct OrOperation;
typedef boost::variant<LogicalAndExpression, boost::recursive_wrapper<OrOperation> > LogicalOrExpression;
struct OrOperation {
  LogicalOrExpression lhs;
  LogicalAndExpression rhs;
};

typedef LogicalOrExpression ConditionalExpression;

struct AssignmentOperation;
typedef boost::variant<ConditionalExpression, boost::recursive_wrapper<AssignmentOperation> > AssignmentExpression;
struct AssignmentOperation {
  LhsExpression lhs;
  std::string operator_;
  AssignmentExpression rhs;
};

typedef std::vector<AssignmentExpression> Expression;

struct ExpressionHolder {
  ExpressionHolder();
  ExpressionHolder(Expression value)
      : value(value) {}
  const Expression value;
};

typedef boost::make_recursive_variant<std::vector<boost::recursive_variant_>, Expression>::type Statement_;
typedef boost::optional<Statement_> Statement;

//typedef boost::variant<FunctionDeclaration, Statement> SourceElement;
typedef Statement SourceElement;
typedef std::vector<SourceElement> Program;

} // namespace ast
} // namespace kungjs

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::This,
    (std::string, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::Null,
    (std::string, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::Identifier,
    (std::string, name)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::NewOperation,
    (kungjs::ast::NewExpression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::PostfixOperation,
    (kungjs::ast::LhsExpression, lhs)
    (std::string, operator_)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::UnaryOperation,
    (std::string, operator_)
    (kungjs::ast::UnaryExpression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::MultiplicativeOperation,
    (kungjs::ast::MultiplicativeExpression, lhs)
    (std::string, operator_)
    (kungjs::ast::UnaryExpression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::AdditiveOperation,
    (kungjs::ast::AdditiveExpression, lhs)
    (std::string, operator_)
    (kungjs::ast::MultiplicativeExpression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::ShiftOperation,
    (kungjs::ast::ShiftExpression, lhs)
    (std::string, operator_)
    (kungjs::ast::AdditiveExpression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::RelationalOperation,
    (kungjs::ast::RelationalExpression, lhs)
    (std::string, operator_)
    (kungjs::ast::ShiftExpression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::EqualityOperation,
    (kungjs::ast::EqualityExpression, lhs)
    (std::string, operator_)
    (kungjs::ast::RelationalExpression, rhs))

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::BitwiseAndOperation,
    (kungjs::ast::BitwiseAndExpression, lhs)
    (kungjs::ast::EqualityExpression, rhs))

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::BitwiseXorOperation,
    (kungjs::ast::BitwiseXorExpression, lhs)
    (kungjs::ast::BitwiseAndExpression, rhs))

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::BitwiseOrOperation,
    (kungjs::ast::BitwiseOrExpression, lhs)
    (kungjs::ast::BitwiseXorExpression, rhs))

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::AndOperation,
    (kungjs::ast::LogicalAndExpression, lhs)
    (kungjs::ast::BitwiseOrExpression, rhs))

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::OrOperation,
    (kungjs::ast::LogicalOrExpression, lhs)
    (kungjs::ast::LogicalAndExpression, rhs))

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::AssignmentOperation,
    (kungjs::ast::LhsExpression, lhs)
    (std::string, operator_)
    (kungjs::ast::AssignmentExpression, rhs))

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::ExpressionHolder,
    (kungjs::ast::Expression, value))

#endif // KUNGJS_AST_H_

