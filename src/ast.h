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
  Null(std::string value) : value(value) {}
  std::string value;
};

typedef boost::variant<int, double> Numeric;
typedef boost::variant<ast::Null, bool, Numeric, std::string> Literal;

struct This {
  This() {}
  This(std::string value) : value(value) {}
  std::string value;
};

struct AssignmentExpression;
typedef std::vector<AssignmentExpression> ArrayLiteral;
typedef std::vector<AssignmentExpression> Expression;

typedef boost::variant<This, std::string, Literal, Expression> PrimaryExpression;

struct ConditionalClauses;
typedef boost::optional<boost::recursive_wrapper<ConditionalClauses> > ConditionalExpression;

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

struct AssignmentOperation {
  LhsExpression lhs;
  std::string operator_;
};
struct AssignmentExpression {
  std::vector<AssignmentOperation> assignments;
  ConditionalExpression rhs;
};
struct ConditionalClauses {
  AssignmentExpression true_clause;
  AssignmentExpression false_clause;
};

struct PostfixExpression {
  LhsExpression lhs;
  boost::optional<std::string> operator_;
};

struct UnaryExpression {
  std::vector<std::string> indecrements;
  PostfixExpression rhs;
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

typedef boost::make_recursive_variant<std::vector<boost::recursive_variant_>, Expression>::type Statement_;
typedef boost::optional<Statement_> Statement;

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
    kungjs::ast::ConditionalClauses,
    (kungjs::ast::AssignmentExpression, true_clause)
    (kungjs::ast::AssignmentExpression, false_clause))

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::AssignmentOperation,
    (kungjs::ast::LhsExpression, lhs)
    (std::string, operator_))

BOOST_FUSION_ADAPT_STRUCT(
    kungjs::ast::AssignmentExpression,
    (std::vector<kungjs::ast::AssignmentOperation>, assignments)
    (kungjs::ast::ConditionalExpression, rhs))


namespace std {

template <typename T>
std::ostream& operator<<(std::ostream& stream, const boost::recursive_wrapper<T>& wrapper) {
  stream << "recursive wrapper";
  return stream;
}

}
#endif // KUNGJS_AST_H_

