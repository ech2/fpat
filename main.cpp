#include <iostream>

#include <variant>
#include "src/atom.h"
#include "llvm/Support/StringPool.h"

using namespace fpat;

int main() {
  llvm::StringPool p;
//  AtomVector aa{Num{1}, Str{p.intern("lol")}, Num{2}};
//
//  for (auto &a : aa) {
//    std::cout << is_number(a) << std::endl;
//  }
//
//  auto num = Result<int>(101);
//  auto f = [](int x) { return std::to_string(x) + " lol"; };
//
//  auto t = num.fmap<std::string>(f);
//  std::cout << t.get_value_or(":(") << std::endl;

//  AtomVec avec{Num{1}, Str{p.intern("lol")}, Num{2}};
//  std::cout << match<Num, Str>(avec) << std::endl;
//  std::cout << match_exact<Num, Str>(avec) << std::endl;

//  std::cout << atom_type_str<Num> << std::endl;
//  std::cout << str_str << std::endl;
//  std::cout << str_str << std::endl;

//  auto v = Vec();
//  std::cout << *vec_str(v, p) << std::endl;
//  std::cout << *num_str(1.0f, p) << std::endl;


  return 0;
}