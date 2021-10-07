#include <iostream>

#include "ampl/stack.hpp"
#include "ampl/func.hpp"
#include "ampl/vm.hpp"

namespace ampl {
  Func::Id Func::next_id = 0;

  Func::Func(const Sym &name, const vector<Arg> &args, const vector<Type> &rets, const Body &body):
    id(next_id++), name(name), args(args), rets(rets), body(body) {}

  bool Func::is_applicable(VM &vm) {
    Stack &s(vm.stack());
    assert(!s.empty());
    Val *sv = &s.back();
    
    for (Arg *av = &args.back(); av >= &args.front(); av--, sv--) {
      assert(sv >= &s.front());
      if (!sv->type.isa(av->type)) { return false; }
    }

    return true;
  }

  PC Func::eval(const Pos &pos, PC ret_pc, VM &vm) { return body(*this, pos, ret_pc, vm); }

  bool operator==(const Func &lhs, const Func &rhs) { return lhs.id == rhs.id; }

  ostream &operator <<(ostream &out, const Func &func) {
    out << "Func(" << func.name;
    for (const Func::Arg &a: func.args) { out << ' ' << a.type; }
    out << ')';
    
    return out;
  }
}
