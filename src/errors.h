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

static Error cant_convert_atom() {
  return Error{100, "Cannot convert an atom."};
}
static Error as_tuple_cant_find_all_types() {
  return Error{101, "as_tuple: not all atom types in AtomVec correspond to the template."};
};
static Error as_tuple_avec_is_too_short() {
  return Error{102, "as_tuple: AtomVec is too short."};
}
static Error as_tuple_avec_is_not_same_length() {
  return Error{103, "as_tuple: AtomVec and template arguments have different lengths."};
}

} // namespace fpat::errors
} // namespace fpat

#endif //FORMAL_PATCH_ERRORS_H
