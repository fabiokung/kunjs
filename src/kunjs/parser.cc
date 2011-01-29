#include "kunjs/parser.h"
#include "kunjs/grammar.h"
#include "kunjs/printer.h"

#include <boost/spirit/include/qi_parse.hpp>
#include <string>

namespace kunjs {

bool Parser::parse(std::string code) {
  typedef kunjs::javascript_grammar<std::string::const_iterator> javascript;
  javascript grammar;
  std::string::const_iterator begin = code.begin();
  std::string::const_iterator end = code.end();
  ast::Program ast;

  bool result = phrase_parse(begin, end, grammar, boost::spirit::ascii::space, ast);

  ASTPrinter print_ast(0);
  print_ast(ast);

  std::cout << "Result: " << std::boolalpha << result << "\n";
  if (result && begin == end) {
    std::cout << "-------------------------\n";
    std::cout << "Parsing succeeded\n";
    std::cout << "\n-------------------------\n";
    return true;
  }
  else {
    std::string rest(begin, end);
    std::cout << "-------------------------\n";
    std::cout << "Parsing failed\n";
    std::cout << "stopped at: \": " << rest << "\"\n";
    std::cout << "-------------------------\n";
    return false;
  }

}

} // namespace kunjs

