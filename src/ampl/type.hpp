#ifndef AMPL_TYPE_HPP
#define AMPL_TYPE_HPP

#include <array>
#include <functional>
#include "ampl/sym.hpp"

namespace ampl {
  using namespace std;

  struct Val;
  struct VM;
  
  struct Type {
    using Id = uint64_t;
    static const Id MAX_COUNT = 64;
    static Id next_id;
    
    struct Methods {
      function<void (const Val &val, ostream &out)> dump;
      function<Order (const Val &x, const Val &y)> compare;
      function<bool (const Val &x, const Val &y)> is_equal;
      function<bool (const Val &val)> is_true;
    };

    struct Imp {
      Imp(const Sym &name, initializer_list<Type> parent_types);
      
      Id id;
      Sym name;
      Methods methods;
      array<bool, MAX_COUNT> parent_types;
    };
      
    Type(const Sym &name, initializer_list<Type> parent_types = {});

    template <typename T>
    Type(const shared_ptr<const T> &imp): imp(imp) {}

    bool isa(const Type &other) const { return imp->parent_types[other.imp->id]; }

    shared_ptr<const Imp> imp;
  };

  template <typename T>
  struct TType: Type {
    TType(const Sym &name, initializer_list<Type> parent_types = {}): Type(name, parent_types) {}

    template <typename U>
    TType(const shared_ptr<const U> &imp): Type(imp) {} 
  };

  bool operator==(const Type &lhs, const Type &rhs);
  bool operator!=(const Type &lhs, const Type &rhs);
  ostream &operator <<(ostream &out, const Type &type);
}

#endif
