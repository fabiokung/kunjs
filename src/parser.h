#ifndef KUNJS_PARSER_H_
#define KUNJS_PARSER_H_

#include <string>

namespace kunjs {

class Parser {
 public:
  bool parse(std::string code);
};


} // namespace kunjs

#endif // KUNJS_PARSER_H_

