#ifndef KUNGJS_GRAMMAR_H_
#define KUNGJS_GRAMMAR_H_

#define BOOST_SPIRIT_DEBUG
#include "ast.h"
#include <boost/tr1/memory.hpp>
#include <boost/spirit/home/phoenix.hpp>
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <vector>
#include <string>

namespace kungjs {

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

using boost::phoenix::function;

struct error_handler_
{
    template <typename, typename, typename>
    struct result { typedef void type; };

    template <typename Iterator>
    void operator()(
        qi::info const& what
      , Iterator err_pos, Iterator last) const
    {
        std::cout
            << "Error! Expecting "
            << what                         // what failed?
            << " here: \""
            << std::string(err_pos, last)   // iterators to error-pos, end
            << "\""
            << std::endl
        ;
    }
};

const function<error_handler_> error_handler = error_handler_();

template <typename Iterator>
struct javascript_grammar : qi::grammar<Iterator, ascii::space_type> {
  javascript_grammar() : javascript_grammar::base_type(program, "program") {

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
    using boost::phoenix::construct;
    using ascii::string;
    using ascii::char_;
    using ascii::alpha;
    using ascii::alnum;

    // Top level
    program %= *source_element;
    source_element %= statement | function_declaration;

    function_declaration %= lit("function") > identifier > '(' > -formal_parameter_list > ')' > '{' > function_body > '}';
    function_expression %= lit("function") > -identifier > '(' > -formal_parameter_list > ')' > '{' > function_body > '}';
    formal_parameter_list %= identifier % ",";
    function_body %= *source_element;

    statement %=
        "{" > *statement > "}"
        | variable_statement
        | empty_statement
        | !(lit("function")) >> expression > ";"
        | if_statement
        | iteration_statement
        | continue_statement
        | break_statement
        | return_statement
        | with_statement
        | labelled_statement
        | switch_statement
        | throw_statement
        | try_statement
        | lit("debugger") >> ";";
    
    variable_statement %= "var" >> (variable_declaration % ",") >> ";";
    variable_declaration %= identifier >> -("=" >> assignment_expression);

    empty_statement %= char_(';');

    if_statement %= lit("if") >> "(" >> expression >> ")" >> statement >> -(lit("else") >> statement);

    iteration_statement %=
        lit("do") >> statement >> "while" >> "(" >> expression >> ")" >> ";"
        | lit("while") >> "(" >> expression >> ")" >> statement
        | lit("for") >> "(" >> -expression >> ";" >> -expression >> ";" >> -expression >> ")" >> statement
        | lit("for") >> "(" >> "var" >> (variable_declaration % ",") >> ";" >> -expression >> ";" >> -expression >> ")" >> statement
        | lit("for") >> "(" >> lhs_expression >> "in" >> expression >> ")" >> statement
        | lit("for") >> "(" >> "var" >> variable_declaration >> "in" >> expression >> ")" >> statement;

    continue_statement %= "continue" > !eol > -identifier > ";";
    break_statement %= "break" > !eol > -identifier > ";";
    return_statement %= "return" > !eol > -expression > ";";

    with_statement %= lit("with") > "(" > expression > ")" > statement;

    switch_statement %= lit("switch") > "(" > expression > ")" > "{" > *case_clause > -default_clause > *case_clause > "}";
    case_clause %= "case" > expression > ":" > *statement;
    default_clause %= lit("default") > ":" > *statement;

    labelled_statement %= identifier >> ":" > statement;

    throw_statement %= "throw" > !eol > expression > ";";

    try_statement %= lit("try") >> "{" >> *statement >> "}" >> (catch_block || finally_block);
    catch_block %= lit("catch") >> "(" >> identifier >> ")" >> "{" >> *statement >> "}";
    finally_block %= lit("finally") >> "{" >> *statement >> "}";

    expression %= assignment_expression % lit(',');

    assignment_expression %= *(lhs_expression >> assignment_operator) >> conditional_expression;
    assignment_operator %=
        string("=")
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

    logical_or_expression %= logical_and_expression >> *("||" > logical_and_expression);

    logical_and_expression %= bitwise_or_expression >> *("&&" > bitwise_or_expression);

    bitwise_or_expression %= bitwise_xor_expression >> *("|" > bitwise_xor_expression);

    bitwise_xor_expression %= bitwise_and_expression >> *("^" > bitwise_and_expression);

    bitwise_and_expression %= equality_expression >> *("&" > equality_expression);

    equality_expression %= relational_expression >> *(equality_operator > relational_expression);
    equality_operator %= 
        string("==")
        | string("!=")
        | string("===")
        | string("!==");

    relational_expression %= shift_expression >> *(relational_operator > shift_expression);
    relational_operator %= 
        string("<")
        | string (">")
        | string("<=")
        | string(">=")
        | string("instanceof")
        | string("in");

    shift_expression %= additive_expression >> *(shift_operator > additive_expression);
    shift_operator %=
        string("<<")
        | string(">>")
        | string(">>>");

    additive_expression %= multiplicative_expression >> *(additive_operator > multiplicative_expression);
    additive_operator %= string("+") | string("-");

    multiplicative_expression %= unary_expression >> *(multiplicative_operator > unary_expression);
    multiplicative_operator %=
        string("*")
        | string("/")
        | string("%");

    unary_expression %= *unary_operator >> postfix_expression;
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

    member_expression %= (primary_expression | function_expression)
        >> *(('[' >> expression >> ']') | ('.' >> identifier_name));
        //| "new" >> member_expression >> arguments;

    primary_expression %=
        this_reference
        | identifier
        | literal 
        | array_literal 
        //| object_literal 
        | '(' >> expression >> ')';

    array_literal = '[' >> *lit(',') >> assignment_expression
        >> *(+lit(',') >> assignment_expression) >> *lit(',') >> ']';

    this_reference %= string("this")[_val = construct<ast::This>(_1)];

    // Lexical Grammar
    identifier %= (identifier_name - reserved_word);

    // FIXME: unicode characters
    identifier_name %= identifier_start >> *alnum;

    identifier_start %= alpha | char_('$') | char_('_');

    reserved_word %=
        keyword
        | future_reserved_word
        | string("null")
        | string("true")
        | string("false");

    keyword %=
        string("break") | string("do") | string("case") | string("else")
        | string("catch") | string( "finally") | string("continue") | string("for")
        | string("debugger") | string("function") | string( "default") | string("if")
        | string("delete") | string("in") | string("instanceof") | string("new")
        | string( "return") | string("switch") | string("this") | string("throw")
        | string("try") | string("typeof") | string( "var") | string("void")
        | string("while") | string( "with");

    future_reserved_word %=
        string("class") | string("enum") | string("const") | string("export")
        | string("implements") | string("let") | string("interface") | string("package")
        | string("yield") | string("extends") | string("import") | string("private")
        | string("protected") | string("super") | string("public") |string( "static");

    literal %=
        null_literal
        | bool_
        | numeric_literal
        | string_literal;

    null_literal %= string("null")[_val = construct<ast::Null>(_1)];

    numeric_literal %= double_ | int_;

    string_literal %=
        '"' >> lexeme[*(~char_('"'))] >> '"'
        | '\'' >> lexeme[*(~char_('\''))] >> '\'';

    on_error<fail>(program, error_handler(_4, _3, _2));

    BOOST_SPIRIT_DEBUG_NODE(program);
    BOOST_SPIRIT_DEBUG_NODE(source_element);
    BOOST_SPIRIT_DEBUG_NODE(function_declaration);
    //BOOST_SPIRIT_DEBUG_NODE(function_expression);
    //BOOST_SPIRIT_DEBUG_NODE(formal_parameter_list);
    BOOST_SPIRIT_DEBUG_NODE(function_body);
    BOOST_SPIRIT_DEBUG_NODE(source_element);
    BOOST_SPIRIT_DEBUG_NODE(statement);
    BOOST_SPIRIT_DEBUG_NODE(variable_statement);
    BOOST_SPIRIT_DEBUG_NODE(variable_declaration);
    BOOST_SPIRIT_DEBUG_NODE(empty_statement);
    BOOST_SPIRIT_DEBUG_NODE(if_statement);
    BOOST_SPIRIT_DEBUG_NODE(iteration_statement);
    BOOST_SPIRIT_DEBUG_NODE(continue_statement);
    BOOST_SPIRIT_DEBUG_NODE(break_statement);
    BOOST_SPIRIT_DEBUG_NODE(return_statement);
    BOOST_SPIRIT_DEBUG_NODE(with_statement);
    BOOST_SPIRIT_DEBUG_NODE(labelled_statement);
    BOOST_SPIRIT_DEBUG_NODE(switch_statement);
    BOOST_SPIRIT_DEBUG_NODE(switch_statement);
    BOOST_SPIRIT_DEBUG_NODE(case_clause);
    BOOST_SPIRIT_DEBUG_NODE(default_clause);
    BOOST_SPIRIT_DEBUG_NODE(catch_block);
    BOOST_SPIRIT_DEBUG_NODE(finally_block);
    BOOST_SPIRIT_DEBUG_NODE(throw_statement);
    BOOST_SPIRIT_DEBUG_NODE(try_statement);
    //BOOST_SPIRIT_DEBUG_NODE(expression);
    //BOOST_SPIRIT_DEBUG_NODE(assignment_expression);
    //BOST_SPIRIT_DEBUG_NODE(assignment_operator);
    //BOOST_SPIRIT_DEBUG_NODE(conditional_clauses);
    //BOOST_SPIRIT_DEBUG_NODE(conditional_expression);
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
    //BOOST_SPIRIT_DEBUG_NODE(lhs_expression);
    //BOOST_SPIRIT_DEBUG_NODE(call_expression);
    //BOOST_SPIRIT_DEBUG_NODE(arguments);
    //BOOST_SPIRIT_DEBUG_NODE(new_expression);
    //BOOST_SPIRIT_DEBUG_NODE(member_expression);
    //BOOST_SPIRIT_DEBUG_NODE(primary_expression);
    //BOOST_SPIRIT_DEBUG_NODE(array_literal);
    //BOOST_SPIRIT_DEBUG_NODE(this_reference);
    BOOST_SPIRIT_DEBUG_NODE(identifier);
    BOOST_SPIRIT_DEBUG_NODE(identifier_name);
    BOOST_SPIRIT_DEBUG_NODE(identifier_start);
    BOOST_SPIRIT_DEBUG_NODE(reserved_word);
    BOOST_SPIRIT_DEBUG_NODE(keyword);
    BOOST_SPIRIT_DEBUG_NODE(future_reserved_word);
    BOOST_SPIRIT_DEBUG_NODE(literal);
    BOOST_SPIRIT_DEBUG_NODE(null_literal);
    BOOST_SPIRIT_DEBUG_NODE(numeric_literal);
    BOOST_SPIRIT_DEBUG_NODE(string_literal);
  }

