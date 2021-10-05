#ifndef AMPL_LIB_HPP
#define AMPL_LIB_HPP

#include <map>
#include "ampl/error.hpp"
#include "ampl/sym.hpp"
#include "ampl/type.hpp"
#include "ampl/val.hpp"

namespace ampl {
  struct Lib {
    template <typename T>
    void bind(const Sym &key, const TType<T> &type, const T &data) {
      auto found = bindings.find(key);
      if (found != bindings.end()) { throw Error("Dup binding: ", key.name); }
      bindings.emplace(key, Val(type, data));
    }

    void import(VM &vm);
    
    map<Sym, Val> bindings;
  };
}

#endif
