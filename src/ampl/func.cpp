#include <iostream>

#include "ampl/stack.hpp"
#include "ampl/func.hpp"
#include "ampl/vm.hpp"

namespace ampl {
  Func::Id Func::next_id = 0;

  Func::Arg::Arg(const optional<Sym> &name, const Type &type): name(name), type(type) {}

  Func::Imp::Imp(const Sym &name, const vector<Arg> &args, const vector<Type> &rets, const Body &body):
    id(Func::next_id++), name(name), args(args), rets(rets), body(body) {}

  Func::Imp::Imp(const Func &func,
		 const Sym &name,
		 const vector<Arg> &args,
		 const vector<Type> &rets,
		 PC start_pc,
		 Reg min_reg):
    id(Func::next_id++), name(name), args(args), rets(rets), min_reg(min_reg),
    body([start_pc](const Func &self, const Pos &pos, PC ret_pc, VM &vm) {
      vm.push_frame(self, ret_pc);
      return start_pc;
    }) {} 

  Func::Func(const Sym &name, const vector<Arg> &args, const vector<Type> &rets, const Body &body):
    imp(make_shared<const Imp>(name, args, rets, body)) {}

  Func::Func(const Sym &name,
	     const vector<Arg> &args,
	     const vector<Type> &rets,
	     PC start_pc,
	     Reg min_reg):
    imp(make_shared<const Imp>(*this, name, args, rets, start_pc, min_reg)) {}
  
  bool Func::is_applicable(VM &vm) const {
    const Stack &s(vm.stack());
    if (s.empty()) { return false; }
    const Val *sv = &s.back(), *min_sv = &s.front();
    const Arg *min_av = &imp->args.front();
    
    for (const Arg *av = &imp->args.back(); av >= min_av; av--, sv--) {
      if (sv < min_sv || !sv->type.isa(av->type)) { return false; }
    }
    
    return true;
  }

  bool operator==(const Func &lhs, const Func &rhs) { return lhs.imp->id == rhs.imp->id; }

  ostream &operator <<(ostream &out, const Func &func) {
    out << "Func(" << func.imp->name;
    for (const Func::Arg &a: func.imp->args) { out << ' ' << a.type; }
    out << ')';
    
    return out;
  }
}
