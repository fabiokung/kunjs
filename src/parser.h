#ifndef KUNGJS_PARSER_H_
#define KUNGJS_PARSER_H_

#include "ast.h"

#include <boost/tr1/memory.hpp>
#include <string>

namespace kungjs {

class Parser {
 public:
  bool parse(std::string code);
};


} // namespace kungjs

#endif // KUNGJS_PARSER_H_

