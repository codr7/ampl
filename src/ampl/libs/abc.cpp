#include "ampl/libs/abc.hpp"
#include "ampl/type.hpp"
#include "ampl/vm.hpp"

#include <ostream>

namespace ampl::libs {
  ABC::ABC(VM &vm): Lib(vm),
    any_type(vm.sym("Any")),
    bool_type(vm.sym("Bool"), {any_type}),
    func_type(vm.sym("Func"), {any_type}),
    int_type(vm.sym("Int"), {any_type}),
    macro_type(vm.sym("Macro"), {any_type}),
    stack_type(vm.sym("Stack"), {any_type}) {
    bool_type.methods.dump = [](auto &val, auto &out) { out << (val.template as<bool>() ? 'T' : 'F'); };
    bool_type.methods.is_true = [](auto &val) { return val.template as<bool>(); };
    int_type.methods.is_true = [](auto &val) { return val.template as<int>(); };
    stack_type.methods.is_true = [](auto &val) { return !val.template as<Stack>().empty(); };

    bind(vm.sym("T"), bool_type, true);
    bind(vm.sym("F"), bool_type, false);

    bind(vm.sym("="), macro_type, Macro(vm.sym("="), 2,
					[](Macro &self, const Form &form, deque<Form> &in, VM &vm) {
					  Form x = in.front();
					  in.pop_front();
					  optional<Val> xv = x.val(vm);
					  if (!xv) { x.emit(in, vm); }
					  
					  Form y = in.front();
					  in.pop_front();
					  optional<Val> yv = y.val(vm);
					  if (!yv) { y.emit(in, vm); }
					  
					  vm.emit<ops::Equal>(form, xv, yv);
					}));

    bind(vm.sym("cp"), macro_type, Macro(vm.sym("cp"), 0,
					 [](Macro &self, const Form &form, deque<Form> &in, VM &vm) {
					   vm.emit<ops::Copy>(form);
					 }));
  }
}
