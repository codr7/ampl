#ifndef AMPL_FORMS_REF_HPP
#define AMPL_FORMS_REF_HPP

#include "ampl/form.hpp"
#include "ampl/sym.hpp"

namespace ampl::forms {
  using namespace ampl;
  
  struct Ref {
    Ref(const Sym &name);

    Sym name;
  };

  template <>
  void dump(const Form &form, const Ref &ref, ostream &out);
  
  template <>
  void emit(const Form &form, const Ref &ref, deque<Form> &in, VM &vm);
}

#endif
