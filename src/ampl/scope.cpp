#include "ampl/error.hpp"
#include "ampl/scope.hpp"
#include "ampl/vm.hpp"

namespace ampl {
  Scope::Scope(VM &vm, optional<Scope> parent_scope):
    vm(vm),
    reg_count(parent_scope ? parent_scope->reg_count : 0) {}

  bool Scope::bind(const Sym &key, const Val &val) {
    auto found = bindings.find(key);
    if (found != bindings.end()) { return false; }
    bindings.emplace(key, val);
    return true;
  }

  optional<Reg> Scope::bind(const Sym &key) {
    Reg reg = reg_count++;
    if (!bind(key, vm.libs.abc.reg_type, reg)) { return nullopt; }
    return reg;
  }
}
