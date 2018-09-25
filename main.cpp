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

//  auto v1 = std::make_shared<Eigen::Matrix<Num, Eigen::Dynamic, 1>>(100);
//  auto v1_size = v1->size();
//  auto v2 = make_atom_ptr_type<Vec>(100);
//  auto v2_size = v2->size();
//  auto v3 = make_vec(100);
//  auto v3_size = v3->size();

//FIXME: adresses returned by atom_str_dyn should be equal to Vec’s inner matrix addresses
//  auto t1 = make_vec(100);
//  auto t2 = make_vec(101);
//  auto t1_ps = atom_str_dyn(t1, p);
//  std::cout << *t1_ps << std::endl;
//  auto t2_ps = atom_str_dyn(t2, p);
//  std::cout << *t1_ps << std::endl;  // t1_ps is different here!
//  std::cout << *t2_ps << std::endl;

  return 0;
}