#ifndef KUNJS_PARSER_H_
#define KUNJS_PARSER_H_

#if defined(_MSC_VER)
#pragma once
#endif

#include <string>

//#define BOOST_SPIRIT_DEBUG

namespace kunjs {

class Parser {
 public:
  bool parse(std::string code);
};

} // namespace kunjs

#endif // KUNJS_PARSER_H_

