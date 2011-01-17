#include "parser.h"
#include "ast.h"
#include "grammar.h"

#include <boost/spirit/include/qi.hpp>
#include <string>

namespace kunjs {

bool Parser::parse(std::string code) {
  typedef kunjs::javascript_grammar<std::string::iterator> javascript;
  javascript grammar;
  std::string::iterator begin = code.begin();
  std::string::iterator end = code.end();
  bool result = phrase_parse(begin, end, grammar, boost::spirit::ascii::space);

  std::cout << "Result: " << std::boolalpha << result << "\n";
  if (result && begin == end)
  {
    std::cout << "-------------------------\n";
    std::cout << "Parsing succeeded\n";
    std::cout << "\n-------------------------\n";
    return true;
  }
  else
  {
    std::string rest(begin, end);
    std::cout << "-------------------------\n";
    std::cout << "Parsing failed\n";
    std::cout << "stopped at: \": " << rest << "\"\n";
    std::cout << "-------------------------\n";
    return false;
  }

}

} // namespace kunjs

