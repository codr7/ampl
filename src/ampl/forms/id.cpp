#include "ampl/forms/id.hpp"
#include "ampl/vm.hpp"

namespace ampl::forms {
  Id::Id(const Sym &name): name(name) {}

  template <>
  void dump(const Form &form, const Id &id, ostream &out) { out << id.name; }
  
  template <>
  void emit(const Form &form, const Id &id, deque<Form> &in, VM &vm) {
    auto found = vm.scope.find(id.name);
    
    if (found) {
      if (found->type == vm.libs.abc.macro_type) {
	found->as<Macro>().expand(form, in, vm);
      } else {
	vm.emit<ops::Push>(form, *found);
      }
    } else {
      throw runtime_error("Not implemented");
    }
  }

  template <>
  optional<Val> val(const Form &form, const Id &id, VM &vm) { return vm.scope.find(id.name); }
}
