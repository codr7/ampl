#ifndef AMPL_SCOPE_HPP
#define AMPL_SCOPE_HPP

#include <map>
#include "ampl/ampl.hpp"
#include "ampl/val.hpp"

namespace ampl {
  using namespace std;

  struct Sym;
  
  struct Scope {
    Scope(VM &vm, optional<Scope> parent_scope = nullopt);

    bool bind(const Sym &key, const Val &val);

    template <typename T>
    bool bind(const Sym &key, const TType<T> &type, const T &data) {
      return bind(key, Val(type, data));
    }
    
    optional<Reg> bind(const Sym &key);
    
    VM &vm;
    Reg reg_count = 0;
    map<Sym, Val> bindings;
  };
}

#endif
