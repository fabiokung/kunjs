#include "kunjs/grammar.h"

#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/qi_lit.hpp>
#include <boost/spirit/include/qi_lexeme.hpp>
#include <boost/spirit/include/qi_raw.hpp>
#include <boost/spirit/include/qi_eol.hpp>
#include <boost/spirit/include/qi_eps.hpp>
#include <boost/spirit/include/qi_int.hpp>
#include <boost/spirit/include/qi_real.hpp>
#include <boost/spirit/include/qi_bool.hpp>
#include <boost/spirit/include/qi_lazy.hpp>
#include <boost/spirit/include/qi_action.hpp>

namespace kunjs {

using qi::lit;
using qi::lexeme;
using qi::raw;
using qi::eol;
using qi::eps;
using qi::_val;
using qi::_1;
using qi::_2;
using qi::_3;
using qi::_4;
using qi::on_error;
using qi::fail;
using qi::int_;
using qi::double_;
using qi::bool_;
using boost::phoenix::function;
using boost::phoenix::construct;
using ascii::string;
using ascii::char_;
using ascii::alpha;
using ascii::alnum;

struct error_handler_ {
  template <typename, typename, typename>
  struct result { typedef void type; };

  template <typename Iterator>
  void operator()(qi::info const& what, Iterator err_pos, Iterator last) const {
    std::cout
        << "Error! Expecting "
        << what
        << " here: \""
        << std::string(err_pos, last)
        << "\""
        << std::endl;
  }
};
const function<error_handler_> error_handler = error_handler_();

template <typename Iterator>
javascript_grammar<Iterator>::javascript_grammar()
  : javascript_grammar::base_type(program, "program") {

  // Top level
  program %= *source_element;
  source_element %= function_declaration | statement;

  function_declaration %= lit("function") > identifier > '(' > -formal_parameter_list > ')' > '{' > function_body > '}';
  function_expression %= lit("function") > -identifier > '(' > -formal_parameter_list > ')' > '{' > function_body > '}';
  formal_parameter_list %= identifier % ",";
  function_body %= *source_element;

  statement %=
      !lit("function") >> expression[_val = _1] >> ";"
      | variable_statement
      | empty_statement
      | if_statement
      | do_while_statement
      | while_statement
      | for_statement
      | for_with_var_statement
      | foreach_statement
      | foreach_with_var_statement
      | continue_statement
      | break_statement
      | return_statement
      | with_statement
      | labelled_statement
      | switch_statement
      | throw_statement
      | try_statement
      | string("debugger") > ";"
      | lit("{") > *statement > lit("}");

  variable_statement %= lit("var") >> (variable_declaration % ",") >> ";";
  variable_declaration %= identifier >> -("=" >> assignment_expression);

  empty_statement %= lit(";")[_val = construct<ast::Noop>()];

  if_statement %= lit("if") >> "(" >> expression >> ")" >> statement >> -(lit("else") >> statement);

  do_while_statement %= lit("do") >> statement >> "while" >> "(" >> expression >> ")" > ";";
  while_statement %= lit("while") >> "(" >> expression >> ")" >> statement;
  for_statement %= lit("for") >> "(" >> -expression >> ";" >> -expression >> ";" >> -expression >> ")" >> statement;
  for_with_var_statement %= lit("for") >> "(" >> "var" >> (variable_declaration % ",") >> ";" >> -expression >> ";" >> -expression >> ")" >> statement;
  foreach_statement %= lit("for") >> "(" >> lhs_expression >> "in" >> expression >> ")" >> statement;
  foreach_with_var_statement %= lit("for") >> "(" >> "var" >> variable_declaration >> "in" >> expression >> ")" >> statement;

  continue_statement %= lit("continue") > !eol > -identifier > ";";
  break_statement %= lit("break") > !eol > -identifier > ";";
  return_statement %= lit("return") > !eol > -expression > ";";

  with_statement %= lit("with") > "(" > expression > ")" > statement;

  // TODO if there are no case clauses, default is not optional
  switch_statement %= lit("switch") > "(" > expression > ")"
      > "{" > *case_clause > -default_clause > *case_clause > "}";
  case_clause %= lit("case") > expression > ":" > *statement;
  default_clause %= lit("default") > ":" > *statement;

  labelled_statement %= identifier >> ":" > statement;

  throw_statement %= lit("throw") > !eol > expression[_val = construct<ast::Throw>(_1)] > ";";

  try_statement %= lit("try") >> "{" >> *statement >> "}" >> (catch_block || finally_block);
  catch_block %= lit("catch") > "(" > identifier > ")" > "{" >> *statement > "}";
  finally_block %= lit("finally") > "{" >> *statement > "}";

  expression %= assignment_expression % lit(',');

  assignment_expression %= *(lhs_expression >> assignment_operator) >> conditional_expression;
  assignment_operator %=
      !lit("==") >> string("=")
      | string("*=")
      | string("/=")
      | string("%=")
      | string("+=")
      | string("-=")
      | string("<<=")
      | string(">>=")
      | string(">>>=")
      | string("&=")
      | string("^=")
      | string("|=");

  conditional_clauses %= '?' > assignment_expression > ':' > assignment_expression;
  conditional_expression %= logical_or_expression
      >> -(conditional_clauses);

  logical_or_expression %= logical_and_expression >> *("||" >> logical_and_expression);

  logical_and_expression %= bitwise_or_expression >> *("&&" >> bitwise_or_expression);

  bitwise_or_expression %= bitwise_xor_expression >> *("|" >> bitwise_xor_expression);

  bitwise_xor_expression %= bitwise_and_expression >> *("^" >> bitwise_and_expression);

  bitwise_and_expression %= equality_expression >> *("&" >> equality_expression);

  equality_expression %= relational_expression >> *(equality_operator > relational_expression);
  equality_operator %= 
      string("===")
      | string("!==")
      | string("==")
      | string("!=");

  relational_expression %= shift_expression >> *(relational_operator > shift_expression);
  relational_operator %= 
      string("<=")
      | string(">=")
      | string("<")
      | string (">")
      | string("instanceof")
      | string("in");

  shift_expression %= additive_expression >> *(shift_operator > additive_expression);
  shift_operator %=
      string(">>>")
      | string("<<")
      | string(">>");

  additive_expression %= multiplicative_expression >> *(additive_operator > multiplicative_expression);
  additive_operator %= string("+") | string("-");

  multiplicative_expression %= unary_expression >> *(multiplicative_operator > unary_expression);
  multiplicative_operator %=
      string("*")
      | string("/")
      | string("%");

  unary_expression %= ((*unary_operator) >> postfix_expression);
  unary_operator %= string("delete")
      | string("void")
      | string("typeof")
      | string("++")
      | string("--")
      | string("+")
      | string("-")
      | string("~")
      | string("!");

  postfix_expression %= lhs_expression >> -(!eol >> postfix_operator);
  postfix_operator %= string("++") | string("--");

  lhs_expression %= call_expression | new_expression;

  call_expression %= member_expression >> arguments
      >> *(arguments | ('[' >> expression >> ']') | ('.' >> identifier_name));

  arguments %= lit('(') >> -(assignment_expression % ',') >> ')';

  new_expression %= *string("new") >> member_expression;

  member_access %= (primary_expression | function_expression)
      >> *(('[' >> expression >> ']') | ('.' >> identifier_name));

  instantiation %= member_expression >> arguments;
  member_expression %=
      member_access
      | lit("new") > instantiation;

  primary_expression %=
      this_reference
      | identifier
      | literal 
      | array_literal 
      //| object_literal 
      | '(' >> expression >> ')';

  array_literal = '[' >> *lit(',') >> assignment_expression
      >> *(+lit(',') >> assignment_expression) >> *lit(',') >> ']';

  this_reference %= string("this")[_val = construct<ast::This>()];

  // Lexical Grammar
  identifier %= !reserved_word >> identifier_name;

  // FIXME: unicode characters and connectors
  identifier_name %= identifier_start >> *alnum;

  identifier_start %= alpha | char_('$') | char_('_');

  reserved_word %=
      keyword
      | future_reserved_word
      | string("null")
      | string("true")
      | string("false");

  keyword %= keywords >> !alnum;
  keywords = "break", "do", "case", "else", "catch",  "finally", "continue",
           "for", "debugger", "function",  "default", "if", "delete", "in",
           "instanceof", "new",  "return", "switch", "this", "throw", "try",
           "typeof",  "var", "void", "while",  "with";

  future_reserved_word %= future_reserved_words >> !alnum;
  future_reserved_words = "class", "enum", "const", "export", "implements", "let",
                        "interface", "package", "yield", "extends", "import",
                        "private", "protected", "super", "public", "static";

  literal %=
      null_literal
      | bool_
      | numeric_literal
      | string_literal;

  null_literal %= string("null")[_val = construct<ast::Null>()];

  numeric_literal %= double_ | int_;

  string_literal %=
      '"' >> lexeme[*(~char_('"'))] >> '"'
      | '\'' >> lexeme[*(~char_('\''))] >> '\'';

  on_error<fail>(program, error_handler(_4, _3, _2));

  BOOST_SPIRIT_DEBUG_NODE(program);
  BOOST_SPIRIT_DEBUG_NODE(source_element);
  BOOST_SPIRIT_DEBUG_NODE(function_declaration);
  BOOST_SPIRIT_DEBUG_NODE(function_expression);
  BOOST_SPIRIT_DEBUG_NODE(formal_parameter_list);
  BOOST_SPIRIT_DEBUG_NODE(function_body);
  BOOST_SPIRIT_DEBUG_NODE(statement);
  BOOST_SPIRIT_DEBUG_NODE(variable_statement);
  BOOST_SPIRIT_DEBUG_NODE(variable_declaration);
  BOOST_SPIRIT_DEBUG_NODE(empty_statement);
  BOOST_SPIRIT_DEBUG_NODE(if_statement);
  BOOST_SPIRIT_DEBUG_NODE(do_while_statement);
  BOOST_SPIRIT_DEBUG_NODE(while_statement);
  BOOST_SPIRIT_DEBUG_NODE(for_statement);
  BOOST_SPIRIT_DEBUG_NODE(for_with_var_statement);
  BOOST_SPIRIT_DEBUG_NODE(foreach_statement);
  BOOST_SPIRIT_DEBUG_NODE(foreach_with_var_statement);
  BOOST_SPIRIT_DEBUG_NODE(continue_statement);
  BOOST_SPIRIT_DEBUG_NODE(break_statement);
  BOOST_SPIRIT_DEBUG_NODE(return_statement);
  BOOST_SPIRIT_DEBUG_NODE(with_statement);
  BOOST_SPIRIT_DEBUG_NODE(labelled_statement);
  BOOST_SPIRIT_DEBUG_NODE(switch_statement);
  BOOST_SPIRIT_DEBUG_NODE(case_clause);
  BOOST_SPIRIT_DEBUG_NODE(default_clause);
  BOOST_SPIRIT_DEBUG_NODE(throw_statement);
  BOOST_SPIRIT_DEBUG_NODE(try_statement);
  BOOST_SPIRIT_DEBUG_NODE(catch_block);
  BOOST_SPIRIT_DEBUG_NODE(finally_block);
  BOOST_SPIRIT_DEBUG_NODE(expression);
  BOOST_SPIRIT_DEBUG_NODE(assignment_expression);
  BOOST_SPIRIT_DEBUG_NODE(assignment_operator);
  BOOST_SPIRIT_DEBUG_NODE(conditional_clauses);
  BOOST_SPIRIT_DEBUG_NODE(conditional_expression);
  BOOST_SPIRIT_DEBUG_NODE(logical_or_expression);
  BOOST_SPIRIT_DEBUG_NODE(logical_and_expression);
  BOOST_SPIRIT_DEBUG_NODE(bitwise_or_expression);
  BOOST_SPIRIT_DEBUG_NODE(bitwise_xor_expression);
  BOOST_SPIRIT_DEBUG_NODE(bitwise_and_expression);
  BOOST_SPIRIT_DEBUG_NODE(equality_expression);
  BOOST_SPIRIT_DEBUG_NODE(equality_operator);
  BOOST_SPIRIT_DEBUG_NODE(relational_expression);
  BOOST_SPIRIT_DEBUG_NODE(relational_operator);
  BOOST_SPIRIT_DEBUG_NODE(shift_expression);
  BOOST_SPIRIT_DEBUG_NODE(shift_operator);
  BOOST_SPIRIT_DEBUG_NODE(additive_expression);
  BOOST_SPIRIT_DEBUG_NODE(additive_operator);
  BOOST_SPIRIT_DEBUG_NODE(multiplicative_expression);
  BOOST_SPIRIT_DEBUG_NODE(multiplicative_operator);
  BOOST_SPIRIT_DEBUG_NODE(unary_expression);
  BOOST_SPIRIT_DEBUG_NODE(unary_operator);
  BOOST_SPIRIT_DEBUG_NODE(postfix_expression);
  BOOST_SPIRIT_DEBUG_NODE(postfix_operator);
  BOOST_SPIRIT_DEBUG_NODE(lhs_expression);
  BOOST_SPIRIT_DEBUG_NODE(call_expression);
  BOOST_SPIRIT_DEBUG_NODE(arguments);
  BOOST_SPIRIT_DEBUG_NODE(new_expression);
  BOOST_SPIRIT_DEBUG_NODE(member_access);
  BOOST_SPIRIT_DEBUG_NODE(instantiation);
  BOOST_SPIRIT_DEBUG_NODE(member_expression);
  BOOST_SPIRIT_DEBUG_NODE(primary_expression);
  BOOST_SPIRIT_DEBUG_NODE(array_literal);
  BOOST_SPIRIT_DEBUG_NODE(this_reference);
  BOOST_SPIRIT_DEBUG_NODE(identifier);
  BOOST_SPIRIT_DEBUG_NODE(identifier_name);
  BOOST_SPIRIT_DEBUG_NODE(identifier_start);
  BOOST_SPIRIT_DEBUG_NODE(reserved_word);
  BOOST_SPIRIT_DEBUG_NODE(keyword);
  BOOST_SPIRIT_DEBUG_NODE(literal);
  BOOST_SPIRIT_DEBUG_NODE(null_literal);
  BOOST_SPIRIT_DEBUG_NODE(numeric_literal);
  BOOST_SPIRIT_DEBUG_NODE(string_literal);
}

template struct javascript_grammar<std::string::const_iterator>;

} // namespace kunjs

