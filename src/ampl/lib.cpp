#include "ampl/lib.hpp"
#include "ampl/vm.hpp"

namespace ampl {
  Lib::Lib(VM &vm): vm(vm) {}

  void Lib::bind(const Type &type) {
    bind(type.imp->name, vm.libs.abc.meta_type, type);
  }			     

  Func Lib::bind_func(const Sym &name, const vector<Func::Arg> &args, const vector<Type> &rets, const Func::Body &body) {
    Func f(name, args, rets, body);
    bind(name, vm.libs.abc.func_type, f);
    return f;
  }

  Macro Lib::bind_macro(const Sym &name, uint8_t arg_count, const Macro::Body &body) {
    Macro m(name, arg_count, body);
    bind(name, vm.libs.abc.macro_type, m);
    return m;
  }

  void Lib::import() {
    Scope &scope = vm.scope();
    for (auto [k, v]: bindings) { scope.bind(k, v); } 
  }
  
}
