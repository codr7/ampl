#ifndef AMPL_FORMS_ID_HPP
#define AMPL_FORMS_ID_HPP

#include "ampl/form.hpp"
#include "ampl/sym.hpp"

namespace ampl::forms {
  using namespace ampl;
  
  struct IdType: Form::TType<Sym> {
    void dump(const Form &form, ostream &out) const override;
    void emit(const Form &form, VM &vm) const override;
  };

  IdType &id_type();
  Form make_id(Pos pos, Sym name);
}

#endif
