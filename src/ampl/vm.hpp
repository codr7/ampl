#ifndef AMPL_VM_HPP
#define AMPL_VM_HPP

#include <unordered_map>

#include "ampl/ampl.hpp"
#include "ampl/libs/abc.hpp"
#include "ampl/stack.hpp"
#include "ampl/op.hpp"
#include "ampl/proc.hpp"
#include "ampl/scope.hpp"
#include "ampl/sym.hpp"
#include "ampl/type.hpp"
#include "ampl/val.hpp"

namespace ampl {
  using namespace std;
    
  struct VM {
    struct Libs {
      Libs(VM &vm): abc(vm) {}
      libs::ABC abc;
    };
    
    VM(): libs(*this) {
      procs.emplace_back();
    }
    
    Sym sym(const string &name) {
      auto found = syms.find(name);
      if (found != syms.end()) { return found->second; }
      auto [i, _] = syms.emplace(name, Sym(name));
      return i->second;
    }

    Val &push(const Val &val) {
      Stack &s = proc().stack;
      s.push_back(val);
      return s.back();
    }

    template <typename T>
    Val &push(TType<T> &type, const T &data) {
      Stack &s = proc().stack;
      s.emplace_back(type, data);
      return s.back();
    }

    Val *peek() {
      Stack &s = proc().stack;
      return s.empty() ? nullptr : &s.back();
    }

    optional<Val> pop() {
      Stack &s = proc().stack;
      if (s.empty()) { return nullopt; }
      Val v = s.back();
      s.pop_back();
      return v;
    }

    template <typename T, typename...Args>
    Op &emit(Args&&...args) {
      T data(forward<Args>(args)...);
      ops.push_back(Op(data));
      return ops.back();
    }
    
    bool eval(PC start_pc);

    Proc &proc() { return procs.back(); }

    Stack &stack() { return proc().stack; }
    
    unordered_map<string, Sym> syms;
    Libs libs;
    Scope scope;
    vector<Op> ops;
    vector<Proc> procs;
  };
}

#endif
