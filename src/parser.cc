#include "parser.h"
#include "ast.h"
#include "grammar.h"

#include <boost/tr1/memory.hpp>
#include <boost/spirit/include/qi.hpp>
#include <string>

namespace kungjs {

std::tr1::shared_ptr<ast::Node> Parser::parse(const std::string& code) {
  typedef kungjs::javascript_grammar<std::string::const_iterator> javascript;
  javascript grammar;
  std::string::const_iterator begin = code.begin();
  std::string::const_iterator end = code.end();
  std::tr1::shared_ptr<ast::Node> program;
  bool result = phrase_parse(begin, end, grammar, boost::spirit::ascii::space, program);

  if (result && begin == end)
  {
    std::cout << "-------------------------\n";
    std::cout << "Parsing succeeded\n";
    std::cout << "\n-------------------------\n";
  }
  else
  {
    std::string rest(begin, end);
    std::cout << "-------------------------\n";
    std::cout << "Parsing failed\n";
    std::cout << "stopped at: \": " << rest << "\"\n";
    std::cout << "-------------------------\n";
  }

  return program;
}

}
