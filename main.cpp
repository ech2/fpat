#include <iostream>

#include <variant>
#include "src/atom.h"
#include "llvm/Support/StringPool.h"

using namespace fpat;

int main() {
  llvm::StringPool p;
  AtomArray aa{Number(1), String(p.intern("lol")), Number(2)};

  for (auto &a : aa) {
    std::cout << is_number(a) << std::endl;
  }

  auto num = Result<int>(101);
  auto f = [](int x) { return std::to_string(x) + " lol"; };

  auto t = num.fmap<std::string>(f);
  std::cout << t.get_value_or(":(") << std::endl;

  return 0;
}