#include "ampl/sym.hpp"

namespace ampl {
  Sym::Id Sym::next_id = 0;

  Sym::Sym(const string_view &name): id(next_id++), name(name) {}

  bool operator==(const Sym &lhs, const Sym &rhs) { return lhs.id == rhs.id; }

  bool operator<(const Sym &lhs, const Sym &rhs) { return lhs.id < rhs.id; }

  ostream &operator <<(ostream &out, const Sym &val) {
    out << val.name;
    return out;
  }
}
