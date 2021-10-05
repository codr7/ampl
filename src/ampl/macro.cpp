#include "ampl/macro.hpp"

namespace ampl {
  Macro::Id Macro::next_id = 0;

  Macro::Macro(const Sym &name, uint8_t arg_count, const Body &body):
    id(next_id++), name(name), arg_count(arg_count), body(body) {}

  void Macro::expand(const Form &form, deque<Form> &in, VM &vm) { body(*this, form, in, vm); }

  bool operator==(const Macro &lhs, const Macro &rhs) { return lhs.id == rhs.id; }

  ostream &operator <<(ostream &out, const Macro &macro) {
    out << "Macro(" << macro.name << ' ' << macro.arg_count << ')';
    return out;
  }
}
