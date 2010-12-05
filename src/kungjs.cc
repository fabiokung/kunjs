#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "kungjs.h"

namespace client {

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

template <typename Iterator>
bool parse_numbers(Iterator first, Iterator last) {
  using qi::double_;
  using qi::phrase_parse;
  using ascii::space;

  bool r = phrase_parse(
      first,                          /*< start iterator >*/
      last,                           /*< end iterator >*/
      double_ >> *(',' >> double_),   /*< the parser >*/
      space                           /*< the skip-parser >*/
      );
  if (first != last)
    return false;
  return r;
}

}

////////////////////////////////////////////////////////////////////////////
//  Main program
////////////////////////////////////////////////////////////////////////////
int
main() {
  std::cout << "/////////////////////////////////////////////////////////\n\n";
  std::cout << "\t\tA comma separated list parser for Spirit...\n\n";
  std::cout << "/////////////////////////////////////////////////////////\n\n";

  std::cout << "Give me a comma separated list of numbers.\n";
  std::cout << "Type [q or Q] to quit\n\n";

  std::string str;
  while (getline(std::cin, str)) {
    if (str.empty() || str[0] == 'q' || str[0] == 'Q')
      break;

    if (client::parse_numbers(str.begin(), str.end())) {
      std::cout << "-------------------------\n";
      std::cout << "Parsing succeeded\n";
      std::cout << str << " Parses OK: " << std::endl;
    }
    else {
      std::cout << "-------------------------\n";
      std::cout << "Parsing failed\n";
      std::cout << "-------------------------\n";
    }
  }

  std::cout << "Bye... :-) \n\n";
  return 0;
}

