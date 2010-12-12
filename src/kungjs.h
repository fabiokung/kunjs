#ifndef KUNGJS_KUNGJS_H_
#define KUNGJS_KUNGJS_H_

#include <string>

namespace kungjs {

class Runner {
 public:
  std::string run(std::string code);
};

}

#endif // KUNGJS_KUNGJS_H_
