#include "ampl/error.hpp"
#include "ampl/scope.hpp"

namespace ampl {
  Scope::Scope(optional<Scope> parent_scope) {
    if (parent_scope) { reg_count = parent_scope->reg_count; }
  }

  void Scope::bind(const Sym &key, const Val &val) {
    auto found = bindings.find(key);
    if (found != bindings.end()) { throw Error("Dup binding: ", key.name); }
    bindings.emplace(key, val);
  }

  optional<Val> Scope::find(const Sym &key) const {
    auto found = bindings.find(key);
    if (found == bindings.end()) { return nullopt; }
    return found->second;
  }
}
