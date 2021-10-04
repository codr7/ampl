#ifndef AMPL_VM_HPP
#define AMPL_VM_HPP

#include <unordered_map>

#include "ampl/libs/abc.hpp"
#include "ampl/stack.hpp"
#include "ampl/sym.hpp"
#include "ampl/type.hpp"
#include "ampl/types.hpp"
#include "ampl/val.hpp"

namespace ampl {
  using namespace std;
    
  struct VM {
    struct Libs {
      Libs(VM &vm);
      libs::ABC abc;
    };
      
    static const size_t SLAB_SIZE = 1024;

    VM();
    
    Sym sym(const string &name);

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
    Stack stack;
  };
}

#endif
