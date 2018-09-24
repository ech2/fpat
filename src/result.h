#include <utility>

//
// Created by ech on 9/21/18.
//

#ifndef FORMAL_PATCH_RESULT_H
#define FORMAL_PATCH_RESULT_H

#include <string>
#include <variant>
#include <optional>
#include <functional>

#include "errors.h"

namespace fpat {

template<typename T>
class Result : private std::variant<T, Error> {
 public:
  Result() = delete;
  explicit Result(T x) : std::variant<T, Error>(x) {};
  explicit Result(Error x) : std::variant<T, Error>(x) {};

  bool is_error() const {
    return std::get_if<0>(this) == nullptr;
  }

  T *get_value_n() {
    return std::get_if<0>(this);
  }

  Error *get_error_n() {
    assert(std::get_if<1>(this) != nullptr);
    return std::get_if<1>(this);
  }

  std::optional<T> get_value() {
    if (auto v = get_value_n()) {
      return std::make_optional(*v);
    } else {
      return std::nullopt;
    }
  }

  T get_value_or(T other) {
    if (auto v = get_value_n()) {
      return *v;
    } else {
      return other;
    }
  }

  template<typename Other>
  Result<Other> fmap(std::function<Other(const T)> f) {
    if (is_error()) {
      return Result<Other>(*get_error_n());
    } else {
      return Result<Other>(f(*get_value_n()));
    }
  }

  template<typename Other>
  Result<Other> map(std::function<Result<Other>(const T)> f) {
    if (is_error()) {
      return Result<Other>(get_error_n().value());
    } else {
      return f(&this);
    }
  }
};

} // namespace fpat
#endif  // FORMAL_PATCH_RESULT_H
