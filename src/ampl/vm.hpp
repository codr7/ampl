#ifndef AMPL_VM_HPP
#define AMPL_VM_HPP

#include <unordered_map>

#include "ampl/ampl.hpp"
#include "ampl/frame.hpp"
#include "ampl/libs/abc.hpp"
#include "ampl/libs/math.hpp"
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
      Libs(VM &vm): abc(vm), math(vm) {}

      libs::ABC abc;
      libs::Math math;
    };
    
    VM(): libs(*this) {
      scopes.emplace_back(*this);
      envs.emplace_back();
    }
    
    Sym sym(const string &name) {
      auto found = syms.find(name);
      if (found != syms.end()) { return found->second; }
      auto [i, _] = syms.emplace(name, Sym(name));
      return i->second;
    }

    Scope &push_scope() {
      scopes.emplace_back(*this, scopes.back());
      return scopes.back();
    }

    void pop_scope() { scopes.pop_back(); }

    Scope &scope() { return scopes.back(); }
    
    void push_frame(const Func &target, PC ret_pc) {
      frames.emplace_back(target, ret_pc, *this);
    }

    Frame pop_frame() {
      assert(!frames.empty());
      Frame f = frames.back();
      frames.pop_back();
      return f;
    }

    Env &push_env() {
      envs.emplace_back();
      return envs.back();
    }

    Env pop_env() {
      assert(!envs.empty());
      Env e(envs.back());
      envs.pop_back();
      return e;
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

    Val pop(size_t offset) {
      Stack &s = env().stack;
      assert(s.size() > offset);
      auto i = next(s.begin(), s.size() - offset);
      Val v = *i;
      s.erase(i);
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
    vector<Scope> scopes;
    vector<Op> ops;
    vector<Env> envs;
    vector<Frame> frames;
  };
}

#endif
