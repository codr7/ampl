#include <iostream>

#include "ampl/stack.hpp"
#include "ampl/func.hpp"
#include "ampl/vm.hpp"

namespace ampl {
  Func::Id Func::next_id = 0;

  Func::Arg::Arg(const optional<Sym> &name, const Type &type): name(name), type(type) {}

  Func::Func(const Sym &name, const vector<Arg> &args, const vector<Type> &rets, const Body &body):
    id(next_id++), name(name), args(args), rets(rets), body(body) {}

  void Func::emit(const Form &body_form, deque<Form> &in, VM &vm) {
    PC skip_pc = vm.pc();
    vm.emit<ops::Goto>(body_form);
    PC start_pc = vm.pc();

    body = [start_pc](const Func &self, const Pos &pos, PC ret_pc, VM &vm) {
      vm.push_frame(self, ret_pc);
      return start_pc;
    };

    Scope &scope = vm.push_scope();
    min_reg = max_reg = scope.reg_count;

    if (!args.empty()) {
      int offset = 0;
      
      for (Arg *a = &args.back(); a >= &args.front(); a--) {
	if (a->name) {
	  vm.emit<ops::Store>(body_form, (max_reg = scope.bind(*a->name)), offset);
	} else {
	  offset++;
	}
      }
    }

    body_form.emit(in, vm);
    vm.pop_scope();
    vm.emit<ops::Ret>(body_form);
    vm.ops[skip_pc].as<ops::Goto>().pc = vm.pc();
  }

  bool Func::is_applicable(VM &vm) const {
    if (!args.empty()) {
      Stack &s(vm.stack());
      if (s.empty()) { return false; }
      Val *sv = &s.back();
      
      for (const Arg *av = &args.back(); av >= &args.front(); av--, sv--) {
	if (sv < &s.front() || !sv->type.isa(av->type)) { return false; }
      }
    }
    
    return true;
  }

  PC Func::eval(const Pos &pos, PC ret_pc, VM &vm) const { return body(*this, pos, ret_pc, vm); }

  bool operator==(const Func &lhs, const Func &rhs) { return lhs.id == rhs.id; }

  ostream &operator <<(ostream &out, const Func &func) {
    out << "Func(" << func.name;
    for (const Func::Arg &a: func.args) { out << ' ' << a.type; }
    out << ')';
    
    return out;
  }
}
