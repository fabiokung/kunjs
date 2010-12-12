#ifndef KUNGJS_PARSER_H_
#define KUNGJS_PARSER_H_

#include <boost/tr1/memory.hpp>
#include <string>

namespace kungjs {

namespace ast {
class Node;
}

class Parser {
 public:
  std::tr1::shared_ptr<ast::Node> parse(const std::string& code);
};


} // namespace kungjs

#endif // KUNGJS_PARSER_H_

