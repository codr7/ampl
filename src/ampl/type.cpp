#include "ampl/sym.hpp"
#include "ampl/type.hpp"

namespace ampl {
  Type::Id Type::next_id = 0;

  Type::Type(const Sym &name): id(next_id++), name(name) {
      methods.is_true = [](auto &val) { return true; };
  }

  bool operator==(const Type &lhs, const Type &rhs) { return lhs.id == rhs.id; }
}
