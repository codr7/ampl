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
    };

    Type(const Sym &name);

    Id id;
    Sym name;
    Methods methods;
  };

  template <typename T>
  struct TType: Type {
    TType(const Sym &name);
  };

  bool operator==(const Type &lhs, const Type &rhs);
}

#endif
