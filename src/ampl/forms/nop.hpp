#ifndef AMPL_FORMS_NOP_HPP
#define AMPL_FORMS_NOP_HPP

#include "ampl/form.hpp"
#include "ampl/val.hpp"

namespace ampl::forms {
  using namespace ampl;
  
  struct Nop {};

  template <>
  void dump(const Form &form, const Nop &nop, ostream &out);
  
  template <>
  void emit(const Form &form, const Nop &nop, deque<Form> &in, VM &vm);
}

#endif
