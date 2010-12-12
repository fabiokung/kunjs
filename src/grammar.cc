#include "grammar.h"
#include <boost/spirit/include/qi.hpp>

namespace kungjs {

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

template <typename Iterator>
struct complex_javascript_grammar : qi::grammar<Iterator, program_node(), ascii::space_type> {
  javascript_grammar() : javascript_grammar::base_type(program) {

    using qi::lit;
    using qi::lexeme;
    using qi:eol;
    using ascii::string;
    using ascii::char_;

    // Top level
    program %= *(source_element);

    source_element %= statement | function_declaration;

    function_declaration %=
            lit("function")
        >>  identifier
        >>  '(' >> *identifier >> ')'
        >>  '{' >> function_body >> '}';

    function_expression %=
            lit("function")
        >>  -identifier
        >>  '(' >> *identifier >> ')'
        >>  '{' >> function_body >> '}';

    function_body %= *(source_element);

    // Statements
    statement %= 
        '{' >> *statement >> '}'
        | variable_statement
        | empty_statement
        | expression_statement
        | if_statement
        | iteration_statement
        //| continue_statement
        //| break_statement
        | return_statement;
        //| with_statement
        //| labelled_statement
        //| switch_statement
        //| throw_statement
        //| try_statement
        //| debugger_statement;
    
    variable_statement %=
        lit("var") > variable_declaration >> *(',' >> variable_declaration) > ';';

    variable_declaration %= string >> -('=' >> assignment_expression);

    empty_statement %= ';';

    expression_statement %= !lit("function") >> expression >> ';';

    if_statement %=
        lit("if") >> '(' >> expression >> ')'
        >> statement
        >> -("else" >> statement);

    iteration_statement %=
        lit("do") >> statement >> "while" >> expression >> ';'
        | "while" >> '(' >> expression >> ')' >> statement
        | "for" >> '(' > -expression > ';' > expression > ';' > expression > ')' > statement;

    return_statement %= lit("return") >> -expression >> ';';

    // Expressions
    expression %= assignment_expression >> *(char_(',') >> assignment_expression);

    assignment_expression %= 
        *(lhs_expression >> assignment_operator) >> conditional_expression;

    assignment_operator =
        lit("=")
        | "*="
        | "/="
        | "%="
        | "+="
        | "-="
        | "<<="
        | ">>="
        | ">>>="
        | "&="
        | "^="
        | "|=";

    conditional_expression %= logical_or_expression >> -(lit('?') >> assignment_expresssion >> ':' >> assignment_expression);

    logical_or_expression %= *(logical_and_expression >> "||") >> logical_and_expression;

    logical_and_expression %= *(bitwise_or_expression >> "&&") >> bitwise_or_expression;

    bitwise_or_expression %= *(bitwise_xor_expression >> '|') >> bitwise_xor_expression;

    bitwise_xor_expression %= *(bitwise_and_expression >> '^') >> bitwise_and_expression;

    bitwise_and_expression %= *(equality_expression >> '&') >> equality_expression;

    equality_expression %=
        relational_expression >> 
        *("==" >> relational_expression
          | "!=" >> relational_expression
          | "===" >> relational_expression
          | "!==" >> relational_expression);

    relational_expression %= 
        shift_expression >>
        *('<' >> shift_expression
          | '>' >> shift_expression
          | "<=" >> shift_expression
          | ">=" >> shift_expression
          | "instanceof" >> shift_expression
          | "in" >> shift_expression);

    shift_expression %=
        additive_expression >>
        *("<<" >> additive_expression
          | ">>" >> additive_expression
          | ">>>" >> additive_expression);

    additive_expression %=
        multiplicative_expression >>
        *('+' >> multiplicative_expression
          | '-' >> multiplicative_expression);

    multiplicative_expression %=
        unary_expression >>
        *('*' >> unary_expression
          | '/' >> unary_expression
          | '%' >> unary_expression);

    unary_expression %= 
        *(lit("delete")
          | "void"
          | "typeof"
          | "++"
          | "--"
          | '+'
          | '-'
          | '~'
          | '!')
        >> postfix_expression;

    postfix_expression %= 
        lhs_expression
        >> -(!eol >> "++"
             | !eol >> "--");

    lhs_expression %=
        new_expression
        | call_expression;

    call_expression %=
        member_expression >> arguments
        >> *(arguments
             | '[' >> expression >> ']'
             | '.' >> identifier_name);

    arguments %=
        '('
        >> -(assignment_expression
             >> *(',' >> assignment_expression))
        >> ')';

    new_expression %= *lit("new") >> member_expression;

    member_expression %=
        ((primary_expression | function_expression)
        >> *('[' >> expression >> ']'
             | '.' >> identifier_name)
        | "new" >> member_expression >> arguments);

    primary_expression %=
        lit("this")
        | identifier
        | literal
        //| array_literal
        //| object_literal
        | '(' >> expression >> ')';



  }

  qi::rule<Iterator, program_node(), ascii::space_type> program;
  qi::rule<Iterator, std::string(), ascii::space_type> statement;
  qi::rule<Iterator, std::string(), ascii::space_type> function_declaration;
  qi::rule<Iterator, std::string(), ascii::space_type> function_body;
};

} // namespace kungjs

