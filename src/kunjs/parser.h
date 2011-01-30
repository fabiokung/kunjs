#ifndef KUNJS_PARSER_H_
#define KUNJS_PARSER_H_

#if defined(_MSC_VER)
#pragma once
#endif

#include "kunjs/ast.h"
#include <string>

//#define BOOST_SPIRIT_DEBUG

namespace kunjs {

namespace {
static ast::Program OPTIONAL = ast::Program();
}

class Parser {
 public:
  bool parse(std::string code, ast::Program& ast = OPTIONAL);
};

} // namespace kunjs

#endif // KUNJS_PARSER_H_

