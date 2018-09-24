//
// Created by ech on 9/21/18.
//

#ifndef FORMAL_PATCH_ATOM_H
#define FORMAL_PATCH_ATOM_H

#include <algorithm>
#include <array>
#include <map>
#include <optional>
#include <utility>
#include <variant>

#include "eigen3/Eigen/Dense"
#include "llvm/Support/StringPool.h"

#include "result.h"

namespace fpat {

////
//// atom types
////

using Num = float;
using Str = llvm::PooledStringPtr;
using Vec = std::shared_ptr<Eigen::Matrix<Num, Eigen::Dynamic, 1>>;
using Mat = std::shared_ptr<Eigen::Matrix<Num, Eigen::Dynamic, Eigen::Dynamic>>;
using Bpf = std::shared_ptr<Eigen::Matrix<Num, Eigen::Dynamic, 2>>;

// TODO unit test to check that every other atom type is included into this type signature
using Dic = std::map<Str, std::variant<Num, Str, Vec, Mat, Bpf>>;

using Atom = std::variant<Num, Str, Vec, Mat, Bpf, Dic>;
using AtomVec = std::vector<Atom>;

////
//// atom type operations
////

template<typename T>
constexpr bool is_atom_type =
    // TODO unit test that checks that all actual Atom types are included in this check
    std::is_same_v<T, Num>
        || std::is_same_v<T, Str>
        || std::is_same_v<T, Vec>
        || std::is_same_v<T, Mat>
        || std::is_same_v<T, Bpf>;

template<typename T, typename = std::enable_if<is_atom_type<T>>>
bool is_a(const Atom &x) {
  return std::holds_alternative<T>(x);
}

constexpr auto is_number = is_a<Num>;
constexpr auto is_string = is_a<Str>;
constexpr auto is_vector = is_a<Vec>;
constexpr auto is_matrix = is_a<Mat>;
constexpr auto is_bpf = is_a<Bpf>;
constexpr auto is_dict = is_a<Dic>;

template<typename T>
Result<T> as(const Atom x) {
  if (auto v = std::get_if<T>(&x)) {
    return *v;
  } else {
    return errors::cant_convert_atom();
  }
}

constexpr auto as_number = as<Num>;
constexpr auto as_string = as<Str>;
constexpr auto as_vector = as<Vec>;
constexpr auto as_matrix = as<Mat>;
constexpr auto as_bpf = as<Bpf>;
constexpr auto as_dict = as<Dic>;

////
//// match and match_exact
////

template<typename...Ts, size_t...Ns>
bool match_impl(std::index_sequence<Ns...>, const AtomVec &avec) {
  auto m = {is_a<Ts>(avec[Ns])...};
  return std::all_of(m.begin(), m.end(), [](bool x) { return x; });
}

template<typename...Ts>
bool match(const AtomVec &avec) {
  if (avec.size() < sizeof...(Ts)) {
    return false;
  }
  return match_impl<Ts...>(std::make_index_sequence<sizeof...(Ts)>(), avec);
}

template<typename...Ts>
bool match_exact(const AtomVec &avec) {
  if (avec.size() != sizeof...(Ts)) {
    return false;
  }
  match<Ts...>(avec);
}
}; // namespace fpat

#endif  // FORMAL_PATCH_ATOM_H
