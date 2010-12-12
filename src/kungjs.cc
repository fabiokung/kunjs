#include "kungjs.h"
#include "grammar.h"

#include <iostream>

namespace kungjs {

std::string Runner::run(std::string code) {
  std::cout << "Running: " << code << "\n";
  return code;
}

}

