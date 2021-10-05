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
      function<bool (const Val &x, const Val &y)> is_equal;
      function<bool (const Val &val)> is_true;
    };

    Type(const Sym &name, initializer_list<Type> parent_types = {});

    bool isa(const Type other) const { return parent_types[other.id]; }
    
    Id id;
    Sym name;
    Methods methods;
    array<bool, MAX_COUNT> parent_types;
  };

  template <typename T>
  struct TType: Type {
    TType(const Sym &name, initializer_list<Type> parent_types = {}); // Defined in val.hpp
  };

  bool operator==(const Type &lhs, const Type &rhs);
  bool operator!=(const Type &lhs, const Type &rhs);
  ostream &operator <<(ostream &out, const Type &type);
}

#endif
