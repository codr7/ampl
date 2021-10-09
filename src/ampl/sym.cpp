#include "ampl/sym.hpp"

namespace ampl {
  Sym::Id Sym::next_id = 0;

  Sym::Imp::Imp(const string_view &name): id(Sym::next_id++), name(name) {}
  
  Sym::Sym(const string_view &name): imp(make_shared<const Imp>(name)) {}

  bool operator==(const Sym &lhs, const Sym &rhs) { return lhs.imp->id == rhs.imp->id; }

  bool operator<(const Sym &lhs, const Sym &rhs) { return lhs.imp->id < rhs.imp->id; }

  ostream &operator <<(ostream &out, const Sym &val) {
    out << val.imp->name;
    return out;
  }

  template <>
  Order compare(const Sym &x, const Sym &y) {
    int res = x.imp->name.compare(y.imp->name);
    if (res < 0) { return LT; }
    return (res == 0) ? EQ : GT;
  }
}
