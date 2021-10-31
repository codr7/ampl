#include <iostream>

#include "ampl/forms/id.hpp"
#include "ampl/forms/lit.hpp"
#include "ampl/sym.hpp"
#include "ampl/vm.hpp"

namespace ampl::forms {
  Id::Id(const Sym &name): name(name) {}

  template <>
  void dump(const Form &form, const Id &id, ostream &out) { out << id.name; }
  
  template <>
  void emit(const Form &form, const Id &id, deque<Form> &in, VM &vm) {
    const string &ids = id.name.imp->name;
    
    if (ids.find_first_not_of('d') == string::npos) {
      vm.emit<ops::Drop>(form, ids.length());
    } else {
      auto found = vm.find(id.name);
      
      if (found) {
	if (found->type == vm.libs.abc.func_type) {
	  const Func &func = found->as<Func>();
	  CallFlags flags = CALL_CHECK;

	  while (!in.empty()) {
	    const Form &f = in.front();
	    if (!f.is<forms::Id>()) { break; }
	    string id = f.as<forms::Id>().name.imp->name;
	    if (id.front() != '|') { break; }
	    string flag = id.substr(1);
	    
	    if (flag == "d" || flag == "drop") {
	      flags = CallFlags(flags | CALL_DROP);
	    } else {
	      throw EmitError(form.pos, "Invalid call flag: ", flag);
	    }
	    
	    in.pop_front();
	  }

	  if (func == vm.libs.math.int_sub_func) {
	    Form x(move(in.front()));
	    in.pop_front();
	    x.emit(in, vm);

	    Form y(move(in.front()));
	    in.pop_front();
	    
	    if (y.is<forms::Lit>()) {
	      vm.emit<ops::Dec>(form, y.as<forms::Lit>().val.as<int>());
	    } else {
	      y.emit(in, vm);
	      vm.emit<ops::Call>(form, func, flags);
	    }
	  } else {	    
	    for (const Func::Arg &a [[maybe_unused]]: func.imp->args) {
	      if (in.empty()) {
		throw EmitError(form.pos, "Missing arg: ", func, ' ', a.name ? a.name->imp->name : "n/a");
	      }
	      
	      Form af = pop_front(in);
	      optional<Val> v = af.val(vm);
	      if (v && !v->type.isa(a.type)) { throw EmitError(form.pos, "Wrong arg type: ", v->type, ' ', a.type); }
	      af.emit(in, vm);
	    }

	    vm.emit<ops::Call>(form, func, flags);
	  }
	} else if (found->type == vm.libs.abc.macro_type) {
	  found->as<Macro>().expand(form, in, vm);
	} else if (found->type == vm.libs.abc.reg_type) {
	  vm.emit<ops::Load>(form, found->as<Reg>());
	} else {
	  vm.emit<ops::Push>(form, *found);
	}
      } else {
	throw EmitError(form.pos, "Unknown id: ", ids);
      }
    }
  }

  template <>
  Val quote(const Form &form, const Id &id, VM &vm) { return Val(vm.libs.abc.sym_type, id.name); }

  template <>
  optional<Val> val(const Form &form, const Id &id, VM &vm) {
    optional<Val> v = vm.find(id.name);
    if (v && v->type == vm.libs.abc.reg_type) { return nullopt; }
    return v;
  }
}
