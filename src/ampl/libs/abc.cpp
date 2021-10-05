#include "ampl/libs/abc.hpp"
#include "ampl/type.hpp"
#include "ampl/vm.hpp"

#include <ostream>

namespace ampl::libs {
  ABC::ABC(VM &vm):
    bool_type(vm.sym("Bool")),
    int_type(vm.sym("Int")),
    macro_type(vm.sym("Macro")),
    stack_type(vm.sym("Stack")) {
    bool_type.methods.dump = [](auto &val, auto &out) { out << (val.template as<bool>() ? 'T' : 'F'); };
    bool_type.methods.is_true = [](auto &val) { return val.template as<bool>(); };
    int_type.methods.is_true = [](auto &val) { return val.template as<int>(); };
    stack_type.methods.is_true = [](auto &val) { return !val.template as<Stack>().empty(); };

    bind(vm.sym("T"), bool_type, true);
    bind(vm.sym("F"), bool_type, false);
    
    bind(vm.sym("cp"), macro_type, Macro(vm.sym("cp"), 0,
					 [](Macro &self, const Form &form, deque<Form> &in, VM &vm) {
					   vm.emit<ops::Copy>(form);
					 }));
  }
}
