#ifndef AMPL_VM_HPP
#define AMPL_VM_HPP

#include <iostream>
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
      push_scope();
      push_env();
    }
    
    Sym sym(const string &name) {
      auto found = syms.find(name);
      if (found != syms.end()) { return found->second; }
      auto [i, _] = syms.emplace(name, Sym(name));
      return i->second;
    }

    Scope &push_scope(optional<Scope> parent_scope = nullopt) {
      scopes.emplace_back(*this, parent_scope);
      return scopes.back();
    }

    void pop_scope() { scopes.pop_back(); }

    Scope &scope() { return scopes.back(); }

    optional<Val> find(const Sym &key) {
      for (Scope *s = &scopes.back(); s >= &scopes.front(); s--) {
	auto found = s->bindings.find(key);
	if (found != s->bindings.end()) { return found->second; }
      }

      return nullopt;
    }

    Val &get(const Sym &key) {
      for (Scope *s = &scopes.back(); s >= &scopes.front(); s--) {
	auto found = s->bindings.find(key);
	if (found != s->bindings.end()) { return found->second; }
      }

      assert(false);
    }
    
    void push_frame(const Func &target, PC ret_pc) {
      frames.emplace_back(target, ret_pc, *this);
    }

    Frame pop_frame() {
      assert(!frames.empty());
      Frame f(move(frames.back()));
      frames.pop_back();
      return f;
    }

    Env &push_env() {
      envs.emplace_back(make_unique<Env>());
      return *envs.back().get();
    }

    unique_ptr<Env> pop_env() {
      assert(!envs.empty());
      unique_ptr<Env> e(envs.back().release());
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

    Val &peek(size_t offset = 0) {
      Stack &s = env().stack;
      assert(offset < s.size());
      return s[s.size() - offset - 1];
    }

    Val pop() {
      Stack &s = env().stack;
      assert(!s.empty());
      Val v(move(s.back()));
      s.pop_back();
      return v;
    }

    Val pop(size_t offset) {
      Stack &s = env().stack;
      assert(offset < s.size());
      auto i = next(s.begin(), s.size() - offset - 1);
      Val v(move(*i));
      s.erase(i);
      return v;
    }

    optional<Val> try_pop() {
      Stack &s = env().stack;
      if (s.empty()) { return nullopt; }
      Val v(move(s.back()));
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
    Op &emit(Args &&...args) {
      T data(forward<Args>(args)...);
      ops.push_back(Op(data));
      return ops.back();
    }

    void emit(const deque<Form> &in) {
      deque<Form> tmp(in);
      PC start_pc = pc();
      
      while (!tmp.empty()) {
	Form f(move(tmp.front()));
	tmp.pop_front();
	f.emit(tmp, *this);
      }

      optional<Op> prev_op;
      
      for (auto op = next(ops.begin(), start_pc); op != ops.end(); op++) {
	if (prev_op && prev_op->code == STORE &&
	    op->code == LOAD &&
	    prev_op->as<ops::Store>().reg == op->as<ops::Load>().reg) {
	  *op-- = Op(ops::Nop());
	  auto &store = op->as<ops::Store>();
	  *op = Op(ops::Store(store.form, store.reg, store.offset, ops::Store::PEEK));
	}

	prev_op = *op;
      }
    }
    
    bool eval(PC start_pc);

    Env &env() {
      assert(!envs.empty());
      return *envs.back().get();
    }

    Stack &stack() { return env().stack; }
    
    unordered_map<string, Sym> syms;
    Libs libs;
    vector<Scope> scopes;
    vector<Op> ops;
    vector<unique_ptr<Env>> envs;
    vector<Frame> frames;
  };
}

#endif
