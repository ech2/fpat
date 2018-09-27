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
#include "llvm/Support/FormatVariadic.h"

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
using Dic = std::shared_ptr<std::map<Str, std::variant<Num, Str, Vec, Mat, Bpf>>>;

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
bool is_a(const Atom &x) noexcept {
  return std::holds_alternative<T>(x);
}

constexpr auto is_num = is_a<Num>;
constexpr auto is_str = is_a<Str>;
constexpr auto is_vec = is_a<Vec>;
constexpr auto is_mat = is_a<Mat>;
constexpr auto is_bpf = is_a<Bpf>;
constexpr auto is_dic = is_a<Dic>;

template<typename T>
Result<T> as(const Atom x) noexcept {
  if (auto v = std::get_if<T>(&x)) {
    return *v;
  } else {
    return errors::cant_convert_atom();
  }
}

constexpr auto as_num = as<Num>;
constexpr auto as_str = as<Str>;
constexpr auto as_vec = as<Vec>;
constexpr auto as_mat = as<Mat>;
constexpr auto as_bpf = as<Bpf>;
constexpr auto as_dic = as<Dic>;

////
//// atom construction
////

Str make_str(const char *str, llvm::StringPool &pool) { return pool.intern(str); }

template<typename T, typename...Args>
T make_atom_ptr_type(Args &&... args) {
  using UnderlyingType = typename T::element_type;
  return std::make_shared<UnderlyingType>(args...);
}

template<typename...Args>
auto make_vec(Args &&...args) {
  return make_atom_ptr_type<Vec, Args...>(std::forward<Args...>(args)...);
}

template<typename...Args>
auto make_mat(Args &&...args) {
  return make_atom_ptr_type<Mat, Args...>(std::forward<Args...>(args)...);
}

template<typename...Args>
auto make_bpf(Args &&...args) {
  return make_atom_ptr_type<Bpf, Args...>(std::forward<Args...>(args)...);
}

template<typename...Args>
auto make_dic(Args &&...args) {
  return make_atom_ptr_type<Dic, Args...>(std::forward<Args...>(args)...);
}

////
//// atom string representation
////

template<typename T>
constexpr const char *atom_type_str = []() {
  if constexpr (std::is_same_v<T, Num>) {
    return "num";
  } else if constexpr (std::is_same_v<T, Str>) {
    return "str";
  } else if constexpr (std::is_same_v<T, Vec>) {
    return "vec";
  } else if constexpr (std::is_same_v<T, Mat>) {
    return "mat";
  } else if constexpr (std::is_same_v<T, Bpf>) {
    return "bpf";
  } else if constexpr (std::is_same_v<T, Dic>) {
    return "dic";
  }
}(); // <-- NB: function invocation

constexpr auto num_type_str = atom_type_str<Num>;
constexpr auto str_type_str = atom_type_str<Str>;
constexpr auto vec_type_str = atom_type_str<Vec>;
constexpr auto mat_type_str = atom_type_str<Mat>;
constexpr auto bpf_type_str = atom_type_str<Bpf>;
constexpr auto dic_type_str = atom_type_str<Dic>;

template<typename T>
Str atom_str(const T &atom_v, llvm::StringPool pool) {
  if constexpr (std::is_same_v<T, Num>) {
    return pool.intern(std::to_string(atom_v));
  } else if constexpr (std::is_same_v<T, Str>) {
    return atom_v;
  } else if constexpr (is_atom_type<T>) {
    // FIXME this gives incoherent addresses
    auto ptr = reinterpret_cast<intptr_t >(atom_v.get());
    auto type_str = atom_type_str<T>;
    auto s = llvm::formatv("{0}_{1}", type_str, ptr).str();
    return pool.intern(s);
  } else {
    __builtin_unreachable();
  }
}

constexpr auto num_str = atom_str<Num>;
constexpr auto str_str = atom_str<Str>;
constexpr auto vec_str = atom_str<Vec>;
constexpr auto mat_str = atom_str<Mat>;
constexpr auto bpf_str = atom_str<Bpf>;
constexpr auto dic_str = atom_str<Dic>;

Str atom_str_dyn(const Atom &atom, llvm::StringPool &pool) noexcept {
  return std::visit([&pool](auto &arg) -> Str {
    using T = std::decay_t<decltype(arg)>;
    return atom_str<T>(arg, pool);  // TODO test this const
  }, atom);
}

////
//// match and match_exact
////

template<typename...Ts, size_t...Ns>
bool match_impl(const AtomVec &avec, std::index_sequence<Ns...>) {
  auto m = {is_a<Ts>(avec[Ns])...};
  return std::all_of(m.begin(), m.end(), [](bool x) { return x; });
}

template<typename...Ts>
bool match(const AtomVec &avec) {
  if (avec.size() < sizeof...(Ts)) {
    return false;
  }
  return match_impl<Ts...>(avec, std::make_index_sequence<sizeof...(Ts)>());
}

template<typename...Ts>
bool match_exact(const AtomVec &avec) {
  if (avec.size() != sizeof...(Ts)) {
    return false;
  }
  return match_impl<Ts...>(avec, std::make_index_sequence<sizeof...(Ts)>());
}

////
//// as_tuple and as_tuple_exact
////

template<typename...Ts, size_t...Ns>
Result<std::tuple<Ts...>> as_tuple_impl(const AtomVec &avec, std::index_sequence<Ns...>) {
  auto check = {is_a<Ts>(avec[Ns])...};

  if (std::all_of(check.begin(), check.end(), [](bool x) -> bool { return x; })) {
    return std::make_tuple(std::get<Ts>(avec[Ns])...);
  } else {
    return errors::as_tuple_cant_find_all_types();
  }
};

template<typename...Ts>
Result<std::tuple<Ts...>> as_tuple(const AtomVec &avec) {
  if (avec.size() < sizeof...(Ts)) {
    return errors::as_tuple_avec_is_too_short();
  }
  return as_tuple_impl<Ts...>(avec, std::make_index_sequence<sizeof...(Ts)>());
}

template<typename...Ts>
Result<std::tuple<Ts...>> as_tuple_exact(const AtomVec &avec) {
  if (avec.size() != sizeof...(Ts)) {
    return errors::as_tuple_avec_is_not_same_length();
  }
  return as_tuple_impl<Ts...>(avec, std::make_index_sequence<sizeof...(Ts)>());
}

}; // namespace fpat

#endif  // FORMAL_PATCH_ATOM_H
