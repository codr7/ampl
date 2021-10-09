#ifndef AMPL_SYM_HPP
#define AMPL_SYM_HPP

#include <ostream>
#include <string>

#include "ampl/compare.hpp"

namespace ampl {
  using namespace std;
  
  struct Sym {
    using Id = uint64_t;
    static Id next_id;

    struct Imp {
      Imp(const string_view &name);
      
      Id id;
      string name;
    };
    
    Sym(const string_view &name);

    shared_ptr<const Imp> imp;
  };

  bool operator==(const Sym &lhs, const Sym &rhs);
  bool operator<(const Sym &lhs, const Sym &rhs);

  ostream &operator <<(ostream &out, const Sym &val);

  template <>
  Order compare(const Sym &x, const Sym &y);
}

#endif
