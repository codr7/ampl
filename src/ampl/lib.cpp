#include "ampl/lib.hpp"
#include "ampl/vm.hpp"

namespace ampl {
  Lib::Lib(VM &vm): vm(vm) {}
  
  void Lib::bind_func(const Sym &name, const vector<Func::Arg> &args, const vector<Type> &rets, const Func::Body &body) {
    bind(name, vm.libs.abc.func_type, Func(name, args, rets, body));
  }

  void Lib::bind_macro(const Sym &name, uint8_t arg_count, const Macro::Body &body) {
    bind(name, vm.libs.abc.macro_type, Macro(name, arg_count, body));
  }

  void Lib::import() {
    for (auto [k, v]: bindings) { vm.scope.bind(k, v); } 
  }
  
}
