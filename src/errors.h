//
// Created by ech on 9/22/18.
//

#ifndef FORMAL_PATCH_ERRORS_H
#define FORMAL_PATCH_ERRORS_H

#include <string>

namespace fpat {
struct Error {
  int code;
  std::string msg;
};

namespace errors {

} // namespace fpat::errors
} // namespace fpat

#endif //FORMAL_PATCH_ERRORS_H
