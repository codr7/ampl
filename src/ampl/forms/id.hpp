#ifndef AMPL_FORMS_ID_HPP
#define AMPL_FORMS_ID_HPP

#include "ampl/form.hpp"
#include "ampl/sym.hpp"

namespace ampl::forms {
  using namespace ampl;
  
  struct Id {
    Id(const Sym &name);

    Sym name;
  };

  template <>
  void dump(const Form &form, const Id &id, ostream &out);
  
  template <>
  void emit(const Form &form, const Id &id, deque<Form> &in, VM &vm);
}

#endif
