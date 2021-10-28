#include <iostream>

#include "ampl/forms/ref.hpp"
#include "ampl/forms/lit.hpp"
#include "ampl/vm.hpp"

namespace ampl::forms {
  Ref::Ref(const Sym &name): name(name) {}

  template <>
  void dump(const Form &form, const Ref &ref, ostream &out) { out << ref.name; }
  
  template <>
  void emit(const Form &form, const Ref &ref, deque<Form> &in, VM &vm) {
    const Sym &id = ref.name;    
    auto found = vm.find(id);
    if (!found) { throw EmitError(form.pos, "Unknown ref: ", id); }
      
    if (found->type == vm.libs.abc.reg_type) {
      vm.emit<ops::Load>(form, found->as<Reg>());
    } else {
      vm.emit<ops::Push>(form, *found);
    }
  }

  template <>
  Val quote(const Form &form, const Ref &ref, VM &vm) { return Val(vm.libs.abc.form_type, form); }

  template <>
  optional<Val> val(const Form &form, const Ref &ref, VM &vm) {
    optional<Val> v = vm.find(ref.name);
    if (v && v->type == vm.libs.abc.reg_type) { return nullopt; }
    return v;
  }
}
