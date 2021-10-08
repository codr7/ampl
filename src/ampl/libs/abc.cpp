#include <iostream>

#include "ampl/forms/group.hpp"
#include "ampl/forms/id.hpp"
#include "ampl/libs/abc.hpp"
#include "ampl/type.hpp"
#include "ampl/vm.hpp"

namespace ampl::libs {
  ABC::ABC(VM &vm): Lib(vm),
    any_type(vm.sym("Any")),
    bool_type(vm.sym("Bool"), {any_type}),
    func_type(vm.sym("Func"), {any_type}),
    int_type(vm.sym("Int"), {any_type}),
    macro_type(vm.sym("Macro"), {any_type}),
    meta_type(vm.sym("Meta"), {any_type}),
    reg_type(vm.sym("Reg"), {any_type}),
    stack_type(vm.sym("Stack"), {any_type}) {
    bool_type.methods.dump = [](auto &val, auto &out) { out << (val.template as<bool>() ? 'T' : 'F'); };
    bool_type.methods.is_true = [](auto &val) { return val.template as<bool>(); };
    int_type.methods.is_true = [](auto &val) { return val.template as<int>(); };
    stack_type.methods.is_true = [](auto &val) { return !val.template as<Stack>().empty(); };

    bind(any_type);
    bind(bool_type);
    bind(func_type);
    bind(int_type);
    bind(macro_type);
    bind(reg_type);
    bind(stack_type);
    
    bind(vm.sym("T"), bool_type, true);
    bind(vm.sym("F"), bool_type, false);

    bind_macro(vm.sym("="), 2,
	       [](const Macro &self, const Form &form, deque<Form> &in, VM &vm) {
		 Form x = in.front();
		 in.pop_front();
		 optional<Val> xv = x.val(vm);
		 if (!xv) { x.emit(in, vm); }
		 
		 Form y = in.front();
		 in.pop_front();
		 optional<Val> yv = y.val(vm);
		 if (!yv) { y.emit(in, vm); }
		 
		 vm.emit<ops::Equal>(form, xv, yv);
	       });

    bind_macro(vm.sym("cp"), 0,
	       [](const Macro &self, const Form &form, deque<Form> &in, VM &vm) {
		 vm.emit<ops::Copy>(form);
	       });

    bind_func(vm.sym("dump"),
	      {{vm.sym("val"), vm.libs.abc.any_type}},
	      {},
	      [](const Func &self, const Pos &pos, PC ret_pc, VM &vm) {
		vm.pop().dump(cout);
		cout << endl;
		return ret_pc;
	      });

    bind_macro(vm.sym("func"), 4,
	       [](const Macro &self, const Form &form, deque<Form> &in, VM &vm) {
		 Form name_form = in.front();
		 in.pop_front();
		 const Sym &name = name_form.as<forms::Id>().name;
		 
		 Form args_form = in.front();
		 in.pop_front();
		 vector<Func::Arg> args;
		 const deque<Form> &args_body = args_form.as<forms::Group>().body;
		 Scope &scope = vm.scope();
		 
		 for (auto i = args_body.begin(); i != args_body.end(); i++) {
		   Form name = *i++, type_form = *i;
		   Val val(scope.get(type_form.as<forms::Id>().name));

		   if (val.type != vm.libs.abc.meta_type) {
		     throw EmitError(type_form.pos, "Invalid argument type: ", val.type);
		   }
		   
		   args.emplace_back(name.as<forms::Id>().name, val.as<Type>());
		 }
		 
		 Form rets_form = in.front();
		 in.pop_front();
		 vector<Type> rets;

		 for (const Form &rf: rets_form.as<forms::Group>().body) {
		   Val val(scope.get(rf.as<forms::Id>().name));

		   if (val.type != vm.libs.abc.meta_type) {
		     throw EmitError(rf.pos, "Invalid return type: ", val.type);
		   }

		   rets.emplace_back(val.as<Type>());
		 }
		 
		 Form body_form = in.front();
		 in.pop_front();
		 Func f(name, args, rets);
		 vm.scope().bind(name, vm.libs.abc.func_type, f);
		 vm.scope().get(name).as<Func>().emit(body_form, in, vm);
	       });
  }
}