  qi::rule<Iterator, ascii::space_type> program;
  qi::rule<Iterator, ascii::space_type> source_element;
  qi::rule<Iterator, ascii::space_type> function_declaration;
  qi::rule<Iterator, ast::FunctionExpression(), ascii::space_type> function_expression;
  qi::rule<Iterator, std::vector<std::string>(), ascii::space_type> formal_parameter_list;
  qi::rule<Iterator, ascii::space_type> function_body;

  qi::rule<Iterator, ascii::space_type> statement;
  qi::rule<Iterator, ascii::space_type> variable_statement;
  qi::rule<Iterator, ascii::space_type> variable_declaration;
  qi::rule<Iterator, ascii::space_type> empty_statement;
  qi::rule<Iterator, ascii::space_type> if_statement;
  qi::rule<Iterator, ascii::space_type> iteration_statement;
  qi::rule<Iterator, ascii::space_type> continue_statement;
  qi::rule<Iterator, ascii::space_type> break_statement;
  qi::rule<Iterator, ascii::space_type> return_statement;
  qi::rule<Iterator, ascii::space_type> with_statement;
  qi::rule<Iterator, ascii::space_type> labelled_statement;
  qi::rule<Iterator, ascii::space_type> switch_statement;
  qi::rule<Iterator, ascii::space_type> case_clause;
  qi::rule<Iterator, ascii::space_type> default_clause;
  qi::rule<Iterator, ascii::space_type> catch_block;
  qi::rule<Iterator, ascii::space_type> finally_block;
  qi::rule<Iterator, ascii::space_type> throw_statement;
  qi::rule<Iterator, ascii::space_type> try_statement;

