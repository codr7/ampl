#include "ampl/form.hpp"

namespace ampl::forms {
  const Form &stop() {
    struct FormType: Form::TType<nullptr_t> {
      void dump(const Form &form, ostream &out) const override { out << "STOP"; }
      void emit(const Form &form, VM &vm) const override {}
    };

    static FormType form_type;
    static Form form(Pos::INVALID, form_type, nullptr);
    return form;
  }
}
