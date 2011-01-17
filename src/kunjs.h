#ifndef KUNJS_KUNJS_H_
#define KUNJS_KUNJS_H_

#include <string>

namespace kunjs {

class Runner {
 public:
  std::string run(std::string code);
};

}

#endif // KUNJS_KUNJS_H_
