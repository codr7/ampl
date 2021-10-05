#ifndef AMPL_TYPE_HPP
#define AMPL_TYPE_HPP

#include <functional>
#include "ampl/sym.hpp"

namespace ampl {
  using namespace std;

  struct Val;
  struct VM;
  
  struct Type {
    using Id = uint64_t;
    static Id next_id;
    
    struct Methods {
      function<void (const Val &val, ostream &out)> dump;
      function<bool (const Val &x, const Val &y)> is_equal;
      function<bool (const Val &val)> is_true;
    };

    Type(const Sym &name);

    Id id;
    Sym name;
    Methods methods;
  };

  template <typename T>
  struct TType: Type {
    TType(const Sym &name); // Defined in val.hpp
  };

  bool operator==(const Type &lhs, const Type &rhs);
  bool operator!=(const Type &lhs, const Type &rhs);
  ostream &operator <<(ostream &out, const Type &type);
}

#endif
