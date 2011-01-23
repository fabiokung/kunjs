#include "kunjs/kunjs.h"

#include <iostream>

namespace kunjs {

std::string Runner::run(std::string code) {
  std::cout << "Running: " << code << "\n";
  return code;
}

}

