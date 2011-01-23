#ifndef KUNJS_GRAMMAR_H_
#define KUNJS_GRAMMAR_H_

#if defined(_MSC_VER)
#pragma once
#endif

#define BOOST_SPIRIT_DEBUG

#include "ast.h"
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <boost/spirit/include/qi.hpp>
#include <vector>
#include <string>

namespace kunjs {

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

template <typename Iterator>
struct javascript_grammar : qi::grammar<Iterator, ast::Program(), ascii::space_type> {

  javascript_grammar();

  qi::rule<Iterator, ast::Program(), ascii::space_type> program;
  qi::rule<Iterator, ast::SourceElement(), ascii::space_type> source_element;
  qi::rule<Iterator, ast::FunctionDeclaration(), ascii::space_type> function_declaration;
  qi::rule<Iterator, std::vector<std::string>(), ascii::space_type> formal_parameter_list;
  qi::rule<Iterator, ast::FunctionBody(), ascii::space_type> function_body;

  qi::rule<Iterator, ast::Statement(), ascii::space_type> statement;
  qi::rule<Iterator, ast::Var(), ascii::space_type> variable_statement;
  qi::rule<Iterator, ast::VarDeclaration(), ascii::space_type> variable_declaration;
  qi::rule<Iterator, ast::Noop(), ascii::space_type> empty_statement;
  qi::rule<Iterator, ast::If(), ascii::space_type> if_statement;

  qi::rule<Iterator, ast::DoWhile(), ascii::space_type> do_while_statement;
  qi::rule<Iterator, ast::While(), ascii::space_type> while_statement;
  qi::rule<Iterator, ast::For(), ascii::space_type> for_statement;
  qi::rule<Iterator, ast::ForWithVar(), ascii::space_type> for_with_var_statement;
  qi::rule<Iterator, ast::Foreach(), ascii::space_type> foreach_statement;
  qi::rule<Iterator, ast::ForeachWithVar(), ascii::space_type> foreach_with_var_statement;

  qi::rule<Iterator, ast::Continue(), ascii::space_type> continue_statement;
  qi::rule<Iterator, ast::Break(), ascii::space_type> break_statement;
  qi::rule<Iterator, ast::Return(), ascii::space_type> return_statement;
  qi::rule<Iterator, ast::With(), ascii::space_type> with_statement;
  qi::rule<Iterator, ast::LabelledStatement(), ascii::space_type> labelled_statement;

  qi::rule<Iterator, ast::Switch(), ascii::space_type> switch_statement;
  qi::rule<Iterator, ast::Case(), ascii::space_type> case_clause;
  qi::rule<Iterator, ast::Default(), ascii::space_type> default_clause;

  qi::rule<Iterator, ast::Throw(), ascii::space_type> throw_statement;
  qi::rule<Iterator, ast::Try(), ascii::space_type> try_statement;
  qi::rule<Iterator, ast::Catch(), ascii::space_type> catch_block;
  qi::rule<Iterator, ast::Finally(), ascii::space_type> finally_block;

  qi::rule<Iterator, ast::Expression(), ascii::space_type> expression;

  qi::rule<Iterator, ast::AssignmentExpression(), ascii::space_type> assignment_expression;
  qi::rule<Iterator, std::string(), ascii::space_type> assignment_operator;

  qi::rule<Iterator, ast::ConditionalClauses(), ascii::space_type> conditional_clauses;
  qi::rule<Iterator, ast::ConditionalExpression(), ascii::space_type> conditional_expression;

  qi::rule<Iterator, ast::LogicalOrExpression(), ascii::space_type> logical_or_expression;
  qi::rule<Iterator, ast::LogicalAndExpression(), ascii::space_type> logical_and_expression;
  qi::rule<Iterator, ast::BitwiseOrExpression(), ascii::space_type> bitwise_or_expression;
  qi::rule<Iterator, ast::BitwiseXorExpression(), ascii::space_type> bitwise_xor_expression;
  qi::rule<Iterator, ast::BitwiseAndExpression(), ascii::space_type> bitwise_and_expression;

  qi::rule<Iterator, ast::EqualityExpression(), ascii::space_type> equality_expression;
  qi::rule<Iterator, std::string(), ascii::space_type> equality_operator;

  qi::rule<Iterator, ast::RelationalExpression(), ascii::space_type> relational_expression;
  qi::rule<Iterator, std::string(), ascii::space_type> relational_operator;

  qi::rule<Iterator, ast::ShiftExpression(), ascii::space_type> shift_expression;
  qi::rule<Iterator, std::string(), ascii::space_type> shift_operator;

  qi::rule<Iterator, ast::AdditiveExpression(), ascii::space_type> additive_expression;
  qi::rule<Iterator, std::string(), ascii::space_type> additive_operator;

  qi::rule<Iterator, ast::MultiplicativeExpression(), ascii::space_type> multiplicative_expression;
  qi::rule<Iterator, std::string(), ascii::space_type> multiplicative_operator;

  qi::rule<Iterator, ast::UnaryExpression(), ascii::space_type> unary_expression;
  qi::rule<Iterator, std::string(), ascii::space_type> unary_operator;

  qi::rule<Iterator, ast::PostfixExpression(), ascii::space_type> postfix_expression;
  qi::rule<Iterator, std::string(), ascii::space_type> postfix_operator;

  qi::rule<Iterator, ast::LhsExpression(), ascii::space_type> lhs_expression;

  qi::rule<Iterator, ast::CallExpression(), ascii::space_type> call_expression;
  qi::rule<Iterator, ast::Arguments(), ascii::space_type> arguments;

  qi::rule<Iterator, ast::NewExpression(), ascii::space_type> new_expression;

  qi::rule<Iterator, ast::FunctionExpression(), ascii::space_type> function_expression;
  qi::rule<Iterator, ast::MemberAccess(), ascii::space_type> member_access;
  qi::rule<Iterator, ast::Instantiation(), ascii::space_type> instantiation;
  qi::rule<Iterator, ast::MemberExpression(), ascii::space_type> member_expression;
  qi::rule<Iterator, ast::PrimaryExpression(), ascii::space_type> primary_expression;
  qi::rule<Iterator, ast::ArrayLiteral(), ascii::space_type> array_literal;
  qi::rule<Iterator, ast::This(), ascii::space_type> this_reference;

  qi::rule<Iterator, std::string(), ascii::space_type> identifier;
  qi::rule<Iterator, std::string(), ascii::space_type> identifier_name;
  qi::rule<Iterator, char(), ascii::space_type> identifier_start;
  qi::rule<Iterator, std::string(), ascii::space_type> reserved_word;
  qi::rule<Iterator, std::string()> keyword;
  qi::symbols<char> keywords;
  qi::rule<Iterator, std::string()> future_reserved_word;
  qi::symbols<char> future_reserved_words;

  qi::rule<Iterator, ast::Literal(), ascii::space_type> literal;
  qi::rule<Iterator, ast::Null(), ascii::space_type> null_literal;
  qi::rule<Iterator, ast::Numeric(), ascii::space_type> numeric_literal;
  qi::rule<Iterator, std::string(), ascii::space_type> string_literal;
};

} // namespace kunjs

#endif // KUNJS_GRAMMAR_H_

