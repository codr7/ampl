#include "ampl/libs/math.hpp"
#include "ampl/type.hpp"
#include "ampl/vm.hpp"

#include <ostream>

namespace ampl::libs {
  Math::Math(VM &vm):
    Lib(vm),
    int_gt_func(bind_func(vm.sym(">"),
			  {{vm.sym("x"), vm.libs.abc.int_type}, {vm.sym("y"), vm.libs.abc.int_type}},
			  {vm.libs.abc.int_type},
			  [](const Func &self, CallFlags flags, const Pos &pos, PC ret_pc, VM &vm) {
			    Val y = vm.pop(), &x = vm.peek();
			    x = Val(vm.libs.abc.bool_type, x.compare(y) == GT);
			    return ret_pc;
			  })),
    int_lt_func(bind_func(vm.sym("<"),
			  {{vm.sym("x"), vm.libs.abc.int_type}, {vm.sym("y"), vm.libs.abc.int_type}},
			  {vm.libs.abc.int_type},
			  [](const Func &self, CallFlags flags, const Pos &pos, PC ret_pc, VM &vm) {
			    Val y = vm.pop(), &x = vm.peek();
			    x = Val(vm.libs.abc.bool_type, x.compare(y) == LT);
			    return ret_pc;
			  })),
    int_sub_func(bind_func(vm.sym("-"),
			   {{vm.sym("x"), vm.libs.abc.int_type}, {vm.sym("y"), vm.libs.abc.int_type}},
			   {vm.libs.abc.int_type},
			   [](const Func &self, CallFlags flags, const Pos &pos, PC ret_pc, VM &vm) {
			     Val y = vm.pop(), &x = vm.peek();
			     x.as<int>() -= y.as<int>();
			     return ret_pc;
			   })) {
    bind_func(vm.sym("+"),
	      {{vm.sym("x"), vm.libs.abc.int_type}, {vm.sym("y"), vm.libs.abc.int_type}},
	      {vm.libs.abc.int_type},
	      [](const Func &self, CallFlags flags, const Pos &pos, PC ret_pc, VM &vm) {
		Val y = vm.pop(), &x = vm.peek();
		x.as<int>() += y.as<int>();
		return ret_pc;
	      });
  }
}
