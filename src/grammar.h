#ifndef KUNGJS_GRAMMAR_H_
#define KUNGJS_GRAMMAR_H_

#include "ast.h"
#include <boost/tr1/memory.hpp>
#include <boost/spirit/home/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <boost/spirit/include/phoenix1_new.hpp>
#include <vector>

namespace kungjs {

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

template <typename Iterator>
struct javascript_grammar : qi::grammar<Iterator, std::tr1::shared_ptr<ast::ProgramNode>(), ascii::space_type> {
  javascript_grammar() : javascript_grammar::base_type(program) {

    using qi::lit;
    using qi::lexeme;
    using qi::raw;
    using qi::eol;
    using qi::eps;
    using qi::_val;
    using qi::_1;
    using qi::_2;
    using qi::_3;
    using qi::_a;
    using qi::int_;
    using qi::double_;
    using ascii::string;
    using ascii::char_;
    using ascii::alpha;
    using ascii::alnum;
    using boost::get;
    using boost::phoenix::new_;
    using boost::phoenix::construct;
    using boost::phoenix::bind;

    // Top level
    program = (*statement)  [_val = construct<std::tr1::shared_ptr<ast::ProgramNode> >(new_<ast::ProgramNode>(_1))];

    statement = 
        '{' >> (*statement)       [_val = construct<std::tr1::shared_ptr<ast::Node> >(new_<ast::BlockNode>(_1))] >> '}'  
        | empty_statement         [_val = _1]
        | expression_statement    [_val = _1];
    
    empty_statement = char_(';')[_val = construct<std::tr1::shared_ptr<ast::Node> >(new_<ast::EmptyNode>())];

    expression_statement = !lit("function") >> expression[_val = _1] >> ';';

    expression = (assignment_expression >> *(',' >> assignment_expression))
        [_val = bind(&ast::ExpressionNode::create, _1, _2)];

    assignment_expression %= 
        conditional_expression[_val = _1]
        | (lhs_expression >> assignment_operator >> assignment_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, _2, _1, _3)];

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

    conditional_expression =
        logical_or_expression   [_val = _1];
        //>> -(lit('?')
             //>> assignment_expresssion
             //>> ':'
             //>> assignment_expression);

    logical_or_expression =
        logical_and_expression[_val = _1]
        | (logical_or_expression >> "||" >> logical_and_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, "||", _1, _3)];

