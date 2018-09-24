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

static Error cant_convert_atom() { return Error{100, "Cannot convert an atom"}; }

} // namespace fpat::errors
} // namespace fpat

#endif //FORMAL_PATCH_ERRORS_H
