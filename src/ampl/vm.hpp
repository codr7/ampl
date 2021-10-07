#ifndef AMPL_VM_HPP
#define AMPL_VM_HPP

#include <unordered_map>

#include "ampl/ampl.hpp"
#include "ampl/libs/abc.hpp"
#include "ampl/stack.hpp"
#include "ampl/op.hpp"
#include "ampl/env.hpp"
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
      envs.emplace_back();
    }
    
    Sym sym(const string &name) {
      auto found = syms.find(name);
      if (found != syms.end()) { return found->second; }
      auto [i, _] = syms.emplace(name, Sym(name));
      return i->second;
    }

    Val &push(const Val &val) {
      Stack &s = env().stack;
      s.push_back(val);
      return s.back();
    }

    template <typename T>
    Val &push(TType<T> &type, const T &data) {
      Stack &s = env().stack;
      s.emplace_back(type, data);
      return s.back();
    }

    Val &peek() {
      Stack &s = env().stack;
      assert(!s.empty());
      return s.back();
    }

    Val pop() {
      Stack &s = env().stack;
      assert(!s.empty());
      Val v = s.back();
      s.pop_back();
      return v;
    }

    optional<Val> try_pop() {
      Stack &s = env().stack;
      if (s.empty()) { return nullopt; }
      Val v = s.back();
      s.pop_back();
      return v;
    }

    bool drop(uint64_t count = 1) {
      Stack &s = env().stack;
      if (s.size() < count) { return false; }
      s.erase(s.end()-count, s.end());
      return true;
    }

    PC pc() { return ops.size(); }
    
    template <typename T, typename...Args>
    Op &emit(Args&&...args) {
      T data(forward<Args>(args)...);
      ops.push_back(Op(data));
      return ops.back();
    }

    void emit(const deque<Form> &in) {
      deque<Form> tmp(in);

      while (!tmp.empty()) {
	Form f = tmp.front();
	tmp.pop_front();
	f.emit(tmp, *this);
      }
    }
    
    bool eval(PC start_pc);

    Env &env() { return envs.back(); }

    Stack &stack() { return env().stack; }
    
    unordered_map<string, Sym> syms;
    Libs libs;
    Scope scope;
    vector<Op> ops;
    vector<Env> envs;
  };
}

#endif
