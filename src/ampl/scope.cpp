#include "ampl/error.hpp"
#include "ampl/scope.hpp"
#include "ampl/vm.hpp"

namespace ampl {
  Scope::Scope(VM &vm, optional<Scope> parent_scope):
    vm(vm),
    reg_count(parent_scope ? parent_scope->reg_count : 0) {}

  void Scope::bind(const Sym &key, const Val &val) {
    auto found = bindings.find(key);
    if (found != bindings.end()) { throw Error("Dup binding: ", key.imp->name); }
    bindings.emplace(key, val);
  }

  Reg Scope::bind(const Sym &key) {
    Reg reg = reg_count++;
    bind(key, vm.libs.abc.reg_type, reg);
    return reg;
  }
}
