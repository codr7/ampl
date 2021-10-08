#include "ampl/forms/group.hpp"
#include "ampl/vm.hpp"

namespace ampl::forms {
  Group::Group(const deque<Form> &body): body(body) {}

  template <>
  void dump(const Form &form, const Group &group, ostream &out) {
    out << '(';

    for (auto i = group.body.begin(); i != group.body.end(); i++) {
      if (i != group.body.begin()) { out << ' '; }
      i->dump(out);
    }
    
    out << ')';
  }
  
  template <>
  void emit(const Form &form, const Group &group, deque<Form> &in, VM &vm) {
    vm.emit(group.body);
  }

  template <>
  Val quote(const Form &form, const Group &group, VM &vm) { return Val(vm.libs.abc.form_type, form); }
}
