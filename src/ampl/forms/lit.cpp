#include "ampl/forms/lit.hpp"
#include "ampl/vm.hpp"

namespace ampl::forms {
  Lit::Lit(const Val &val): val(val) {}

  template <>
  void dump(const Form &form, const Lit &lit, ostream &out) { out << lit.val; }
  
  template <>
  void emit(const Form &form, const Lit &lit, deque<Form> &in, VM &vm) {
    vm.emit<ops::Push>(form, lit.val);
  }

  template <>
  Val quote(const Form &form, const Lit &lit, VM &vm) { return lit.val; }

  template <>
  optional<Val> val(const Form &form, const Lit &lit, VM &vm) { return lit.val; }
}
