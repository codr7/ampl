#include "ampl/forms/id.hpp"

namespace ampl::forms {
  void IdType::dump(const Form &form, ostream &out) const { out << form.as<Sym>().name; }
  
  void IdType::emit(const Form &form, VM &vm) const {

  }

  IdType &id_type() {
    static IdType t;
    return t;
  }

  Form make_id(Pos pos, Sym name) { return Form(pos, id_type(), name); }
}
