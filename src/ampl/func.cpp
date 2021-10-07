#include "ampl/func.hpp"

namespace ampl {
  Func::Id Func::next_id = 0;

  Func::Func(const Sym &name, const vector<Arg> &args, const vector<Type> &rets, const Body &body):
    id(next_id++), name(name), args(args), rets(rets), body(body) {}

  PC Func::call(const Pos &pos, PC ret, VM &vm) { return body(*this, pos, ret, vm); }

  bool operator==(const Func &lhs, const Func &rhs) { return lhs.id == rhs.id; }

  ostream &operator <<(ostream &out, const Func &func) {
    out << "Func(" << func.name;
    for (const Func::Arg &a: func.args) { out << ' ' << a.type; }
    out << ')';
    
    return out;
  }
}