    logical_and_expression =
        bitwise_or_expression[_val = _1]
        | (logical_and_expression >> "&&" >> bitwise_or_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, "&&", _1, _3)];

    bitwise_or_expression =
        bitwise_xor_expression[_val = _1]
        | (bitwise_or_expression >> '|' >> bitwise_xor_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, "|", _1, _3)];

    bitwise_xor_expression =
        bitwise_and_expression[_val = _1]
        | (bitwise_xor_expression >> '^' >> bitwise_and_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, "^", _1, _3)];

    bitwise_and_expression =
        equality_expression[_val = _1]
        | (bitwise_and_expression >> '&' >> equality_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, "&", _1, _3)];

    equality_expression =
        relational_expression[_val = _1]

        | (equality_expression >> "==" >> relational_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, "==", _1, _3)]

        | (equality_expression >> "!=" >> relational_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, "!=", _1, _3)]

        | (equality_expression >> "===" >> relational_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, "===", _1, _3)]

        | (equality_expression >> "!==" >> relational_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, "!==", _1, _3)];

    relational_expression = 
        shift_expression[_val = _1]

        | (relational_expression >> '<' >> shift_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, "<", _1, _3)]

        | (relational_expression >> '>' >> shift_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, ">", _1, _3)]

        | (relational_expression >> "<=" >> shift_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, "<=", _1, _3)]

        | (relational_expression >> ">=" >> shift_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, ">=", _1, _3)]

        | (relational_expression >> "instanceof" >> shift_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, "instanceof", _1, _3)]

        | (relational_expression >> "in" >> shift_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, "in", _1, _3)];

    shift_expression =
        additive_expression[_val = _1]

        | (shift_expression >> "<<" >> additive_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, "<<", _1, _3)]

        | (shift_expression >> ">>" >> additive_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, ">>", _1, _3)]

        | (shift_expression >> ">>>" >> additive_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, ">>>", _1, _3)];

    additive_expression =
        multiplicative_expression[_val = _1]

        | (additive_expression >> '+' >> multiplicative_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, "+", _1, _3)]

        | (additive_expression >> '-' >> multiplicative_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, "-", _1, _3)];

    multiplicative_expression =
        unary_expression[_val = _1]
        | (multiplicative_expression >> '*' >> unary_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, "*", _1, _3)]

        | (multiplicative_expression >> '/' >> unary_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, "/", _1, _3)]

        | (multiplicative_expression >> '%' >> unary_expression)
            [_val = bind(&ast::BinaryOperatorNode::create, "%", _1, _3)];

    unary_expression = 
        postfix_expression              [_val = _1]
        | "delete" >> unary_expression  [_val = bind(&ast::DeleteNode::create, _1)]
        //| "void" >> unary_expression    
        //| "typeof" >> unary_expression
        | "++" >> unary_expression      [_val = bind(&ast::PreIncrementNode::create, _1)]
        | "--" >> unary_expression      [_val = bind(&ast::PreDecrementNode::create, _1)]
        //| '+' >> unary_expression
        //| '-' >> unary_expression
        //| '~' >> unary_expression
        | '!' >> unary_expression       [_val = bind(&ast::NotNode::create, _1)];

    postfix_expression = 
        lhs_expression                        [_val = _1]
        | (lhs_expression >> !eol >> "++")    [_val = bind(&ast::PostIncrementNode::create, _1)]
        | (lhs_expression >> !eol >> "--")    [_val = bind(&ast::PostDecrementNode::create, _1)];

    lhs_expression %=
        new_expression;
        //| call_expression;

    //call_expression %=
        //member_expression >> arguments
        //| call_expression >> arguments;
        //| call_expression >> '[' >> expression >> ']'
        //| call_expression >> '.' >> identifier_name;

    //arguments %=
        //'('
        //>> -(assignment_expression
             //>> *(',' >> assignment_expression))
        //>> ')';

    new_expression =
        member_expression           [_val = _1]
        | "new" >> new_expression   [_val = bind(&ast::NewNode::create, _1)];

    member_expression %=
        primary_expression;
        //| function_expression
        //| member_expression >> '[' >> expression >> ']'
        //| member_expression >> '.' >> identifier_name
        //| "new" >> member_expression >> arguments;

    primary_expression =
        lit("this")                   [_val = construct<std::tr1::shared_ptr<ast::ThisNode> >(new_<ast::ThisNode>())]
        | identifier                  [_val = _1]
        | literal                     [_val = _1]
        | '(' >> expression           [_val = _1] >> ')';

    // Lexical Grammar
    identifier =
        (identifier_name - reserved_word)[_val = bind(&ast::IdentifierNode::create, _1)];

    // FIXME: unicode characters
    identifier_name %=
        identifier_start >> *alnum;

    identifier_start %= alpha | string('$') | string('_');

    reserved_word =
        keyword                 [_val = construct<std::tr1::shared_ptr<ast::Node> >(new_<ast::KeywordNode>(_1))]
        | future_reserved_word  [_val = construct<std::tr1::shared_ptr<ast::Node> >(new_<ast::KeywordNode>(_1))]
        | null_literal          [_val = _1]
        | boolean_literal       [_val = _1];

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

    literal =
        null_literal      [_val = _1]
        | boolean_literal [_val = _1]
        | numeric_literal [_val = _1]
        | string_literal  [_val = _1];

    null_literal = lit("null")[_val = construct<std::tr1::shared_ptr<ast::NullNode> >(new_<ast::NullNode>())];

    boolean_literal = 
        lit("true")    [_val = bind(&ast::LiteralNode<bool>::create, true)]
        | lit("false")   [_val = bind(&ast::LiteralNode<bool>::create, false)];

    numeric_literal = 
        int_        [_val = bind(&ast::LiteralNode<int>::create, _1)]
        | double_   [_val = bind(&ast::LiteralNode<double>::create, _1)];

    string_literal =
        raw[lexeme['"' >> *(char_ - '"') >> '"']]            [_val = bind(&ast::LiteralNode<std::string>::create, _1)]
        | raw[lexeme['\'' >> *(char_ - '\'') >> '\'']]       [_val = bind(&ast::LiteralNode<std::string>::create, _1)];

  }

  qi::rule<Iterator, std::tr1::shared_ptr<ast::ProgramNode>(), ascii::space_type> program;
  //qi::rule<Iterator, std::tr1::shared_ptr<ast::FunctionNode>(), ascii::space_type> function_declaration;
  //qi::rule<Iterator, std::tr1::shared_ptr<ast::FunctionNode>(), ascii::space_type> function_expression;
  //qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> function_body;

  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> statement;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> empty_statement;

  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> expression_statement;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> expression;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> assignment_expression;
  qi::rule<Iterator, std::string(), ascii::space_type> assignment_operator;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> conditional_expression;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> logical_or_expression;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> logical_and_expression;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> bitwise_or_expression;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> bitwise_xor_expression;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> bitwise_and_expression;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> equality_expression;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> relational_expression;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> shift_expression;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> additive_expression;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> multiplicative_expression;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> unary_expression;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> postfix_expression;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> lhs_expression;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> new_expression;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> member_expression;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> primary_expression;

  qi::rule<Iterator, std::tr1::shared_ptr<ast::IdentifierNode>(), ascii::space_type> identifier;
  qi::rule<Iterator, std::string(), ascii::space_type> identifier_name;
  qi::rule<Iterator, std::string(), ascii::space_type> identifier_start;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> reserved_word;
  qi::rule<Iterator, std::string(), ascii::space_type> keyword;
  qi::rule<Iterator, std::string(), ascii::space_type> future_reserved_word;

  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> literal;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> null_literal;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::LiteralNode<bool> >, ascii::space_type> boolean_literal;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::Node>(), ascii::space_type> numeric_literal;
  qi::rule<Iterator, std::tr1::shared_ptr<ast::LiteralNode<std::string> >(), ascii::space_type> string_literal;
};

} // namespace kungjs

#endif // KUNGJS_GRAMMAR_H_

