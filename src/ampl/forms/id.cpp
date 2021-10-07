#include "ampl/forms/id.hpp"
#include "ampl/vm.hpp"

namespace ampl::forms {
  Id::Id(const Sym &name): name(name) {}

  template <>
  void dump(const Form &form, const Id &id, ostream &out) { out << id.name; }
  
  template <>
  void emit(const Form &form, const Id &id, deque<Form> &in, VM &vm) {
    const string &ids = id.name.name;
    
    if (ids.find_first_not_of('d') == string::npos) {
      vm.emit<ops::Drop>(form, ids.length());
    } else {
      auto found = vm.scope.find(id.name);
      
      if (found) {
	if (found->type == vm.libs.abc.func_type) {
	  const Func &f = found->as<Func>();

	  for (const Func::Arg &a: f.args) {
	    Form af = in.front();
	    optional<Val> v = af.val(vm);

	    if (v && !v->type.isa(a.type)) {
	      throw EmitError(af.pos, "Not applicable: ", a.type, ' ', v->type);
	    }
	    
	    in.pop_front();
	    af.emit(in, vm);
	  }
	  
	  vm.emit<ops::Call>(form, f);
	} else if (found->type == vm.libs.abc.macro_type) {
	  found->as<Macro>().expand(form, in, vm);
	} else {
	  vm.emit<ops::Push>(form, *found);
	}
      } else {
	throw EmitError(form.pos, "Unknown id: ", ids);
      }
    }
  }

  template <>
  optional<Val> val(const Form &form, const Id &id, VM &vm) { return vm.scope.find(id.name); }
}