  qi::rule<Iterator, ast::Expression(), ascii::space_type> expression;

  qi::rule<Iterator, ast::AssignmentExpression(), ascii::space_type> assignment_expression;
  qi::rule<Iterator, std::string(), ascii::space_type> assignment_operator;

  qi::rule<Iterator, ast::ConditionalClauses(), ascii::space_type> conditional_clauses;
  qi::rule<Iterator, ast::ConditionalExpression(), ascii::space_type> conditional_expression;
  qi::rule<Iterator, ascii::space_type> logical_or_expression;

  qi::rule<Iterator, ascii::space_type> logical_and_expression;

  qi::rule<Iterator, ascii::space_type> bitwise_or_expression;

  qi::rule<Iterator, ascii::space_type> bitwise_xor_expression;

  qi::rule<Iterator, ascii::space_type> bitwise_and_expression;

  qi::rule<Iterator, ascii::space_type> equality_expression;
  qi::rule<Iterator, std::string(), ascii::space_type> equality_operator;

  qi::rule<Iterator, ascii::space_type> relational_expression;
  qi::rule<Iterator, std::string(), ascii::space_type> relational_operator;

  qi::rule<Iterator, ascii::space_type> shift_expression;
  qi::rule<Iterator, std::string(), ascii::space_type> shift_operator;

