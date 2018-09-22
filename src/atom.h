#include <utility>

//
// Created by ech on 9/21/18.
//

#ifndef FORMAL_PATCH_ATOM_H
#define FORMAL_PATCH_ATOM_H

#include "llvm/ADT/MapVector.h"
#include "llvm/Support/StringPool.h"
#include "eigen3/Eigen/Dense"
#include <map>
#include <variant>
#include <optional>
#include "result.h"

namespace fpat {

using Number = float;
using String = llvm::PooledStringPtr;
using Vector = std::shared_ptr<Eigen::Matrix<Number, Eigen::Dynamic, 1>>;
using Matrix = std::shared_ptr<Eigen::Matrix<Number, Eigen::Dynamic, Eigen::Dynamic>>;
using BPF    = std::shared_ptr<Eigen::Matrix<Number, Eigen::Dynamic, 2>>;

// TODO unit test to check that every other atom type is included into this type signature
//using Dict   = llvm::MapVector<String, std::variant<Number, String, Vector, Matrix, BPF>>;
using Dict   = std::map<String, std::variant<Number, String, Vector, Matrix, BPF>>;

using Atom = std::variant<Number, String, Vector, Matrix, BPF, Dict>;
using AtomArray = std::vector<Atom>;

template<typename T>
constexpr bool is_atom_type =
    // TODO unit test that checks that all actual Atom types are included in this check
    std::is_same_v<T, Number>
        || std::is_same_v<T, String>
        || std::is_same_v<T, Vector>
        || std::is_same_v<T, Matrix>
        || std::is_same_v<T, BPF>;

template<typename T, typename = std::enable_if<is_atom_type<T>>>
bool is_a(const Atom &x) {
  return std::holds_alternative<T>(x);
}

constexpr auto is_number = is_a<Number>;
constexpr auto is_string = is_a<String>;
constexpr auto is_vector = is_a<Vector>;
constexpr auto is_matrix = is_a<Matrix>;
constexpr auto is_bpf = is_a<BPF>;
constexpr auto is_dict = is_a<Dict>;

template<typename T>
Result<T> as(const Atom x) {
  if (auto v = std::get_if<T>(&x)) {
    return *v;
  } else {
    return Error{101, "unknown error"};
  }
}

constexpr auto as_number = as<Number>;
constexpr auto as_string = as<String>;
constexpr auto as_vector = as<Vector>;
constexpr auto as_matrix = as<Matrix>;
constexpr auto as_bpf = as<BPF>;
constexpr auto as_dict = as<Dict>;

} // namespace fpat

#endif  // FORMAL_PATCH_ATOM_H
