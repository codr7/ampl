#ifndef AMPL_LIB_HPP
#define AMPL_LIB_HPP

#include <map>
#include "ampl/error.hpp"
#include "ampl/func.hpp"
#include "ampl/macro.hpp"
#include "ampl/sym.hpp"
#include "ampl/type.hpp"
#include "ampl/val.hpp"

namespace ampl {
  struct Lib {
    Lib(VM &vm);
    
    template <typename T>
    void bind(const Sym &key, const TType<T> &type, const T &data) {
      auto found = bindings.find(key);
      if (found != bindings.end()) { throw Error("Dup binding: ", key.name); }
      bindings.emplace(key, Val(type, data));
    }

    void bind_macro(const Sym &name, uint8_t arg_count, const Macro::Body &body);
    void bind_func(const Sym &name, const vector<Func::Arg> &args, const vector<Type> &rets, const Func::Body &body);
    void import();
    
    VM &vm;
    map<Sym, Val> bindings;
  };
}

#endif