  qi::rule<Iterator, ascii::space_type> additive_expression;
  qi::rule<Iterator, std::string(), ascii::space_type> additive_operator;

  qi::rule<Iterator, ascii::space_type> multiplicative_expression;
  qi::rule<Iterator, std::string(), ascii::space_type> multiplicative_operator;

  qi::rule<Iterator, ascii::space_type> unary_expression;
  qi::rule<Iterator, std::string(), ascii::space_type> unary_operator;

  qi::rule<Iterator, ascii::space_type> postfix_expression;
  qi::rule<Iterator, std::string(), ascii::space_type> postfix_operator;

  qi::rule<Iterator, ast::LhsExpression(), ascii::space_type> lhs_expression;

  qi::rule<Iterator, ast::CallExpression(), ascii::space_type> call_expression;
  qi::rule<Iterator, ast::Arguments(), ascii::space_type> arguments;

  qi::rule<Iterator, ast::NewExpression(), ascii::space_type> new_expression;

  qi::rule<Iterator, ast::MemberExpression(), ascii::space_type> member_expression;
  qi::rule<Iterator, ast::PrimaryExpression(), ascii::space_type> primary_expression;
  qi::rule<Iterator, ast::ArrayLiteral(), ascii::space_type> array_literal;
  qi::rule<Iterator, ast::This(), ascii::space_type> this_reference;

  qi::rule<Iterator, std::string(), ascii::space_type> identifier;
  qi::rule<Iterator, std::string(), ascii::space_type> identifier_name;
  qi::rule<Iterator, char(), ascii::space_type> identifier_start;
  qi::rule<Iterator, std::string(), ascii::space_type> reserved_word;
  qi::rule<Iterator, std::string(), ascii::space_type> keyword;
  qi::rule<Iterator, std::string(), ascii::space_type> future_reserved_word;

  qi::rule<Iterator, ast::Literal(), ascii::space_type> literal;
  qi::rule<Iterator, ast::Null(), ascii::space_type> null_literal;
  qi::rule<Iterator, ast::Numeric(), ascii::space_type> numeric_literal;
  qi::rule<Iterator, std::string(), ascii::space_type> string_literal;
};

} // namespace kungjs

#endif // KUNGJS_GRAMMAR_H_

