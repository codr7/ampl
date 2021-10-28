#include "ampl/forms/nop.hpp"
#include "ampl/vm.hpp"

namespace ampl::forms {
  template <>
  void dump(const Form &form, const Nop &nop, ostream &out) { out << '_'; }
  
  template <>
  void emit(const Form &form, const Nop &nop, deque<Form> &in, VM &vm) {}

  template <>
  Val quote(const Form &form, const Nop &nop, VM &vm) { return Val(vm.libs.abc.sym_type, vm.sym("_")); }
}
