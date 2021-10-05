#include "ampl/forms/id.hpp"

namespace ampl::forms {
  Id::Id(const Sym &name): name(name) {}

  template <>
  void dump(const Form &form, const Id &id, ostream &out) { out << id.name; }
  
  template <>
  void emit(const Form &form, const Id &id, deque<Form> &in, VM &vm) {

  }
}
