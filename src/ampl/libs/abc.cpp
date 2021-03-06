#include <iostream>

#include "ampl/forms/group.hpp"
#include "ampl/forms/id.hpp"
#include "ampl/libs/abc.hpp"
#include "ampl/type.hpp"
#include "ampl/vm.hpp"

namespace ampl::libs {
  BoolType::Imp::Imp(const Sym &name, initializer_list<Type> parent_types): Type::Imp(name, parent_types) {
    methods.dump = [](auto &val, auto &out) { out << (val.template as<bool>() ? 'T' : 'F'); };
    
    methods.compare = [](auto &x, auto &y) {
      bool xv = x.template as<bool>(), yv = y.template as<bool>();
      if (!xv && yv) { return LT; }
      if (xv && !yv) { return GT; }
      return EQ;
    };
    
    methods.is_equal = [](auto &x, auto &y) {
      bool xv = x.template as<bool>(), yv = y.template as<bool>();
      return xv == yv;
    };
    
    methods.is_true = [](auto &val) { return val.template as<bool>(); };
  }

  BoolType::BoolType(const Sym &name, initializer_list<Type> parent_types):
    TType<bool>(make_shared<const Imp>(name, parent_types)) {}

  FormType::Imp::Imp(const Sym &name, initializer_list<Type> parent_types): Type::Imp(name, parent_types) {
    methods.dump = [](auto &val, auto &out) { val.template as<Form>().dump(out); };
  }

  FormType::FormType(const Sym &name, initializer_list<Type> parent_types):
    TType<Form>(make_shared<const Imp>(name, parent_types)) {}

  FuncType::Imp::Imp(const Sym &name, initializer_list<Type> parent_types): Type::Imp(name, parent_types) {
    methods.dump = [](auto &val, auto &out) { out << val.template as<Func>(); };
  }

  FuncType::FuncType(const Sym &name, initializer_list<Type> parent_types):
    TType<Func>(make_shared<const Imp>(name, parent_types)) {}

  IntType::Imp::Imp(const Sym &name, initializer_list<Type> parent_types): Type::Imp(name, parent_types) {
    methods.dump = [](auto &val, auto &out) { out << val.template as<int>(); };

    methods.compare = [](auto &x, auto &y) {
      int xv = x.template as<int>(), yv = y.template as<int>();
      if (xv < yv) { return LT; }
      return (xv == yv) ? EQ : GT;
    };

    methods.is_equal = [](auto &x, auto &y) {
      int xv = x.template as<int>(), yv = y.template as<int>();
      return xv == yv;
    };

    methods.is_true = [](auto &val) { return val.template as<int>(); };
  }

  IntType::IntType(const Sym &name, initializer_list<Type> parent_types):
    TType<int>(make_shared<const Imp>(name, parent_types)) {}

