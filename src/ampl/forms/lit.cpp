#include "ampl/forms/lit.hpp"

namespace ampl::forms {
  Lit::Lit(const Sym &name): name(name) {}

  template <>
  void dump(const Form &form, const Lit &lit, ostream &out) { out << lit.val; }
  
  template <>
  void emit(const Form &form, const Lit &lit, deque<Form> &in, VM &vm) {
    vm.emit<ops::Push>(form, lit.val);
  }

  template <>
  optional<Val> val(const Form &form, const Lit &lit, VM &vm) { return lit.val; }
}
