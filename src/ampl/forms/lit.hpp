#ifndef AMPL_FORMS_LIT_HPP
#define AMPL_FORMS_LIT_HPP

#include "ampl/form.hpp"
#include "ampl/val.hpp"

namespace ampl::forms {
  using namespace ampl;
  
  struct Lit {
    Lit(const Sym &name);

    Val val;
  };

  template <>
  volit dump(const Form &form, const Lit &lit, ostream &out);
  
  template <>
  volit emit(const Form &form, const Lit &lit, deque<Form> &in, VM &vm);

  template <>
  optional<Val> val(const Form &form, const Lit &lit, VM &vm);
}

#endif
