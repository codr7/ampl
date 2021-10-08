#ifndef AMPL_FORMS_GROUP_HPP
#define AMPL_FORMS_GROUP_HPP

#include <deque>

#include "ampl/form.hpp"
#include "ampl/sym.hpp"

namespace ampl::forms {
  using namespace ampl;
  
  struct Group {
    Group(const deque<Form> &body);

    deque<Form> body;
  };

  template <>
  void dump(const Form &form, const Group &group, ostream &out);
  
  template <>
  void emit(const Form &form, const Group &group, deque<Form> &in, VM &vm);
}

#endif
