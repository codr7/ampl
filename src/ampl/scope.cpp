#include "ampl/error.hpp"
#include "ampl/scope.hpp"
#include "ampl/vm.hpp"

namespace ampl {
  Scope::Scope(VM &vm, optional<Scope> parent_scope):
    vm(vm),
    index(parent_scope ? parent_scope->index+1 : 0),
    reg_count(parent_scope ? parent_scope->reg_count : 0) {}

  void Scope::bind(const Sym &key, const Val &val) {
    auto found = bindings.find(key);
    if (found != bindings.end()) { throw Error("Dup binding: ", key.name); }
    bindings.emplace(key, val);
  }

  Reg Scope::bind(const Sym &key) {
    Reg reg = reg_count++;
    bind(key, vm.libs.abc.reg_type, reg);
    return reg;
  }

  Val &Scope::get(const Sym &key) {
    auto found = bindings.find(key);
    assert(found != bindings.end());
    return found->second;    
  }

  optional<Val> Scope::find(const Sym &key) const {
    auto found = bindings.find(key);
    if (found == bindings.end()) { return nullopt; }
    return found->second;
  }
}