  StackType::Imp::Imp(const Sym &name, initializer_list<Type> parent_types): Type::Imp(name, parent_types) {
    methods.dump = [](auto &val, auto &out) { out << val.template as<Stack>(); };

    methods.compare = [](auto &x, auto &y) {
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

    methods.is_equal = [](auto &x, auto &y) {
      const Stack &xs = x.template as<Stack>(), &ys = y.template as<Stack>();
      if (xs.size() != ys.size()) { return false; }

      for (auto xi = xs.begin(), yi = ys.begin(); xi != xs.end(); xi++, yi++) {
	if (*xi != *yi) { return false; }
      }

      return true;
    };

    methods.is_true = [](auto &val) { return !val.template as<Stack>().empty(); };
  }

  StackType::StackType(const Sym &name, initializer_list<Type> parent_types):
    TType<Stack>(make_shared<const Imp>(name, parent_types)) {}

  SymType::Imp::Imp(const Sym &name, initializer_list<Type> parent_types): Type::Imp(name, parent_types) {
    methods.dump = [](auto &val, auto &out) { out << '\'' << val.template as<Sym>(); };

    methods.compare = [](auto &x, auto &y) {
      const Sym &xv = x.template as<Sym>(), &yv = y.template as<Sym>();
      return compare(xv, yv);
    };

    methods.is_equal = [](auto &x, auto &y) {
      Sym xv = x.template as<Sym>(), yv = y.template as<Sym>();
      return xv == yv;
    };
  }

  SymType::SymType(const Sym &name, initializer_list<Type> parent_types):
    TType<Sym>(make_shared<const Imp>(name, parent_types)) {}

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
		 Form x = pop_front(in);
		 optional<Val> xv = x.val(vm);
		 if (!xv) { x.emit(in, vm); }
		 
		 Form y = pop_front(in);
		 optional<Val> yv = y.val(vm);
		 if (!yv) { y.emit(in, vm); }
		 
		 vm.emit<ops::Equal>(form, xv, yv);
	       });
    
    bind_macro(vm.sym("bench"), 2,
	       [](const Macro &self, const Form &form, deque<Form> &in, VM &vm) {
		 Form reps_form = pop_front(in), body_form = pop_front(in);
		 PC op_pc = vm.pc();
		 vm.emit<ops::Bench>(form, reps_form.val(vm)->as<int>());
		 body_form.emit(in, vm);
		 vm.emit<ops::Stop>();
		 vm.ops[op_pc].as<ops::Bench>().end_pc = vm.pc();
	       });

    bind_func(vm.sym("call"),
	      {{vm.sym("target"), vm.libs.abc.func_type}},
	      {},
	      [](const Func &self, CallFlags flags, const Pos &pos, PC ret_pc, VM &vm) {
		return vm.pop().as<Func>().eval(flags, pos, ret_pc, vm);
	      });

    bind_macro(vm.sym("cp"), 0,
	       [](const Macro &self, const Form &form, deque<Form> &in, VM &vm) {
		 vm.emit<ops::Copy>(form);
	       });

    bind_func(vm.sym("dump"),
	      {{vm.sym("val"), vm.libs.abc.any_type}},
	      {},
	      [](const Func &self, CallFlags flags, const Pos &pos, PC ret_pc, VM &vm) {
		vm.pop().dump(cout);
		cout << endl;
		return ret_pc;
	      });

    bind_macro(vm.sym("func"), 4,
	       [](const Macro &self, const Form &form, deque<Form> &in, VM &vm) {
		 Form name_form = pop_front(in);
		 bool lambda = name_form.is<forms::Group>();
		 Sym name = lambda ? vm.sym("") : name_form.as<forms::Id>().name;
		 
		 Form args_form = lambda ? name_form : pop_front(in);
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
		 
		 Form rets_form = pop_front(in);
		 vector<Type> rets;

		 for (const Form &rf: rets_form.as<forms::Group>().body) {
		   Val val(vm.get(rf.as<forms::Id>().name));

		   if (val.type != vm.libs.abc.meta_type) {
		     throw EmitError(rf.pos, "Invalid return type: ", val.type);
		   }

		   rets.emplace_back(val.as<Type>());
		 }
		 
		 Form body_form = pop_front(in);
		 PC skip_pc = vm.pc();
		 vm.emit<ops::Goto>(body_form);
		 PC start_pc = vm.pc();
		 Scope &dscope = vm.scope();
		 Func f(name, args, rets, start_pc, dscope.reg_count);

		 if (!lambda && !dscope.bind(name, vm.libs.abc.func_type, f)) {
		   throw EmitError(form.pos, "Dup binding: ", name);
		 }
		 
		 Scope &scope = vm.push_scope(dscope);
		 
		 if (!f.imp->args.empty()) {
		   int offset = 0;
		   
		   for (const Func::Arg *a = &f.imp->args.back(); a >= &f.imp->args.front(); a--) {
		     if (a->name) {
		       optional<Reg> reg = scope.bind(*a->name);
		       if (!reg) { throw EmitError(form.pos, "Dup binding: ", *a->name); }
		       vm.emit<ops::Store>(body_form, *reg, offset);
		     } else {
		       offset++;
		     }
		   }    
		 }
		 
		 body_form.emit(in, vm);
		 vm.pop_scope();
		 vm.emit<ops::Ret>(body_form);
		 vm.ops[skip_pc].as<ops::Goto>().pc = vm.pc();
		 if (lambda) { vm.emit<ops::Push>(form, vm.libs.abc.func_type, f); }
	       });

    bind_macro(vm.sym("if"), 3,
	       [](const Macro &self, const Form &form, deque<Form> &in, VM &vm) {
		 Form cond = pop_front(in);
		 cond.emit(in, vm);
		 PC branch_pc = vm.pc();
		 vm.emit<ops::Branch>(form);
		 Form true_branch = pop_front(in);
		 true_branch.emit(in, vm);
		 PC skip_pc = vm.pc();
		 vm.emit<ops::Goto>(form);
		 vm.ops[branch_pc].as<ops::Branch>().false_pc = vm.pc();
		 Form false_branch = pop_front(in);
		 false_branch.emit(in, vm);
		 vm.ops[skip_pc].as<ops::Goto>().pc = vm.pc();
	       });

    bind_macro(vm.sym("let"), 2,
	       [](const Macro &self, const Form &form, deque<Form> &in, VM &vm) {
		 Form key_form = pop_front(in);
		 if (!key_form.is<forms::Id>()) { throw EmitError(key_form.pos, "Invalid id: ", key_form); }
		 const Sym &key = key_form.as<forms::Id>().name;
		 Form val_form = pop_front(in);
		 optional<Val> val = val_form.val(vm);

		 if (val) {
		   if (!vm.scope().bind(key, *val)) { throw EmitError(form.pos, "Dup binding: ", key.imp->name); }
		 } else {
		   optional<Reg> reg = vm.scope().bind(key);
		   if (!reg) { throw EmitError(form.pos, "Dup binding: ", key); }
		   val_form.emit(in, vm);
		   vm.emit<ops::Store>(key_form, *reg);
		 }
	       });
  }
}
