#ifndef AMPL_VM_HPP
#define AMPL_VM_HPP

#include <unordered_map>

#include "ampl/ampl.hpp"
#include "ampl/libs/abc.hpp"
#include "ampl/stack.hpp"
#include "ampl/op.hpp"
#include "ampl/scope.hpp"
#include "ampl/sym.hpp"
#include "ampl/type.hpp"
#include "ampl/val.hpp"

namespace ampl {
  using namespace std;
    
  struct VM {
    struct Libs {
      Libs(VM &vm);
      libs::ABC abc;
    };
    
    VM();
    
    Sym sym(const string &name);

    template <typename T, typename...Args>
    Op &emit(Args&&...args) {
      T data(forward<Args>(args)...);
      ops.push_back(Op(data));
      return ops.back();
    }
    
    bool eval(PC start_pc);

    Val &push(const Val &val);

    template <typename T>
    Val &push(TType<T> &type, const T &data) {
      stack.emplace_back(type, data);
      return stack.back();
    }

    Val *peek();
    optional<Val> pop();
    
    unordered_map<string, Sym> syms;
    Libs libs;
    Scope scope;
    vector<Op> ops;
    Stack stack;
  };
}

#endif
