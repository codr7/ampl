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
		    form_type(vm.sym("Form"), {any_type}),
		    func_type(vm.sym("Func"), {any_type}),
		    int_type(vm.sym("Int"), {any_type}),
		    macro_type(vm.sym("Macro"), {any_type}),
		    meta_type(vm.sym("Meta"), {any_type}),
		    reg_type(vm.sym("Reg"), {any_type}),
		    stack_type(vm.sym("Stack"), {any_type}),
		    sym_type(vm.sym("Sym"), {any_type}) {
    bool_type.methods.dump = [](auto &val, auto &out) { out << (val.template as<bool>() ? 'T' : 'F'); };

    bool_type.methods.compare = [](auto &x, auto &y) {
      bool xv = x.template as<bool>(), yv = y.template as<bool>();
      if (!xv && yv) { return LT; }
      if (xv && !yv) { return GT; }
      return EQ;
    };

    bool_type.methods.is_equal = [](auto &x, auto &y) {
      bool xv = x.template as<bool>(), yv = y.template as<bool>();
      return xv == yv;
    };
      
    bool_type.methods.is_true = [](auto &val) { return val.template as<bool>(); };

    form_type.methods.dump = [](auto &val, auto &out) { val.template as<Form>().dump(out); };
    form_type.methods.is_true = [](auto &val) { return true; };

    int_type.methods.dump = [](auto &val, auto &out) { out << val.template as<int>(); };

    int_type.methods.compare = [](auto &x, auto &y) {
      int xv = x.template as<int>(), yv = y.template as<int>();
      if (xv < yv) { return LT; }
      return (xv == yv) ? EQ : GT;
    };

    int_type.methods.is_equal = [](auto &x, auto &y) {
      int xv = x.template as<int>(), yv = y.template as<int>();
      return xv == yv;
    };

    int_type.methods.is_true = [](auto &val) { return val.template as<int>(); };
    stack_type.methods.dump = [](auto &val, auto &out) { out << val.template as<Stack>(); };

    stack_type.methods.compare = [](auto &x, auto &y) {
      const Stack &xs = x.template as<Stack>(), &ys = y.template as<Stack>();
      for (auto xi = xs.begin(), yi = ys.begin();; xi++, yi++) {
	if (xi == xs.end() && yi != ys.end()) { return LT; }
	if (xi != xs.end() && yi == ys.end()) { return GT; }
	if (xi == xs.end() && yi == ys.end()) { break; }

	Order result = xi->compare(*yi);
	if (result != EQ) { return result; }
      }

      return EQ;
    };

    stack_type.methods.is_equal = [](auto &x, auto &y) {
      const Stack &xs = x.template as<Stack>(), &ys = y.template as<Stack>();
      if (xs.size() != ys.size()) { return false; }

      for (auto xi = xs.begin(), yi = ys.begin(); xi != xs.end(); xi++, yi++) {
	if (*xi != *yi) { return false; }
      }

      return true;
    };

    stack_type.methods.is_true = [](auto &val) { return !val.template as<Stack>().empty(); };
    sym_type.methods.dump = [](auto &val, auto &out) { out << '\'' << val.template as<Sym>().name; };

    sym_type.methods.compare = [](auto &x, auto &y) {
      const Sym &xv = x.template as<Sym>(), &yv = y.template as<Sym>();
      return compare(xv, yv);
    };

    sym_type.methods.is_equal = [](auto &x, auto &y) {
      Sym xv = x.template as<Sym>(), yv = y.template as<Sym>();
      return xv == yv;
    };

    sym_type.methods.is_true = [](auto &val) { return true; };

    bind(any_type);
    bind(bool_type);
    bind(form_type);
    bind(func_type);
    bind(int_type);
    bind(macro_type);
    bind(reg_type);
    bind(stack_type);
    bind(sym_type);
    
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
		 
		 for (auto i = args_body.begin(); i != args_body.end(); i++) {
		   Form name = *i++, type_form = *i;
		   Val val(vm.get(type_form.as<forms::Id>().name));

		   if (val.type != vm.libs.abc.meta_type) {
		     throw EmitError(type_form.pos, "Invalid argument type: ", val.type);
		   }
		   
		   args.emplace_back(name.as<forms::Id>().name, val.as<Type>());
		 }
		 
		 Form rets_form = in.front();
		 in.pop_front();
		 vector<Type> rets;

		 for (const Form &rf: rets_form.as<forms::Group>().body) {
		   Val val(vm.get(rf.as<forms::Id>().name));

		   if (val.type != vm.libs.abc.meta_type) {
		     throw EmitError(rf.pos, "Invalid return type: ", val.type);
		   }

		   rets.emplace_back(val.as<Type>());
		 }
		 
		 Form body_form = in.front();
		 in.pop_front();
		 vm.scope().bind(name, vm.libs.abc.func_type, Func(name, args, rets));
		 vm.get(name).as<Func>().emit(body_form, in, vm);
	       });

    bind_macro(vm.sym("if"), 3,
	       [](const Macro &self, const Form &form, deque<Form> &in, VM &vm) {
		 Form cond(in.front());
		 in.pop_front();
		 cond.emit(in, vm);
		 PC branch_pc = vm.pc();
		 vm.emit<ops::Branch>(form);
		 Form true_branch(in.front());
		 in.pop_front();
		 true_branch.emit(in, vm);
		 PC skip_pc = vm.pc();
		 vm.emit<ops::Goto>(form);
		 vm.ops[branch_pc].as<ops::Branch>().false_pc = vm.pc();
		 Form false_branch(in.front());
		 in.pop_front();
		 false_branch.emit(in, vm);
		 vm.ops[skip_pc].as<ops::Goto>().pc = vm.pc();
	       }); 
  }
}
