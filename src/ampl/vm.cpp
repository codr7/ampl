#include "ampl/vm.hpp"

namespace ampl {
  VM::Libs::Libs(VM &vm): abc(vm) {}

  VM::VM(): libs(*this) {}

  Sym VM::sym(const string &name) {
    auto found = syms.find(name);
    if (found != syms.end()) { return found->second; }
    auto [i, _] = syms.emplace(name, Sym(name));
    return i->second;
  }

  Op &VM::emit(const Op &op) {
    ops.push_back(op);
    return ops.back();
  }

  Val &VM::push(const Val &val) {
    stack.push_back(val);
    return stack.back();
  }

  Val *VM::peek() { return stack.empty() ? nullptr : &stack.back(); }

  optional<Val> VM::pop() {
    if (stack.empty()) { return nullopt; }
    Val v = stack.back();
    stack.pop_back();
    return v;
  }
}
